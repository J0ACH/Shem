#include "Quant.h"
#include "QuantConfig.h"

#include <QApplication>

//using namespace Jui;
using namespace SupercolliderBridge;

int main(int argc, char** argv){

	QApplication app(argc, argv);

	QuantIDE::Quant *win = new QuantIDE::Quant();
	win->setGeometry(50, 50, 1400, 700);
	win->show();

	return app.exec();
}

namespace QuantIDE
{

	Quant::Quant(QWidget *parent) : QWidget(parent)
	{

		canvan = new Canvan(this);
		bridge = new ScBridge(this);
		customize = new Customize(this);

		customize->setTargetBridge(bridge);

		canvan->setHeaderHeight(42);
		canvan->setTailHeight(34);
		canvan->setLogo(QImage(":/logo32.png"));
		canvan->setTitle("Quant");
		canvan->setVersion(Quant_VERSION_MAJOR, Quant_VERSION_MINOR, Quant_VERSION_PATCH);

		this->initControl();
		this->fitGeometry();
		//this->setMouseTracking(true);
		
		// CONTROLS
		connect(this, SIGNAL(bootInterpretAct()), bridge, SLOT(changeInterpretState()));
		connect(this, SIGNAL(evaulateAct(QString)), bridge, SLOT(evaluateCode(QString)));
		connect(canvan, SIGNAL(resizeScreenAct()), this, SLOT(fitGeometry()));
		connect(canvan, SIGNAL(closeAct()), bridge, SLOT(killBridge()));
		connect(bridge, SIGNAL(killBridgeDoneAct()), this, SLOT(close()));
		connect(globalCode, SIGNAL(sendText(QString)), bridge, SLOT(evaluateCode(QString)));

		// CONFIG
		connect(customize, SIGNAL(actConfigData(QMap<QString, QVariant*>)),
			this, SLOT(onConfigData(QMap<QString, QVariant*>)));
		connect(customize, SIGNAL(actConfigData(QMap<QString, QVariant*>)),
			canvan, SLOT(onConfigData(QMap<QString, QVariant*>)));

		// MSG actions
		connect(this, SIGNAL(print(QString, QColor)), canvan, SLOT(print(QString, QColor)));
		connect(this, SIGNAL(println(QString, QColor)), canvan, SLOT(println(QString, QColor)));
		connect(bridge, SIGNAL(msgNormalAct(QString)), this, SLOT(onMsgNormal(QString)));
		connect(bridge, SIGNAL(msgStatusAct(QString)), this, SLOT(onMsgStatus(QString)));
		connect(bridge, SIGNAL(msgEvaluateAct(QString)), this, SLOT(onMsgEvaluate(QString)));
		connect(bridge, SIGNAL(msgResultAct(QString)), this, SLOT(onMsgResult(QString)));
		connect(bridge, SIGNAL(msgErrorAct(QString)), this, SLOT(onMsgError(QString)));
		connect(bridge, SIGNAL(msgWarningAct(QString)), this, SLOT(onMsgWarning(QString)));
		connect(bridge, SIGNAL(msgBundleAct(QString)), this, SLOT(onMsgBundle(QString)));

		// INTERPRET actions
		connect(buttLang, SIGNAL(pressAct()), bridge, SLOT(changeInterpretState()));
		connect(bridge, SIGNAL(interpretBootInitAct()), this, SLOT(onInterpretBootInit()));
		connect(bridge, SIGNAL(interpretBootDoneAct()), this, SLOT(onInterpretBootDone()));
		connect(bridge, SIGNAL(interpretKillInitAct()), this, SLOT(onInterpretKillInit()));
		connect(bridge, SIGNAL(interpretKillDoneAct()), this, SLOT(onInterpretKillDone()));

		// SERVER actions
		connect(buttServer, SIGNAL(pressAct()), bridge, SLOT(changeServerState()));
		connect(bridge, SIGNAL(serverBootInitAct()), this, SLOT(onServerBootInit()));
		connect(bridge, SIGNAL(serverBootDoneAct()), this, SLOT(onServerBootDone()));
		connect(bridge, SIGNAL(serverKillInitAct()), this, SLOT(onServerKillInit()));
		connect(bridge, SIGNAL(serverKillDoneAct()), this, SLOT(onServerKillDone()));

		emit bootInterpretAct();
		onMsgStatus("Quant init...");
	}

