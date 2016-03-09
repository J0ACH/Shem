#include "ScBridge.h"
#include <QtWidgets/QApplication>


namespace SupercolliderBridge
{

	ScBridge::ScBridge(QObject * parent) :
		QProcess(parent),
		mIpcServer(new QLocalServer(this)),
		mIpcSocket(NULL),
		mIpcServerName("SCBridge_" + QString::number(QCoreApplication::applicationPid())),
		mTerminationRequested(false),
		mCompiled(false)
	{
		//this->startLang();

		connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
		//connect(mIpcServer, SIGNAL(newConnection()), this, SLOT(onNewIpcConnection()));

		//connect(this, SIGNAL(started()), this, SLOT(testStart()));
	}



	void ScBridge::startLang()
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
		}
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
			}
		}
		mTerminationRequested = false;
	}

	void ScBridge::startServer()
	{
		
		evaluateCode("Server.local = Server.default = s;");
		evaluateCode("s.boot;");
		evaluateCode("s.waitForBoot({ 'MOMENT kdy Server nastartoval....'.postln; })");
		emit bootedServer(true);
	}

	void ScBridge::killServer()
	{
		//evaluateCode("Server.killAll;"); // not working?
		evaluateCode("s.quit;");
		emit bootedServer(false);
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

		emit scPost(tr("userCmd: %1").arg(commandString));

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

		emit scPost(postString);
	}


	ScBridge::~ScBridge()
	{

	}
}

