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
	//if (!processStarted)
		//emit statusMessage(tr("Failed to start interpreter!"));

}

void ScBridge::read()
{
	if (mTerminationRequested) {
		// when stopping the language, we don't want to post for longer than 200 ms to prevent the UI to freeze
		if (QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() - mTerminationRequestTime.toMSecsSinceEpoch() > 200)
			return;
	}

	QByteArray out = QProcess::readAll();
	QString postString = QString::fromUtf8(out);
	//emit scPost(postString);
}

ScBridge::~ScBridge()
{

}

