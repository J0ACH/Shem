#include "ScBridge.h"

#include <QtWidgets/QApplication>
#include <QWidget>

int main(int argc, char** argv){

	QApplication app(argc, argv);

	QWidget *win = new QWidget();
	win->setGeometry(100, 100, 300, 200);
	win->show();

	ScBridge *bridge = new ScBridge(win);	

	return app.exec();
}


ScBridge::ScBridge(QObject * parent):
QProcess(parent),
mIpcServer(new QLocalServer(this)),
mIpcSocket(NULL),
mIpcServerName("SCIde_" + QString::number(QCoreApplication::applicationPid())),
mTerminationRequested(false),
mCompiled(false)
{
	this->startLang();

	connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	connect(mIpcServer, SIGNAL(newConnection()), this, SLOT(onNewIpcConnection()));
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
		printf("OUTPUT: %s", txt);
	}

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
	printf("lang: %s", c_str2);
	
	//emit scPost(postString);
}

void ScBridge::onNewIpcConnection()
{
	if (mIpcSocket)
		// we can handle only one ipc connection at a time
		mIpcSocket->disconnect();

	mIpcSocket = mIpcServer->nextPendingConnection();
	//connect(mIpcSocket, SIGNAL(disconnected()), this, SLOT(finalizeConnection()));
	connect(mIpcSocket, SIGNAL(readyRead()), this, SLOT(onIpcData()));
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

		printf("OUTPUT: %s", receivedData);

		//onResponse(selector, message);

		//emit response(selector, message);
	}
}

ScBridge::~ScBridge()
{

}

