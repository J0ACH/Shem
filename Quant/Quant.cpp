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

    customize->setTargetBridge(bridge);


		//this->initProcessDialog();
		this->initControl();
		this->fitGeometry();

		// CONTROLS
		connect(this, SIGNAL(bootInterpretAct()), bridge, SLOT(changeInterpretState()));
		//connect(this, SIGNAL(evaulateAct(QString)), bridge, SLOT(evaluateCode(QString)));
		connect(canvan, SIGNAL(resizeScreenAct()), this, SLOT(fitGeometry()));
		connect(canvan, SIGNAL(closeAct()), bridge, SLOT(killBridge()));
		connect(bridge, SIGNAL(killBridgeDoneAct()), this, SLOT(close()));
		connect(globalCode, SIGNAL(sendText(QString)), this, SLOT(onRecivedGlobalCode(QString)));


		// MSG actions
		connect(this, SIGNAL(print(QString, QColor)), canvan, SLOT(print(QString, QColor)));
		connect(this, SIGNAL(println(QString, QColor)), canvan, SLOT(println(QString, QColor)));

		// INTERPRET actions
		connect(buttLang, SIGNAL(pressAct()), bridge, SLOT(changeInterpretState()));
		connect(bridge, SIGNAL(interpretBootInitAct()), this, SLOT(onInterpretBootInit()));
		connect(bridge, SIGNAL(interpretBootDoneAct()), this, SLOT(onInterpretBootDone()));
		connect(bridge, SIGNAL(interpretKillInitAct()), this, SLOT(onInterpretKillInit()));
		connect(bridge, SIGNAL(interpretKillDoneAct()), this, SLOT(onInterpretKillDone()));

    // CONTROLS
    connect(this, SIGNAL(bootInterpretAct()), bridge, SLOT(changeInterpretState()));
    connect(this, SIGNAL(evaulateAct(QString)), bridge, SLOT(evaluateCode(QString)));
    connect(canvan, SIGNAL(resizeScreenAct()), this, SLOT(fitGeometry()));
    connect(canvan, SIGNAL(closeAct()), bridge, SLOT(killBridge()));
    connect(bridge, SIGNAL(killBridgeDoneAct()), this, SLOT(close()));
    connect(globalCode, SIGNAL(sendText(QString)), bridge, SLOT(evaluateCode(QString)));

		emit bootInterpretAct();
		onMsgStatus("Quant init...");
	}

	void Quant::initControl()
	{
		nodePanel = new NodePanel(canvan->screen, bridge);
		nodePanel->setTitle("NodePanel");

		buttLang = new Button(canvan->tail);
		buttLang->setText("Lang");
		buttLang->setStateKeeping(Button::StateKeeping::HOLD);
		buttLang->setIcon(QImage(":/lang_16px.png"), 0);
		buttLang->setToolTip("Interpreter");

    // MSG actions
    connect(this, SIGNAL(print(QString, QColor)), canvan, SLOT(print(QString, QColor)));
    connect(this, SIGNAL(println(QString, QColor)), canvan, SLOT(println(QString, QColor)));

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
    onMsgStatus("Quant init...\r\n");

		globalCode = new CodeEditor(nodePanel);
		globalCode->setText("s.plotTree");

		labelServerMeter = new QLabel(canvan->tail);
		labelServerMeter->setText("NaN");
		labelServerMeter->setToolTip("CPU");

		labelServerSynths = new QLabel(canvan->tail);
		labelServerSynths->setText("0");
		labelServerSynths->setToolTip("numSynths");
	}

    udpServer = new UDPServer(this);
  }

  void Quant::initControl()
  {
    nodePanel = new NodePanel(canvan->screen, bridge);
    nodePanel->setTitle("NodePanel");

		globalCode->setGeometry(10, nodePanel->height() - 40, 350, 30);

		labelServerMeter->setGeometry(canvan->tail->width() - 200, 5, 40, 25);
		labelServerSynths->setGeometry(canvan->tail->width() - 155, 5, 30, 25);
	}

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
    /*
    testGraph = new Graph(canvan->screen);
    testGraph->setGeometry(100, 270, 500, 250);
    */
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
  void Quant::onMsgNormal(QString msg) { emit print(msg, colorMsgNormal); }
  void Quant::onMsgStatus(QString msg) { emit println(msg, colorMsgStatus); }
  void Quant::onMsgEvaluate(QString msg) { emit println(msg, colorMsgEvaluate); }
  void Quant::onMsgResult(QString msg) { emit println(msg, colorMsgResult); }
  void Quant::onMsgError(QString msg) { emit println(msg, colorMsgError); }
  void Quant::onMsgWarning(QString msg) { emit println(msg, colorMsgWarning); }
  void Quant::onMsgBundle(QString msg) { emit println(msg, colorMsgBundle); }

  // CONFIG

  void Quant::onConfigData(QMap<QString, QVariant*> config)
  {
    connect(this, SIGNAL(actConfigData(QMap<QString, QVariant*>)),
      canvan, SLOT(onConfigData(QMap<QString, QVariant*>)));
    connect(this, SIGNAL(actConfigData(QMap<QString, QVariant*>)),
      nodePanel, SLOT(onConfigData(QMap<QString, QVariant*>)));

		labelServerMeter->setFont(fontTextSmall);
		labelServerSynths->setFont(fontTextSmall);

		emit actConfigData(config);
		onMsgStatus("Cutomization done...");

    colorMsgNormal = config.value("color_shem_MsgNormal")->value<QColor>();
    colorMsgStatus = config.value("color_shem_MsgStatus")->value<QColor>();
    colorMsgEvaluate = config.value("color_shem_MsgEvaluate")->value<QColor>();
    colorMsgResult = config.value("color_shem_MsgResult")->value<QColor>();
    colorMsgError = config.value("color_shem_MsgError")->value<QColor>();
    colorMsgWarning = config.value("color_shem_MsgWarning")->value<QColor>();
    colorMsgBundle = config.value("color_shem_MsgBundle")->value<QColor>();

    fontTextSmall = config.value("font_shem_TextSmall")->value<QFont>();
    fontTextCode = config.value("font_shem_TextCode")->value<QFont>();

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

		connect(&serverTask, SIGNAL(timeout()), this, SLOT(onServerTask()));

		emit bootServerAct();
	}

    buttLang->setFont(fontTextSmall);
    buttServer->setFont(fontTextSmall);
    buttConsol->setFont(fontTextSmall);
    buttNodes->setFont(fontTextSmall);
    buttCustomize->setFont(fontTextSmall);

    globalCode->setFontCode(fontTextCode);

		//this->closeProcessDialog();
	}
	void Quant::onInterpretKillInit()
	{
		onMsgStatus("Interpret kill init...\r\n");
	}
	void Quant::onInterpretKillDone()
	{
		onMsgStatus("Interpret kill done...\r\n");
		buttLang->setState(Jui::Button::State::OFF);
		buttServer->setStateKeeping(Button::StateKeeping::TOUCH);
	}

    update();

	void Quant::onServerBootInit()
	{
		onMsgStatus("ScServer boot init...\r\n");
	}
	void Quant::onServerBootDone()
	{
		onMsgStatus("ScServer boot done...\r\n");
		bridge->evaluateNEW("p = ProxySpace.push(s).makeTempoClock;", true);
		//bridge->evaluateNEW("s.waitForBoot({ p = ProxySpace.push(s).makeTempoClock; })", true);
		bridge->evaluateNEW("p.clock.tempo_(60/60);", true);

		buttServer->setState(Jui::Button::State::ON);

		serverTask.start(1000);
	}
	void Quant::onServerKillInit()
	{
		onMsgStatus("ScServer kill init...\r\n");
		serverTask.stop();
		labelServerMeter->setText("NaN");
	}
	void Quant::onServerKillDone()
	{
		onMsgStatus("ScServer kill done...\r\n");
		buttServer->setState(Jui::Button::State::OFF);
	}

  void Quant::onConfigDataDone()
  {
    connect(bridge, SIGNAL(msgNormalAct(QString)), this, SLOT(onMsgNormal(QString)));
    connect(bridge, SIGNAL(msgStatusAct(QString)), this, SLOT(onMsgStatus(QString)));
    connect(bridge, SIGNAL(msgEvaluateAct(QString)), this, SLOT(onMsgEvaluate(QString)));
    connect(bridge, SIGNAL(msgResultAct(QString)), this, SLOT(onMsgResult(QString)));
    connect(bridge, SIGNAL(msgErrorAct(QString)), this, SLOT(onMsgError(QString)));
    connect(bridge, SIGNAL(msgWarningAct(QString)), this, SLOT(onMsgWarning(QString)));
    connect(bridge, SIGNAL(msgBundleAct(QString)), this, SLOT(onMsgBundle(QString)));

	void Quant::onServerTask()
	{
		
		QString txtCPU = bridge->questionNEW("s.peakCPU").toString();	
		if (txtCPU != NULL)
		{
			double serverCPU = txtCPU.toDouble();
			labelServerMeter->setText(tr("%1 %").arg(QString::number(serverCPU, 'f', 2)));
		}

		QString numSynths = bridge->questionNEW("s.numSynths").toString();
		if (numSynths != NULL)	{ labelServerSynths->setText(numSynths); }

	}

	// GLOBAL CODE

	void Quant::onRecivedGlobalCode(QString code) { bridge->evaluateNEW(code, true); }

	Quant::~Quant() { }
}


