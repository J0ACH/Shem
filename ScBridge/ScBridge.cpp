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
		//mIpcServer = new QLocalServer(this);
		//mIpcServerName("SCBridge_" + QString::number(QCoreApplication::applicationPid())),
		//this->startLang();

		stateInterpret = StateInterpret::OFF;
		stateServer = StateServer::OFF;

		connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
		connect(mIpcServer, SIGNAL(newConnection()), this, SLOT(onNewIpcConnection()));
	}


	void ScBridge::onStart()
	{
		if (!mIpcServer->isListening()) // avoid a warning on stderr
			mIpcServer->listen(mIpcServerName);

		QString command = QStringLiteral("ScIDE.connect(\"%1\")").arg(mIpcServerName);
		evaluateCode(command, true);
		qDebug("onStart");
		emit statusMessage("onStart");
		//Main::documentManager()->sendActiveDocument();
	}
	
	void ScBridge::startLang()
	{
		if (stateInterpret == StateInterpret::OFF)
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
				emit statusMessage(tr("Starting ScLang ..."));
				emit bootedLang(true);
				stateInterpret = StateInterpret::RUNNING;
				onStart();
			}
		}
		else
		{
			emit statusMessage(tr("Interpret already running"));
		};
	}

	void ScBridge::killLang()
	{
		this->killServer();

		//evaluateCode("Server.local.quit;");

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
				emit statusMessage(tr("MOMENT kdy LANG umrel...."));
				emit bootedLang(false);
				stateInterpret = StateInterpret::OFF;
			}
		}
		mTerminationRequested = false;
	}
	
	void ScBridge::startServer()
	{
		//if (stateInterpret == StateInterpret::RUNNING)
		//{
			evaluateCode("Server.local = Server.default = s;");
			evaluateCode("s.boot;");
//			evaluateCode("s.waitForBoot({ 'MOMENT kdy Server nastartoval....'.postln; })");
			//stateServer = StateServer::RUNNING;
			//emit bootedServer(true);
		//}
	}

	void ScBridge::killServer()
	{
		//if (stateServer == StateServer::RUNNING)
		//{
			//evaluateCode("Server.killAll;"); // not working?
			evaluateCode("s.quit;");
			//stateServer = StateServer::OFF;
			//emit bootedServer(false);
		//}
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

		//emit evaluatedCode(tr("evalCode: %1").arg(commandString));

		write(&commandChar, 1);
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

		//emit scPost(tr("scPost: %1").arg(postString));
		emit scPost(postString);
	}

	/////////////////////////////////////////////////////////////////////////

	void ScBridge::onNewIpcConnection()
	{
		if (mIpcSocket)
			// we can handle only one ipc connection at a time
			mIpcSocket->disconnect();

		mIpcSocket = mIpcServer->nextPendingConnection();

		emit statusMessage("ScBridge::onNewIpcConnection()");
		emit statusMessage(QString::number(mIpcSocket->socketDescriptor()));

		connect(mIpcSocket, SIGNAL(disconnected()), this, SLOT(finalizeConnection()));
		connect(mIpcSocket, SIGNAL(readyRead()), this, SLOT(onIpcData()));
	}

	void ScBridge::finalizeConnection()
	{
		emit statusMessage("ScBridge::finalizeConnection()");
		mIpcData.clear();
		mIpcSocket->deleteLater();
		mIpcSocket = NULL;
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
			emit response(selector, message);
		}
	}

	void ScBridge::onResponse(const QString & selector, const QString & data)
	{
		static QString serverRunningSelector("defaultServerRunningChanged");

		static QString introspectionSelector("introspection");
		static QString classLibraryRecompiledSelector("classLibraryRecompiled");
		static QString requestCurrentPathSelector("requestCurrentPath");

		emit statusMessage("ScBridge::onResponse()");

		emit statusMessage(tr("IPC selector: %1").arg(selector));

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
				emit bootedServer(false);
				emit statusMessage("ScServer not running...");
			}
			else if (msg[0] == "- true") 
			{
				stateServer = StateServer::RUNNING;
				emit bootedServer(true);
				emit statusMessage("ScServer start run...");
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
			emit statusMessage(tr("IPC message: %1").arg(data));
		}


		/*
		if (selector == introspectionSelector)
		mIntrospectionParser->process(data);

		else if (selector == classLibraryRecompiledSelector){
		mCompiled = true;
		emit classLibraryRecompiled();
		}

		else if (selector == requestCurrentPathSelector)
		Main::documentManager()->sendActiveDocument();
		*/
	}


	ScBridge::~ScBridge()
	{

	}
}

