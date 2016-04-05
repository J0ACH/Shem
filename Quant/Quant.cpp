#include "Quant.h"
#include "QuantConfig.h"

#include <QApplication>

//using namespace Jui;
using namespace SupercolliderBridge;

int main(int argc, char** argv){

	QApplication app(argc, argv);

	QuantIDE::Quant *win = new QuantIDE::Quant();
	win->setGeometry(50, 50, 1200, 700);
	win->show();

	return app.exec();
}

namespace QuantIDE
{

	Quant::Quant(QWidget *parent) : QWidget(parent)
	{
		canvan = new Canvan(this);
		bridge = new ScBridge(this);

		canvan->msgConsole(QString("ScBridge init..."));

		canvan->setHeaderHeight(50);
		canvan->setTailHeight(30);
		canvan->setLogo(QImage(":/logo128.png"));

		this->initControl();
		this->fitGeometry();

		canvan->msgConsole(QString("Control init..."));

		//this->setMouseTracking(true);

		canvan->setTitle("Quant");
		canvan->setVersion(Quant_VERSION_MAJOR, Quant_VERSION_MINOR, Quant_VERSION_PATCH);

		connect(bridge, SIGNAL(statusMessage(QString)), canvan, SLOT(msgConsole(QString)));
		connect(bridge, SIGNAL(scPost(QString)), canvan, SLOT(msgConsole(QString)));

		connect(buttLang, SIGNAL(pressAct()), bridge, SLOT(startLang()));
		connect(buttServer, SIGNAL(pressAct()), bridge, SLOT(startServer()));

		//connect(nodePanel->buttAddNode, SIGNAL(pressAct()), this, SLOT(addNode()));

		connect(testButton, SIGNAL(pressed()), this, SLOT(beep()));

		connect(canvan, SIGNAL(resizeScreenAct()), this, SLOT(fitGeometry()));

		//connect(nodePanel->testNode1, SIGNAL(evaluateAct(QString)), this, SLOT(consoleAddMsg(QString)));
		//connect(nodePanel->testNode2, SIGNAL(evaluateAct(QString)), this, SLOT(consoleAddMsg(QString)));
	}

	void Quant::initControl()
	{
		nodePanel = new NodePanel(canvan->screen);
		nodePanel->setTitle("NodePanel");
		nodePanel->setBackground(QColor(20,20,20));
		nodePanel->setTargetCanvan(canvan);
		nodePanel->setTargetBridge(bridge);
		

		buttLang = new Button(canvan->tail);
		buttLang->setText("Lang");

		buttServer = new Button(canvan->tail);
		buttServer->setText("Server");

		testButton = new QPushButton(canvan->tail);
		testButton->setText("BEEP!");
	}

	void Quant::fitGeometry()
	{
		QRect screenRect = canvan->screen->rect();
		nodePanel->setGeometry(1, 0, screenRect.width(), screenRect.height());

		buttLang->setGeometry(5, 5, 50, 20);
		buttServer->setGeometry(60, 5, 50, 20);
		testButton->setGeometry(115, 5, 50, 20);
	}

	void Quant::paintEvent(QPaintEvent *paintEvent)
	{
		QPainter painter(this);
		painter.fillRect(QRect(0, 0, width() - 1, height() - 1), QColor(20, 20, 20));
	}

	void Quant::resizeEvent(QResizeEvent *resizeEvent)
	{
		//canvan->msgConsole(tr("TOPView resize [%1, %2]").arg(QString::number(width()), QString::number(height())));
	}

	void Quant::addNode()
	{
		canvan->msgConsole(QString("Node add..."));
	}

	void Quant::beep()
	{
		bridge->evaluateCode("().play;");
		canvan->msgConsole(QString("Beep code"));
	}
	/*
	void Quant::startLang()
	{
	canvan->msgConsole(QString("SClang started..."));
	bridge->startLang();
	}
	*/

	void Quant::consoleAddMsg(QString msg)
	{
		qDebug() << "Quant::consoleAddMsg: " << msg;
		canvan->msgConsole(msg);
	}

	Quant::~Quant()
	{

	}

	void Quant::closeEvent(QCloseEvent *event)
	{
		bridge->evaluateCode("Server.local.quit;"); // not working?
		bridge->killLang();

	}
}

