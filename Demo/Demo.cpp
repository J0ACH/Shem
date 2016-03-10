#include "Demo.h"
#include <QApplication>

using namespace DemoIDE;
using namespace SupercolliderBridge;

int main(int argc, char** argv){

	QApplication app(argc, argv);

	Demo *win = new Demo();
	win->setGeometry(20, 100, 900, 500);
	win->show();

	return app.exec();
}

Demo::Demo()
{
	bridge = new ScBridge(this);

	this->initControlers();

	connect(buttStartLang, SIGNAL(pressed()), bridge, SLOT(startLang()));
	connect(buttKillLang, SIGNAL(pressed()), bridge, SLOT(killLang()));

	connect(buttStartServer, SIGNAL(pressed()), bridge, SLOT(startServer()));
	connect(buttKillServer, SIGNAL(pressed()), bridge, SLOT(killServer()));

	connect(bridge, SIGNAL(bootedLang(bool)), runningLang, SLOT(setVisible(bool)));
	connect(bridge, SIGNAL(bootedServer(bool)), runningServer, SLOT(setVisible(bool)));

	connect(bridge, SIGNAL(scPost(QString)), this, SLOT(consoleAddMsg(QString)));
	connect(bridge, SIGNAL(statusMessage(QString)), this, SLOT(consoleAddMsg(QString)));

	connect(buttBeep, SIGNAL(pressed()), this, SLOT(beep()));
	connect(buttCode, SIGNAL(pressed()), this, SLOT(sendCode()));
	
			
	
	//bridge->startLang();
	//bridge->startServer();
}

void Demo::initControlers()
{
	buttStartLang = new QPushButton(this);
	buttStartLang->setGeometry(10, 10, 190, 30);
	buttStartLang->setText("ScBridge.startLang()");

	buttKillLang = new QPushButton(this);
	buttKillLang->setGeometry(10, 45, 190, 30);
	buttKillLang->setText("ScBridge.killLang()");

	buttStartServer = new QPushButton(this);
	buttStartServer->setGeometry(10, 85, 190, 30);
	buttStartServer->setText("ScBridge.startServer()");

	buttKillServer = new QPushButton(this);
	buttKillServer->setGeometry(10, 120, 190, 30);
	buttKillServer->setText("ScBridge.killServer()");

	console = new QTextEdit(this);
	console->setGeometry(210, 10, 680, 320);
	console->setReadOnly(true);
	console->setOverwriteMode(false);
	console->setFont(QFont("Consolas", 8));
	console->append(tr("Console init test..."));

	runningLang = new QLabel(this);
	runningLang->setGeometry(10, 160, 190, 20);
	runningLang->setText("Lang is running...");
	runningLang->setVisible(false);

	runningServer = new QLabel(this);
	runningServer->setGeometry(10, 180, 190, 20);
	runningServer->setText("Server is running...");
	runningServer->setVisible(false);

	buttBeep = new QPushButton(this);
	buttBeep->setGeometry(10, 210, 190, 50);
	buttBeep->setText("BEEP!");

	editor = new QTextEdit(this);
	editor->setGeometry(210, 350, 680, 50);
	editor->setFont(QFont("Consolas", 8));
	editor->append(tr("Ndef('test', {SinOsc.ar(150!2, 0, Saw.kr(1))}).play;"));

	buttCode = new QPushButton(this);
	buttCode->setGeometry(10, 350, 190, 50);
	buttCode->setText("ScBridge.evaluateCode(String)");

}

void Demo::beep()
{
	bridge->evaluateCode("().play");
}

void Demo::sendCode()
{
	bridge->evaluateCode(editor->toPlainText());
}

void Demo::consoleAddMsg(QString msg)
{
	console->append(msg);
}


Demo::~Demo()
{
	
}

void Demo::closeEvent(QCloseEvent *event)
{
	//bridge->evaluateCode("Server.local.quit;"); // not working?
	//bridge->killLang();
	//close();

	//bridge->killServer();
	//bridge->killLang();
}