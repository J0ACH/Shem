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

		canvan->println("ScBridge init...");


		canvan->setHeaderHeight(42);
		canvan->setTailHeight(30);
		canvan->setLogo(QImage(":/logo32.png"));

		this->initControl();
		this->fitGeometry();

		canvan->println("Control init...");

		//this->setMouseTracking(true);

		canvan->setTitle("Quant");
		canvan->setVersion(Quant_VERSION_MAJOR, Quant_VERSION_MINOR, Quant_VERSION_PATCH);

		connect(bridge, SIGNAL(statusMessage(QString)), canvan, SLOT(println(QString)));
		connect(bridge, SIGNAL(evaluatedCode(QString)), canvan, SLOT(println(QString)));
		connect(bridge, SIGNAL(scPost(QString)), canvan, SLOT(println(QString)));

		connect(buttLang, SIGNAL(pressAct()), this, SLOT(switchInterpretr()));
		connect(buttServer, SIGNAL(pressAct()), this, SLOT(switchServer()));

		connect(this, SIGNAL(bootInterpretrAct()), bridge, SLOT(startLang()));
		connect(this, SIGNAL(killInterpretrAct()), bridge, SLOT(killLang()));
		connect(this, SIGNAL(bootServerAct()), bridge, SLOT(startServer()));
		connect(this, SIGNAL(killServerAct()), bridge, SLOT(killServer()));

		connect(canvan, SIGNAL(resizeScreenAct()), this, SLOT(fitGeometry()));

		connect(buttEvaluate, SIGNAL(pressAct()), this, SLOT(evaluateCode()));

		connect(testCode, SIGNAL(cursorPositionChanged()), this, SLOT(evaluateCode())); // testBlbost
	}

	void Quant::initControl()
	{
		nodePanel = new NodePanel(canvan->screen);
		nodePanel->setTitle("NodePanel");
		nodePanel->setBackground(QColor(20, 20, 20));
		nodePanel->setTargetCanvan(canvan);
		nodePanel->setTargetBridge(bridge);

		globalCode = new QTextEdit(nodePanel);
		buttEvaluate = new Button(nodePanel);
		buttEvaluate->setText("evaluateGlobal");

		buttLang = new Button(canvan->tail);
		buttLang->setText("Lang");
		buttLang->setStateKeeping(Button::StateKeeping::HOLD);

		buttServer = new Button(canvan->tail);
		buttServer->setText("Server");
		buttServer->setStateKeeping(Button::StateKeeping::TOUCH);

		testCode = new CodeEditor(nodePanel);
	}

	void Quant::fitGeometry()
	{
		QRect screenRect = canvan->screen->rect();
		nodePanel->setGeometry(1, 0, screenRect.width(), screenRect.height());

		globalCode->setGeometry(10, nodePanel->height() - 40, 350, 30);
		buttEvaluate->setGeometry(globalCode->geometry().right() + 5, nodePanel->height() - 40, 90, 30);

		buttLang->setGeometry(5, 5, 50, 20);
		buttServer->setGeometry(60, 5, 50, 20);

		testCode->setGeometry(10, nodePanel->height() - 100, 350, 50);
	}

	void Quant::switchInterpretr()
	{
		switch (bridge->stateInterpret)
		{
		case StateInterpret::OFF:
			emit bootInterpretrAct();
			qDebug("switchInterpretr:bootInterpret");
			buttServer->setStateKeeping(Button::StateKeeping::HOLD);
			break;
		case StateInterpret::RUNNING:
			emit killInterpretrAct();
			buttServer->setStateKeeping(Button::StateKeeping::TOUCH);
			qDebug("switchInterpretr:killInterpret");
			break;
		}
	}

	void Quant::switchServer()
	{
		switch (bridge->stateServer)
		{
		case StateServer::OFF:
			emit bootServerAct();
			qDebug("switchServer:bootServer");
			break;
		case StateServer::RUNNING:
			emit killServerAct();
			qDebug("switchServer:killServer");
			break;
		}
	}

	void Quant::paintEvent(QPaintEvent *paintEvent)
	{
		QPainter painter(this);
		painter.fillRect(QRect(0, 0, width() - 1, height() - 1), QColor(20, 20, 20));
	}


	void Quant::addNode()
	{
		canvan->println("Node add...");
	}

	void Quant::evaluateCode()
	{
		QString code = globalCode->toPlainText();
		qDebug() << "EvaluateAct: " << code;
		bridge->evaluateCode(code);
	}

	Quant::~Quant() { }

	void Quant::closeEvent(QCloseEvent *event)
	{
		bridge->evaluateCode("Server.local.quit;"); // not working?
		bridge->killLang();

	}
}

