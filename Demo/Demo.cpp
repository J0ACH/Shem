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

	connect(bridge, SIGNAL(bootedLang(bool)), runningLang, SLOT(setChecked(bool)));
	connect(bridge, SIGNAL(bootedServer(bool)), runningServer, SLOT(setChecked(bool)));

	connect(bridge, SIGNAL(scPost(QString)), this, SLOT(consoleAddMsg(QString)));
	connect(bridge, SIGNAL(statusMessage(QString)), this, SLOT(consoleAddMsg(QString)));

	connect(buttBeep, SIGNAL(pressed()), this, SLOT(beep()));

	
	
			
	
	//bridge->startLang();
	//bridge->startServer();
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
	buttStartServer->setGeometry(10, 85, 150, 30);
	buttStartServer->setText("ScBridge.startServer()");

	buttKillServer = new QPushButton(this);
	buttKillServer->setGeometry(10, 120, 150, 30);
	buttKillServer->setText("ScBridge.killServer()");

	console = new QTextEdit(this);
	console->setGeometry(180, 10, 700, 480);
	console->setReadOnly(true);
	console->setOverwriteMode(false);
	console->setFont(QFont("Consolas", 8));
	console->append(tr("Console init test..."));

	runningLang = new QCheckBox(this);
	runningLang->setGeometry(10, 400, 150, 30);
	runningLang->setText("running Lang");
	
	runningServer = new QCheckBox(this);
	runningServer->setGeometry(10, 430, 150, 30);
	runningServer->setText("running Server");

	buttBeep = new QPushButton(this);
	buttBeep->setGeometry(10, 190, 150, 50);
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
	//bridge->evaluateCode("Server.local.quit;"); // not working?
	//bridge->killLang();
	//close();

	//bridge->killServer();
	//bridge->killLang();
}