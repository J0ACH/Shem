#include "ScBridge.h"
#include <QtWidgets/QApplication>

ScBridge::ScBridge(QObject * parent) :
QProcess(parent),
mIpcServer(new QLocalServer(this)),
mIpcSocket(NULL),
mIpcServerName("SCIde_" + QString::number(QCoreApplication::applicationPid())),
mTerminationRequested(false),
mCompiled(false)
{
	this->startLang();

	connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	//connect(mIpcServer, SIGNAL(newConnection()), this, SLOT(onNewIpcConnection()));
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
		char *txt = "Failed to start interpreter!";
		printf("OUTPUT: %s", txt);
		emit statusMessage(tr("Failed to start interpreter!"));
	}
	else
	{
		char *txt = "Start interpreter done!";
		printf("OUTPUT: %s\n", txt);
	}

}

void ScBridge::stopLang()
{
	char *txt = "Stop interpreter!";
	printf("OUTPUT: %s\n", txt);

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
	}
	mTerminationRequested = false;
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

	QByteArray ba = commandString.toLatin1();
	const char *c_str2 = ba.data();
	printf("command: %s\n", c_str2);

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

	QByteArray ba = postString.toLatin1();
	const char *c_str2 = ba.data();
	printf("msg: %s", c_str2);

	//emit scPost(postString);
}


ScBridge::~ScBridge()
{

}

