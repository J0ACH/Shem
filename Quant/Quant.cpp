#include "Quant.h"
#include <QApplication>


int main(int argc, char** argv){

	QApplication app(argc, argv);

	Quant *win = new Quant();
	win->setGeometry(100, 100, 700, 500);
	win->show();

	return app.exec();
}

Quant::Quant()
{
	bridge = new ScBridge(this);

	butt1 = new QPushButton(this);
	butt1->setGeometry(10, 10, 50, 50);
	butt1->setText("BEEP!");
	connect(butt1, SIGNAL(pressed()), this, SLOT(beep()));

	butt2 = new QPushButton(this);
	butt2->setGeometry(60, 10, 50, 50);
	butt2->setText("Pdef!");
	connect(butt2, SIGNAL(pressed()), this, SLOT(pdefPlay()));

	console = new QTextEdit(this);
	console->setGeometry(10, 70, 680, 420);
	console->setReadOnly(true);
	console->setOverwriteMode(false);
	console->setFont(QFont("Consolas", 8));
	console->append(tr("Console init test..."));
	connect(bridge, SIGNAL(scPost(QString)), this, SLOT(consoleAddMsg(QString)));
	connect(bridge, SIGNAL(statusMessage(QString)), this, SLOT(consoleAddMsg(QString)));
			
	bridge->evaluateCode("Server.local = Server.default = s;");
	bridge->evaluateCode("s.boot;");
	bridge->evaluateCode("s.waitForBoot(s.scope;{().play});");

}

void Quant::consoleAddMsg(QString msg)
{
	console->append(msg);
}
void Quant::beep()
{
	bridge->evaluateCode("().play");
}

void Quant::pdefPlay()
{
	bridge->evaluateCode("Pdef('test', Pbind('instrument', 'default', 'dur', Pseq([1,1,0.5,0.5], 2), 'freq', 90)).play;");
}

Quant::~Quant()
{
	
}

void Quant::closeEvent(QCloseEvent *event)
{
	bridge->evaluateCode("Server.local.quit;"); // not working?
	bridge->stopLang();
	close();
}