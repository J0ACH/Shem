#include "Quant.h"
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
		canvan->msgConsole(QString("Control init..."));

		//this->setMouseTracking(true);

		//canvan->setTitle("Quant");
		//canvan.setVersion(Qnt_VERSION_MAJOR, Qnt_VERSION_MINOR, Qnt_VERSION_PATCH);

		connect(bridge, SIGNAL(statusMessage(QString)), canvan, SLOT(msgConsole(QString)));
		connect(bridge, SIGNAL(scPost(QString)), canvan, SLOT(msgConsole(QString)));

		connect(buttLang, SIGNAL(pressAct()), bridge, SLOT(startLang()));
		connect(buttServer, SIGNAL(pressAct()), bridge, SLOT(startServer()));
		connect(buttAddNode, SIGNAL(pressAct()), this, SLOT(addNode()));

	}

	void Quant::initControl()
	{

		buttLang = new Button(canvan->screen);
		buttLang->setGeometry(15, 5, 80, 20);
		buttLang->setText("Lang");

		buttServer = new Button(canvan->screen);
		buttServer->setGeometry(15, 35, 80, 20);
		buttServer->setText("Server");

		buttAddNode = new Button(canvan->screen);
		buttAddNode->setGeometry(400, 5, 80, 30);
		buttAddNode->setText("AddNode");

		testPanel = new Panel(this);
		testPanel->setGeometry(200, 100, 400, 500);
		testPanel->setTitle("Test");
		testPanel->setBackground(Qt::black);

		testPanel2 = new Panel(testPanel);
		testPanel2->setGeometry(50, 50, 200, 200);
		testPanel2->setTitle("Test");
		testPanel2->setBackground(QColor(120, 30, 30));

	}

	void Quant::paintEvent(QPaintEvent *paintEvent)
	{
		QPainter painter(this);
		painter.fillRect(QRect(0, 0, width() - 1, height() - 1), QColor(20, 20, 20));
	}

	void Quant::resizeEvent(QResizeEvent *resizeEvent)
	{
		canvan->msgConsole(tr("TOPView resize [%1, %2]").arg(QString::number(width()), QString::number(height())));
	}

	void Quant::addNode()
	{
		canvan->msgConsole(QString("Node add..."));
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

