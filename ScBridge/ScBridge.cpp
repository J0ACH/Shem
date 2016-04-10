#include "ScBridge.h"
#include <QtWidgets/QApplication>


namespace SupercolliderBridge
{

	ScBridge::ScBridge(QObject * parent) :
		QProcess(parent),
		mIpcServer(new QLocalServer(this)),
		mIpcSocket(NULL),
		mIpcServerName("SCIde_" + QString::number(QCoreApplication::applicationPid())),
		mTerminationRequested(false),
		mCompiled(false)
	{
		//mIpcServerName("SCBridge_" + QString::number(QCoreApplication::applicationPid())),

		stateInterpret = StateInterpret::OFF;
		stateServer = StateServer::OFF;

		connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
		connect(mIpcServer, SIGNAL(newConnection()), this, SLOT(onNewIpcConnection()));
	}

	void ScBridge::changeInterpretState()
	{
		switch (stateInterpret)
		{
		case StateInterpret::OFF:
			emit interpretBootInitAct();
			startInterpretr();
			break;

		case StateInterpret::RUNNING:
			emit interpretKillInitAct();
			killInterpreter();
			break;
		}
	}
	void ScBridge::changeServerState()
	{
		switch (stateServer)
		{
		case StateServer::OFF:
			emit serverBootInitAct();
			evaluateCode("Server.local = Server.default = s;");
			evaluateCode("s.boot;");
			break;
		case StateServer::RUNNING:
			emit serverKillInitAct();
			evaluateCode("s.quit;");
			break;
		}
	}

	void ScBridge::evaluateCode(QString const & commandString, bool silent)
	{
		if (state() != QProcess::Running) {
			emit statusMessage(tr("Interpreter is not running!"));
			return;
		}

		QByteArray bytesToWrite = commandString.toUtf8();
		size_t writtenBytes = write(bytesToWrite);
		if (writtenBytes != bytesToWrite.size()) {
			emit statusMessage(tr("Error when passing data to interpreter!"));
			return;
		}

		char commandChar = silent ? '\x1b' : '\x0c';

		write(&commandChar, 1);
	}

	void ScBridge::startInterpretr()
	{
		QString sclangCommand = "sclang";
		QString configFile;

		QStringList sclangArguments;
		if (!configFile.isEmpty())
			sclangArguments << "-l" << configFile;
		sclangArguments << "-i" << "scqt";

		QProcess::start(sclangCommand, sclangArguments);
		bool processStarted = QProcess::waitForStarted();
		if (!processStarted)
		{
			emit statusMessage(tr("Failed to start interpreter!"));
		}
		else
		{
			if (!mIpcServer->isListening()) // avoid a warning on stderr
				mIpcServer->listen(mIpcServerName);

			QString command = QStringLiteral("ScIDE.connect(\"%1\")").arg(mIpcServerName);
			evaluateCode(command, true);
		}
	}
	void ScBridge::killInterpreter()
	{
		if (state() != QProcess::Running) {
			emit statusMessage(tr("Interpreter is not running!"));
			return;
		}

		evaluateCode("0.exit", true);
		closeWriteChannel();

		mCompiled = false;
		mTerminationRequested = true;
		mTerminationRequestTime = QDateTime::currentDateTimeUtc();

		bool finished = waitForFinished(200);
		if (!finished && (state() != QProcess::NotRunning)) {
#ifdef Q_OS_WIN32
			kill();
#else
			terminate();
#endif
			bool reallyFinished = waitForFinished(200);
			if (!reallyFinished)
				emit statusMessage(tr("Failed to stop interpreter!"));
			else
			{
				stateInterpret = StateInterpret::OFF;
			}
		}
		mTerminationRequested = false;
	}

	void ScBridge::onReadyRead()
	{
		if (mTerminationRequested) {
			// when stopping the language, we don't want to post for longer than 200 ms to prevent the UI to freeze
			if (QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() - mTerminationRequestTime.toMSecsSinceEpoch() > 200)
				return;
		}

		QByteArray out = QProcess::readAll();
		QString postString = QString::fromUtf8(out);

		emit scPost(postString);
	}

	void ScBridge::onNewIpcConnection()
	{
		if (mIpcSocket)
			// we can handle only one ipc connection at a time
			mIpcSocket->disconnect();

		mIpcSocket = mIpcServer->nextPendingConnection();

		connect(mIpcSocket, SIGNAL(disconnected()), this, SLOT(finalizeConnection()));
		connect(mIpcSocket, SIGNAL(readyRead()), this, SLOT(onIpcData()));

		stateInterpret = StateInterpret::RUNNING;
		emit interpretBootDoneAct();
	}

	void ScBridge::finalizeConnection()
	{
		mIpcData.clear();
		mIpcSocket->deleteLater();
		mIpcSocket = NULL;
		emit interpretKillDoneAct();
	}

	void ScBridge::onIpcData()
	{
		mIpcData.append(mIpcSocket->readAll());

		while (mIpcData.size()) {
			QBuffer receivedData(&mIpcData);
			receivedData.open(QIODevice::ReadOnly);

			QDataStream in(&receivedData);
			in.setVersion(QDataStream::Qt_4_6);
			QString selector, message;
			in >> selector;
			if (in.status() != QDataStream::Ok)
				return;

			in >> message;
			if (in.status() != QDataStream::Ok)
				return;

			mIpcData.remove(0, receivedData.pos());

			onResponse(selector, message);
			//emit response(selector, message);
		}
	}

	void ScBridge::onResponse(const QString & selector, const QString & data)
	{
		static QString serverRunningSelector("defaultServerRunningChanged");

		static QString introspectionSelector("introspection");
		static QString classLibraryRecompiledSelector("classLibraryRecompiled");
		static QString requestCurrentPathSelector("requestCurrentPath");

		if (selector == serverRunningSelector)
		{
			// DATA O STAVU SERVERU - msg[0] bool STATE; msg[1] int IP; msg[2] int PORT!!!!!!!!!!!
			QStringList msg = data.split("\n");

			//bool serverRunning;
			//int ip;
			//int port;

			//emit statusMessage(tr("SERVER msg size: %1").arg(msg.size()));
			//emit statusMessage(tr("SERVER msg[0]: %1").arg(msg[0]));
			//emit statusMessage(tr("SERVER msg[1]: %1").arg(msg[1]));
			//emit statusMessage(tr("SERVER msg[2]: %1").arg(msg[2]));

			if (msg[0] == "- false")
			{
				stateServer = StateServer::OFF;
				emit serverKillDoneAct();
			}
			else if (msg[0] == "- true")
			{
				stateServer = StateServer::RUNNING;
				emit serverBootDoneAct();
			}

			//emit statusMessage(tr("SERVER: %1").arg(data));
		}
		else if (selector == introspectionSelector)
		{
			// DATA O VSECH CLASS PRO SUPERCOLIDER!!!!!!
			//emit statusMessage(tr("INTROSPECTION message: %1").arg(data));
		}
		else
		{
			//emit statusMessage(tr("IPC message: %1").arg(data));
		}
	}

	ScBridge::~ScBridge() {	}
}

