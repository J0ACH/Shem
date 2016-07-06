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
    customize = new Customize(this, bridge);
    udpServer = new UDPServer(this);

    canvan->setHeaderHeight(42);
    canvan->setTailHeight(34);
    canvan->setLogo(QImage(":/logo32.png"));
    canvan->setTitle("Quant");
    canvan->setVersion(tr("v%1").arg(Quant_VERSION));


    //this->initProcessDialog();
    this->initControl();
    this->fitGeometry();

    // CONTROLS
    connect(this, SIGNAL(bootInterpretAct()), bridge, SLOT(changeInterpretState()));
    connect(canvan, SIGNAL(resizeScreenAct()), this, SLOT(fitGeometry()));
    connect(canvan, SIGNAL(closeAct()), bridge, SLOT(killBridge()));
    connect(bridge, SIGNAL(killBridgeDoneAct()), this, SLOT(onCloseQuant()));
    connect(globalCode, SIGNAL(sendText(QString)), this, SLOT(onRecivedGlobalCode(QString)));

     // CONFIG
    connect(customize, SIGNAL(actConfigData(QMap<QString, QVariant*>)),
      this, SLOT(onConfigData(QMap<QString, QVariant*>)));
    connect(this, SIGNAL(actConfigDone()), this, SLOT(onConfigDataDone()));

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
    connect(bridge, SIGNAL(actServerStatus(QStringList)), this, SLOT(onServerStatus(QStringList)));

    emit bootInterpretAct();
    onMsgStatus("Quant init...");
  }

  void Quant::initControl()
  {
    nodePanel = new NodePanel(canvan->screen, bridge);
    nodePanel->setTitle("NodePanel");
    /*
    customizePanel = new Panel(canvan->screen);
    customizePanel->setTitle("Customize");
    */

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

    buttConsole = new Button(canvan->tail);
    buttConsole->setText("Console");
    buttConsole->setStateKeeping(Button::StateKeeping::SWITCH);
    buttConsole->setIcon(QImage(":/console_16px.png"), 0);
    buttConsole->setToolTip("Display console panel");
    buttConsole->setState(Button::ON);

    buttCustomize = new Button(canvan->tail);
    buttCustomize->setText("Customize");
    buttCustomize->setStateKeeping(Button::StateKeeping::SWITCH);
    buttCustomize->setIcon(QImage(":/customize_16px.png"), 0);
    buttCustomize->setToolTip("Display customize panel");
    
    buttNetwork = new Button(canvan->tail);
    buttNetwork->setText("Network");
    buttNetwork->setStateKeeping(Button::StateKeeping::SWITCH);
    buttNetwork->setIcon(QImage(":/network_16px.png"), 0);
    buttNetwork->setToolTip("Display network panel");

    QList<Button*> panelButtons = QList<Button*>();
    panelButtons.append(buttConsole);
    panelButtons.append(buttCustomize);
    panelButtons.append(buttNetwork);
    
    buttConsole->setButtonGroup(panelButtons);
    buttCustomize->setButtonGroup(panelButtons);
    buttNetwork->setButtonGroup(panelButtons);

    globalCode = new CodeEditor(nodePanel);
    globalCode->setText("s.plotTree");

    labelServerMeter = new QLabel(canvan->tail);
    labelServerMeter->setText("NaN");
    labelServerMeter->setToolTip("CPU");

    labelServerSynths = new QLabel(canvan->tail);
    labelServerSynths->setText("0");
    labelServerSynths->setToolTip("numSynths");

    labelServerGroups = new QLabel(canvan->tail);
    labelServerGroups->setText("0");
    labelServerGroups->setToolTip("numGroups");
  }

  void Quant::fitGeometry()
  {
    QRect screenRect = canvan->screen->rect();
    nodePanel->setGeometry(1, 0, screenRect.width(), screenRect.height());

    buttLang->setGeometry(5, 5, 24, 24);
    buttServer->setGeometry(35, 5, 24, 24);
    buttConsole->setGeometry(95, 5, 24, 24);
    buttCustomize->setGeometry(125, 5, 24, 24);
    buttNetwork->setGeometry(155, 5, 24, 24);

    globalCode->setGeometry(10, nodePanel->height() - 40, 350, 30);

    labelServerMeter->setGeometry(canvan->tail->width() - 270, 5, 40, 25);
    labelServerSynths->setGeometry(canvan->tail->width() - 220, 5, 15, 25);
    labelServerGroups->setGeometry(canvan->tail->width() - 200, 5, 15, 25);
  }

  void Quant::onCloseQuant()
  {
    qDebug("Quant closed");
    this->close();
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

    colorAppBackground = QColor(config.value("color_shem_AppBackground")->value<QColor>());
    colorPanelBackground = config.value("color_shem_PanelBackground")->value<QColor>();
    colorNormal = config.value("color_shem_Normal")->value<QColor>();
    colorOver = config.value("color_shem_Over")->value<QColor>();
    colorActive = config.value("color_shem_Active")->value<QColor>();
    colorText = config.value("color_shem_Text")->value<QColor>();

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
    buttConsole->setColorNormal(colorNormal);
    buttCustomize->setColorNormal(colorNormal);
    buttNetwork->setColorNormal(colorNormal);

    buttLang->setColorOver(colorOver);
    buttServer->setColorOver(colorOver);
    buttConsole->setColorOver(colorOver);
    buttCustomize->setColorOver(colorOver);
    buttNetwork->setColorOver(colorOver);

    buttLang->setColorActive(colorActive);
    buttServer->setColorActive(colorActive);
    buttConsole->setColorActive(colorActive);
    buttCustomize->setColorActive(colorActive);
    buttNetwork->setColorActive(colorActive);

    buttLang->setFont(fontTextSmall);
    buttServer->setFont(fontTextSmall);
    buttConsole->setFont(fontTextSmall);
    buttCustomize->setFont(fontTextSmall);
    buttNetwork->setFont(fontTextSmall);

    globalCode->setFontCode(fontTextCode);

    labelServerMeter->setFont(fontTextSmall);
    labelServerSynths->setFont(fontTextSmall);
    labelServerGroups->setFont(fontTextSmall);

    emit actConfigData(config);
    onMsgStatus("Cutomization done...");

    update();

    emit actConfigDone();
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

    connect(this, SIGNAL(bootServerAct()), bridge, SLOT(changeServerState()));

    //connect(&serverTask, SIGNAL(timeout()), this, SLOT(onServerTask()));

    emit bootServerAct();
  }

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

    bridge->OSCtest();
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

  // SERVER

  void Quant::onServerBootInit()
  {
    onMsgStatus("ScServer boot init...\r\n");
  }
  void Quant::onServerBootDone()
  {
    onMsgStatus("ScServer boot done...\r\n");
    QString bootDoneCode;
    bootDoneCode += "\ns.sync;";
    bootDoneCode += "\n2.wait;";
    bootDoneCode = "\np = ProxySpace.push(s).makeTempoClock;";
    bootDoneCode += "\np.clock.tempo_(60 / 60);";
    bootDoneCode += "p.clock.sched(p.clock.timeToNextBeat(1), {\\beatFlag.postln; 1});";
    bootDoneCode += "\nSynthDef(\\envControl, { |bus, tempo| ReplaceOut.kr(bus, EnvGen.kr(\\env.kr( Env.newClear().asArray ), timeScale:tempo.reciprocal, doneAction: 2)); }).add;";
    bridge->evaluate(tr("(s.waitForBoot({%1}))").arg(bootDoneCode));

    onMsgStatus("ProxySpace init done...");
    onMsgStatus("TempoClock set done...");
    onMsgStatus("SynthDefs send done...");

    buttServer->setState(Jui::Button::State::ON);
  }
  void Quant::onServerKillInit()
  {
    onMsgStatus("ScServer kill init...\r\n");
    labelServerMeter->setText("NaN");
    labelServerSynths->setText("0");
    labelServerGroups->setText("0");
  }
  void Quant::onServerKillDone()
  {
    onMsgStatus("ScServer kill done...\r\n");
    buttServer->setState(Jui::Button::State::OFF);
  }

  void Quant::paintEvent(QPaintEvent *paintEvent)
  {
    QPainter painter(this);
    painter.fillRect(QRect(0, 0, width() - 1, height() - 1), colorAppBackground);
  }
  void Quant::closeEvent(QCloseEvent *event)	{ canvan->close(); }

  void Quant::onServerStatus(QStringList data)
  {
    float peakCPU = data[0].toFloat();

    labelServerMeter->setText(tr("%1 %").arg(QString::number(peakCPU, 'f', 2)));
    labelServerSynths->setText(data[1]);
    labelServerGroups->setText(data[2]);
  }
 
  // GLOBAL CODE

  void Quant::onRecivedGlobalCode(QString code) { bridge->question(code, true); }

  Quant::~Quant() { }
}


