#include "Quant.h"
#include <QApplication>


int main(int argc, char** argv){

	QApplication app(argc, argv);

	Quant *win = new Quant();
	win->setGeometry(100, 100, 300, 200);
	win->show();

	return app.exec();
}

Quant::Quant()
{
	bridge = new ScBridge(this);

	bridge->evaluateCode("Server.local = Server.default = s;");
	bridge->evaluateCode("s.boot;");
	bridge->evaluateCode("s.waitForBoot({().play});");

	butt1 = new QPushButton(this);
	butt1->setGeometry(10, 10, 50, 50);
	butt1->setText("BEEP!");
	connect(butt1, SIGNAL(pressed()), this, SLOT(beep()));

	butt2 = new QPushButton(this);
	butt2->setGeometry(60, 10, 50, 50);
	butt2->setText("Pdef!");
	connect(butt2, SIGNAL(pressed()), this, SLOT(pdefPlay()));
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
	bridge->stopLang();
}