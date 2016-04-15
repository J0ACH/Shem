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

	void ScBridge::killBridge()
	{
		if (stateServer == StateServer::RUNNING)
		{
			connect(this, SIGNAL(serverKillDoneAct()), this, SLOT(killBridge())); // second loop for interpreter
			emit changeServerState();
			emit msgStatusAct("1st round killing");
			return;
		};

		if (stateInterpret == StateInterpret::RUNNING)
		{
			connect(this, SIGNAL(interpretrKillDoneAct()), this, SLOT(killBridge())); // third loop for emit signal
			emit changeInterpretState();
			emit msgStatusAct("2nd round killing");
		};

		if (stateInterpret == StateInterpret::OFF)
		{
			emit msgStatusAct("3rd round killing");
			emit killBridgeDoneAct();
		};
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
		QString oscFunc;

		switch (stateServer)
		{
		case StateServer::OFF:
			emit serverBootInitAct();
			evaluateCode("Server.local = Server.default = s;");
			// evaluateCode("s.dumpOSC;");
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
			emit msgStatusAct(tr("Interpreter is not running!"));
			return;
		}

		QByteArray bytesToWrite = commandString.toUtf8();
		size_t writtenBytes = write(bytesToWrite);
		if (writtenBytes != bytesToWrite.size()) {
			emit msgStatusAct(tr("Error when passing data to interpreter!"));
			return;
		}

		char commandChar = silent ? '\x1b' : '\x0c';
		emit msgEvaluateAct(tr("EVALUATE: %1").arg(commandString));
		write(&commandChar, 1);
	}
	void ScBridge::question(QString pattern, int selector, QString commandString, bool printAnswer)
	{
		//QString command = QStringLiteral("[ \"%1\", %2, %3 ]").arg(pattern, commandString, QString::number(printAnswer));
		QString command = QStringLiteral("[\"ANSWER_MARKER\",\"%1\",%2,%3,%4]").arg(
			pattern,
			QString::number(selector),
			commandString,
			QString::number(printAnswer));
		qDebug() << "ScBridge::question: " << command;
		evaluateCode(command, false);
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
			emit msgStatusAct(tr("Failed to start interpreter!"));
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
			emit msgStatusAct(tr("Interpreter is not running!"));
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
				emit msgStatusAct(tr("Failed to stop interpreter!"));
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

		this->msgFilter(postString);
		/*

		if (postString.startsWith("ERROR:") || postString.startsWith("!"))
		{
		QStringList msgLines = postString.split("\n");
		for (int i = 0; i < msgLines.size(); i = i + 1)
		{
		QString msg = msgLines.at(i);
		msg = msg.replace("\r", "");

		if (msg.startsWith("ERROR:"))
		{
		emit msgErrorAct(msg);
		}
		else if (msg.startsWith("->"))
		{
		msg = msg.remove(0, 3);
		emit msgResultAct(tr("ANSWER: %1").arg(msg));
		}
		else
		{
		emit msgNormalAct(tr("\t- %1").arg(msg));
		}
		};
		}
		else if (postString.startsWith("WARNING:") || postString.startsWith("?"))
		{
		emit msgWarningAct(postString);
		}

		else if (postString.startsWith("->"))
		{
		QString msg = postString.remove(0, 3);
		emit msgResultAct(tr("RESULT: %1").arg(msg));
		}

		else if (postString.startsWith("***")) { emit msgStatusAct(postString); }
		else if (postString.contains("->"))
		{
		qDebug() << "ANSWER: " << postString;
		//QString msg = postString.remove(0, 3);
		emit answerAct(postString, "TEST");
		}
		else if (postString.startsWith("\r\n[ \"#bundle\"")) // neni 100%
		{
		emit msgBundleAct(postString);
		}
		else { emit msgNormalAct(postString); };
		*/
	}

	void ScBridge::msgFilter(QString msg)
	{
		//	qDebug() << "msg: " << msg;

		if (msg.contains("ERROR"))
		{
			emit msgErrorAct(msg);
			/*
			QStringList msgLines = postString.split("\n");
			for (int i = 0; i < msgLines.size(); i = i + 1)
			{
			QString msg = msgLines.at(i);
			msg = msg.replace("\r", "");

			if (msg.startsWith("ERROR:"))
			{
			emit msgErrorAct(msg);
			}
			else
			{
			//emit
			}
			*/
		}
		else if (msg.contains("WARNING")) { emit msgWarningAct(msg); }
		else if (msg.contains("***")) { emit msgStatusAct(msg); }
		else if (msg.contains("->"))
		{
			if (msg.contains("ANSWER_MARKER"))
			{
				qDebug() << "msg [ANSWER_MARKER]: " << msg;

				QStringList incomingMSG = msg.split("->");
				foreach(QString oneMSG, incomingMSG)
				{
					if (oneMSG.contains("ANSWER_MARKER"))
					{
						oneMSG = oneMSG.replace("\r\n", "");
						qDebug() << "oneMSG: " << oneMSG;

						QStringList msgParts = oneMSG.split(",");
						qDebug() << QString("msgParts.size: %1 ").arg(msgParts.size());

						QString pattern;
						QStringList answer;
						int selector, printAnswer;

						for (int i = 0; i < msgParts.size(); i = i + 1)
						{
							QString onePart = msgParts.at(i);
							onePart = onePart.replace(" ", "");
							onePart = onePart.replace("[", "");
							onePart = onePart.replace("]", "");

							qDebug() << "i: " << i;
							if (i == 0) { /* skiping marker */ }
							else if (i == 1) { pattern = onePart; qDebug() << "pattern"; }
							else if (i == 2) { selector = onePart.toInt(); qDebug() << "selector"; }
							else if (i != msgParts.size() - 1) { answer.append(onePart); qDebug() << "answer"; }
							else if (i == msgParts.size() - 1) { printAnswer = onePart.toInt(); qDebug() << "print"; }

						}

						qDebug() << "msgAnswer: "
							<< " pattern " << pattern
							<< " pattern " << QString::number(selector)
							<< " answer " << answer
							<< " print " << pattern;
						emit answerAct(pattern, selector, answer);			
					}

				}
			}
			else { emit msgResultAct(msg); }
		}
		else if (msg.contains("bundle")) { emit msgBundleAct(msg); }
		else { emit msgNormalAct(msg); }
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

		//	emit msgStatusAct(tr("SELECTOR: %1").arg(selector));

		if (selector == serverRunningSelector)
		{
			// DATA O STAVU SERVERU - msg[0] bool STATE; msg[1] int IP; msg[2] int PORT!!!!!!!!!!!
			QStringList msg = data.split("\n");

			//bool serverRunning;
			//int ip;
			//int port;

			//emit msgStatusAct(tr("SERVER msg size: %1").arg(msg.size()));
			//emit msgStatusAct(tr("SERVER msg[0]: %1").arg(msg[0]));
			//emit msgStatusAct(tr("SERVER msg[1]: %1").arg(msg[1]));
			//emit msgStatusAct(tr("SERVER msg[2]: %1").arg(msg[2]));

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

			//emit msgStatusAct(tr("STATUS: %1").arg(data));
		}
		else if (selector == introspectionSelector)
		{
			// DATA O VSECH CLASS PRO SUPERCOLIDER!!!!!!
			//emit msgStatusAct(tr("INTROSPECTION message: %1").arg(data));
		}
		else
		{
			//emit msgStatusAct(tr("IPC message: %1").arg(data));
		}
	}

	ScBridge::~ScBridge() {	}
}