	void Quant::initControl()
	{
		nodePanel = new NodePanel(canvan->screen);
		nodePanel->setTitle("NodePanel");
		nodePanel->setTargetBridge(bridge);

		buttLang = new Button(canvan->tail);
		buttLang->setText("Lang");
		buttLang->setStateKeeping(Button::StateKeeping::HOLD);
		buttLang->setIcon(QImage(":/lang_16px.png"), 0);
		buttLang->setToolTip("Interpreter");

		buttServer = new Button(canvan->tail);
		buttServer->setText("Server");
		buttServer->setStateKeeping(Button::StateKeeping::TOUCH);
		buttServer->setIcon(QImage(":/server_16px.png"), 0);
		buttServer->setToolTip("Server");

		buttConsol = new Button(canvan->tail);
		buttConsol->setText("Console");
		buttConsol->setStateKeeping(Button::StateKeeping::TOUCH);
		buttConsol->setToolTip("Display console panel");

		buttNodes = new Button(canvan->tail);
		buttNodes->setText("Nodes");
		buttNodes->setStateKeeping(Button::StateKeeping::TOUCH);
		buttNodes->setToolTip("Display node panel");

		buttCustomize = new Button(canvan->tail);
		buttCustomize->setText("Customize");
		buttCustomize->setStateKeeping(Button::StateKeeping::TOUCH);
		buttCustomize->setToolTip("Customize");

		globalCode = new CodeEditor(nodePanel);
		globalCode->setText("s.sendMsg('/g_dumpTree', 0, 1)");
	}

	void Quant::onConfigData(QMap<QString, QVariant*> config)
	{
		colorAppBackground = QColor(config.value("shem_colorAppBackground")->value<QColor>());
		colorPanelBackground = config.value("shem_colorPanelBackground")->value<QColor>();
		colorNormal = config.value("shem_colorNormal")->value<QColor>();
		colorOver = config.value("shem_colorOver")->value<QColor>();
		colorActive = config.value("shem_colorActive")->value<QColor>();
		colorText = config.value("shem_colorText")->value<QColor>();

		nodePanel->setColorBackground(colorPanelBackground);
		nodePanel->setColorTitle(colorText);

		buttLang->setColorNormal(colorNormal);
		buttServer->setColorNormal(colorNormal);
		buttConsol->setColorNormal(colorNormal);
		buttNodes->setColorNormal(colorNormal);
		buttCustomize->setColorNormal(colorNormal);

		buttLang->setColorOver(colorOver);
		buttServer->setColorOver(colorOver);
		buttConsol->setColorOver(colorOver);
		buttNodes->setColorOver(colorOver);
		buttCustomize->setColorOver(colorOver);

		buttLang->setColorActive(colorActive);
		buttServer->setColorActive(colorActive);
		buttConsol->setColorActive(colorActive);
		buttNodes->setColorActive(colorActive);
		buttCustomize->setColorActive(colorActive);

		update();
	}

	void Quant::fitGeometry()
	{
		QRect screenRect = canvan->screen->rect();
		nodePanel->setGeometry(1, 0, screenRect.width(), screenRect.height());

		buttLang->setGeometry(5, 5, 24, 24);
		buttServer->setGeometry(34, 5, 24, 24);
		buttConsol->setGeometry(60, 5, 60, 24);
		buttNodes->setGeometry(125, 5, 60, 24);
		buttCustomize->setGeometry(190, 5, 60, 24);

		globalCode->setGeometry(10, nodePanel->height() - 40, 350, 30);
	}

	// MSG
	void Quant::onMsgNormal(QString msg) { emit println(msg, QColor(120, 120, 120)); }
	void Quant::onMsgStatus(QString msg) { emit println(msg, QColor(230, 230, 230)); }
	void Quant::onMsgEvaluate(QString msg) { emit println(msg, QColor(30, 130, 230)); }
	void Quant::onMsgResult(QString msg) { emit println(msg, QColor(30, 230, 30)); }
	void Quant::onMsgError(QString msg) { emit println(msg, QColor(230, 30, 30)); }
	void Quant::onMsgWarning(QString msg) { emit println(msg, QColor(230, 130, 30)); }
	void Quant::onMsgBundle(QString msg) { emit println(msg, QColor(170, 160, 20)); }

	// INTERPRET

	void Quant::onInterpretBootInit()
	{
		onMsgStatus("Interpret boot init...");
	}
	void Quant::onInterpretBootDone()
	{
		onMsgStatus("Interpret boot done...");
		buttLang->setState(Jui::Button::State::ON);
		buttServer->setStateKeeping(Button::StateKeeping::HOLD);
	}
	void Quant::onInterpretKillInit()
	{
		onMsgStatus("Interpret kill init...");
	}
	void Quant::onInterpretKillDone()
	{
		onMsgStatus("Interpret kill done...");
		buttServer->setStateKeeping(Button::StateKeeping::TOUCH);
	}

	// SERVER

	void Quant::onServerBootInit()
	{
		onMsgStatus("ScServer boot init...");
	}
	void Quant::onServerBootDone()
	{
		onMsgStatus("ScServer boot done...");
		emit evaulateAct("p = ProxySpace.push(s)");
		//bridge->evaluateCode("p = ProxySpace.push(s)");
	}
	void Quant::onServerKillInit()
	{
		onMsgStatus("ScServer kill init...");
	}
	void Quant::onServerKillDone()
	{
		onMsgStatus("ScServer kill done...");
	}

	void Quant::paintEvent(QPaintEvent *paintEvent)
	{
		QPainter painter(this);
		painter.fillRect(QRect(0, 0, width() - 1, height() - 1), colorAppBackground);
	}
	void Quant::closeEvent(QCloseEvent *event)	{ canvan->close(); }

	Quant::~Quant() { }
}


