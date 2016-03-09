#include "Demo.h"
#include <QApplication>

using namespace DemoIDE;
using namespace SupercolliderBridge;

int main(int argc, char** argv){

	QApplication app(argc, argv);

	Demo *win = new Demo();
	win->setGeometry(100, 100, 900, 500);
	win->show();

	return app.exec();
}

Demo::Demo()
{
	bridge = new ScBridge(this);

	this->initControlers();

	/*
	butt1 = new QPushButton(this);
	butt1->setGeometry(80, 10, 50, 50);
	butt1->setText("BEEP!");
	connect(butt1, SIGNAL(pressed()), this, SLOT(beep()));

	butt2 = new QPushButton(this);
	butt2->setGeometry(60, 10, 50, 50);
	butt2->setText("Pdef!");
	connect(butt2, SIGNAL(pressed()), this, SLOT(pdefPlay()));
	*/

	connect(buttStartLang, SIGNAL(pressed()), bridge, SLOT(startLang()));
	connect(buttKillLang, SIGNAL(pressed()), bridge, SLOT(killLang()));

	connect(buttStartServer, SIGNAL(pressed()), bridge, SLOT(startServer()));
	connect(buttKillServer, SIGNAL(pressed()), bridge, SLOT(killServer()));

	connect(buttBeep, SIGNAL(pressed()), this, SLOT(beep()));

	console = new QTextEdit(this);
	console->setGeometry(180, 10, 700, 480);
	console->setReadOnly(true);
	console->setOverwriteMode(false);
	console->setFont(QFont("Consolas", 8));
	console->append(tr("Console init test..."));
	connect(bridge, SIGNAL(scPost(QString)), this, SLOT(consoleAddMsg(QString)));
	connect(bridge, SIGNAL(statusMessage(QString)), this, SLOT(consoleAddMsg(QString)));
			
	
	bridge->evaluateCode("s.waitForBoot(s.scope;{().play});");

}

void Demo::initControlers()
{
	buttStartLang = new QPushButton(this);
	buttStartLang->setGeometry(10, 10, 150, 30);
	buttStartLang->setText("ScBridge.startLang()");

	buttKillLang = new QPushButton(this);
	buttKillLang->setGeometry(10, 45, 150, 30);
	buttKillLang->setText("ScBridge.killLang()");

	buttStartServer = new QPushButton(this);
	buttStartServer->setGeometry(10, 80, 150, 30);
	buttStartServer->setText("ScBridge.startServer()");

	buttKillServer = new QPushButton(this);
	buttKillServer->setGeometry(10, 115, 150, 30);
	buttKillServer->setText("ScBridge.killServer()");

	
	buttBeep = new QPushButton(this);
	buttBeep->setGeometry(10, 160, 150, 20);
	buttBeep->setText("BEEP!");
	

	

}

void Demo::consoleAddMsg(QString msg)
{
	console->append(msg);
}
void Demo::beep()
{
	bridge->evaluateCode("().play");
}

void Demo::pdefPlay()
{
	bridge->evaluateCode("Pdef('test', Pbind('instrument', 'default', 'dur', Pseq([1,1,0.5,0.5], 2), 'freq', 90)).play;");
}

Demo::~Demo()
{
	
}

void Demo::closeEvent(QCloseEvent *event)
{
	bridge->evaluateCode("Server.local.quit;"); // not working?
	bridge->killLang();
	close();
}