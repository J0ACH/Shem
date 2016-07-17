#include "Quant.h"
#include "QuantConfig.h"

int main(int argc, char** argv){

  QApplication app(argc, argv);

  QuantIDE::QuantNEW *win = new QuantIDE::QuantNEW(0);
  /*
  QuantIDE::Quant *win = new QuantIDE::Quant();
  win->setGeometry(50, 50, 1400, 700);
  win->show();
  */

  return app.exec();
}

namespace QuantIDE
{
  QuantNEW::QuantNEW(QObject *parent) : QObject(parent)
  {
    QFontDatabase::addApplicationFont(":/fontText.ttf");
    QFontDatabase::addApplicationFont(":/fontConsole.ttf");

    customize = new Customize(this);

    canvanNEW = new CanvanNEW();
    canvanNEW->setGeometry(50, 50, 1400, 700);
    canvanNEW->show();

    networkPanel = new PanelNEW();
    customizePanel = new CustomizePanel();

    console = new Console();
    console->setColorBackground(QColor(30, 30, 30));
    console->setColorHeader(QColor(40, 40, 40));
    console->setColorTitle(QColor(130, 30, 30));
    console->setColorText(QColor(130, 130, 130));

    canvanNEW->addPanel(console, "Console");
    canvanNEW->addPanel(networkPanel, "Network");
    canvanNEW->addPanel(customizePanel, "Customize");

    //this->onCustomize();
    //this->initStyleSheet();

    core = new QuantCore(canvanNEW, customize);
    core->addProxySpace();
    core->addNode("testNode1");


    connect(customize, SIGNAL(actDataChanged(Data)), this, SLOT(onCustomize(Data)));
    connect(customize, SIGNAL(actDataChanged(Data)), core, SLOT(onCustomize(Data)));

    connect(customize, SIGNAL(actDataChanged(Data)), customizePanel, SLOT(onCustomize(Data)));
    connect(customizePanel, SIGNAL(actChangeConfirmed(Data)), customize, SLOT(onModify(Data)));
    connect(customizePanel, SIGNAL(actSaveConfirmed(Data)), customize, SLOT(onSave(Data)));

    customize->refresh();

  }

  void QuantNEW::onCustomize(Data data)
  {
    canvanNEW->setColorHeaders(data.getValue_color(DataKey::COLOR_APP_HEADER));
    canvanNEW->setColorBackground(data.getValue_color(DataKey::COLOR_APP_BACKGROUND));
    canvanNEW->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    canvanNEW->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    canvanNEW->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));

    console->setColorHeader(data.getValue_color(DataKey::COLOR_PANEL_HEADER));
    console->setColorBackground(data.getValue_color(DataKey::COLOR_PANEL_BACKGROUND));
    console->setColorTitle(data.getValue_color(DataKey::COLOR_TEXT));
    console->setFontTitle(data.getValue_font(DataKey::FONT_SMALL));
    console->setFont(data.getValue_font(DataKey::FONT_CONSOLE));
    console->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    console->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    console->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));
    console->setColorMsgNormal(data.getValue_color(DataKey::COLOR_MSG_NORMAL));
    console->setColorMsgStatus(data.getValue_color(DataKey::COLOR_MSG_STATUS));
    console->setColorMsgEvaluate(data.getValue_color(DataKey::COLOR_MSG_EVALUATE));
    console->setColorMsgAnswer(data.getValue_color(DataKey::COLOR_MSG_ANSWER));
    console->setColorMsgError(data.getValue_color(DataKey::COLOR_MSG_ERROR));
    console->setColorMsgWarning(data.getValue_color(DataKey::COLOR_MSG_WARNINIG));
    console->setColorMsgBundle(data.getValue_color(DataKey::COLOR_MSG_BUNDLE));

    networkPanel->setColorHeader(data.getValue_color(DataKey::COLOR_PANEL_HEADER));
    networkPanel->setColorBackground(data.getValue_color(DataKey::COLOR_PANEL_BACKGROUND));
    networkPanel->setColorTitle(data.getValue_color(DataKey::COLOR_TEXT));
    networkPanel->setFontTitle(data.getValue_font(DataKey::FONT_SMALL));
    networkPanel->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    networkPanel->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    networkPanel->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));

    QString txt;
    txt.append(tr("QTextEdit { color: %1; }").arg(data.getValue_color(DataKey::COLOR_TEXT).name()));
    txt.append(tr("QTextEdit { background-color: %1; }").arg(data.getValue_color(DataKey::COLOR_PANEL_BACKGROUND).name()));
    txt.append(tr("QTextEdit { selection-background-color: %1; }").arg(data.getValue_color(DataKey::COLOR_ACTIVE).name()));

    txt.append("QScrollBar:vertical { width: 2px; }");
    txt.append("QScrollBar:horizontal { height: 2px; }");
    txt.append(tr("QScrollBar:vertical { background: %1; }").arg(data.getValue_color(DataKey::COLOR_PANEL_BACKGROUND).name()));
    txt.append(tr("QScrollBar:horizontal { background: %1; }").arg(data.getValue_color(DataKey::COLOR_PANEL_BACKGROUND).name()));
    txt.append(tr("QScrollBar::handle:vertical{	background: %1;	min-height: 40px; }").arg(data.getValue_color(DataKey::COLOR_TEXT).name()));
    txt.append(tr("QScrollBar::handle:horizontal{ background: %1; min-height: 40px; }").arg(data.getValue_color(DataKey::COLOR_TEXT).name()));
    txt.append("QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }");
    txt.append("QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }");
    txt.append("QScrollBar::right-arrow:horizontal, QScrollBar::left-arrow:horizontal {	border: none; background: none;	color: none; }");
    txt.append("QScrollBar::top-arrow:vertical, QScrollBar::bottom-arrow:vertical {	border: none; background: none;	color: none; }");
    txt.append("QScrollBar::add-line:horizontal { border: none; background: none; }");
    txt.append("QScrollBar::sub-line:horizontal { border: none;	background: none; }");
    txt.append("QScrollBar::add-line:vertical { border: none; background: none; }");
    txt.append("QScrollBar::sub-line:vertical { border: none;	background: none; }");

    txt.append(tr("QToolTip { color: %1; }").arg(data.getValue_color(DataKey::COLOR_TEXT).name()));
    txt.append(tr("QToolTip { background-color:  %1; }").arg(data.getValue_color(DataKey::COLOR_PANEL_BACKGROUND).name()));
    txt.append(tr("QToolTip { border: 1px solid white; }"));

    qApp->setStyleSheet(txt);
  }

  // bude odstraneno
  /*
  void QuantNEW::onCustomize(Customize *custom)
  {
    canvanNEW->setColorBars(customize->getColor("color_shem_AppHeaderBackground"));

    networkPanel->setFontTitle(customize->getFont("font_shem_TextSmall"));
    networkPanel->setColorTitle(customize->getColor("color_shem_Over"));


    console->setFont(customize->getFont("font_shem_TextConsole"));
    console->setFontTitle(customize->getFont("font_shem_TextSmall"));
    //console->setColorTitle(customize->getColor("color_shem_Over"));
    console->setColorTitle(custom->getColor(Customize::colorOver));

    console->setColorNormal(customize->getColor("color_shem_MsgNormal"));
    console->setColorStatus(customize->getColor("color_shem_MsgStatus"));
    console->setColorEvaluate(customize->getColor("color_shem_MsgEvaluate"));
    console->setColorResult(customize->getColor("color_shem_MsgResult"));
    console->setColorError(customize->getColor("color_shem_MsgError"));
    console->setColorWarning(customize->getColor("color_shem_MsgWarning"));
    console->setColorBundle(customize->getColor("color_shem_MsgBundle"));

    this->initStyleSheet();
  }

  void QuantNEW::initStyleSheet()
  {
    QString txt;

    txt.append(tr("QTextEdit { color: %1; }").arg(customize->getColor("color_shem_Text").name()));
    txt.append(tr("QTextEdit { background-color: %1; }").arg(customize->getColor("color_shem_PanelBackground").name()));
    txt.append(tr("QTextEdit { selection-background-color: %1; }").arg(customize->getColor("color_shem_Active").name()));

    txt.append("QScrollBar:vertical { width: 2px; }");
    txt.append("QScrollBar:horizontal { height: 2px; }");
    txt.append(tr("QScrollBar:vertical { background: %1; }").arg(customize->getColor("color_shem_PanelBackground").name()));
    txt.append(tr("QScrollBar:horizontal { background: %1; }").arg(customize->getColor("color_shem_PanelBackground").name()));
    txt.append(tr("QScrollBar::handle:vertical{	background: %1;	min-height: 40px; }").arg(customize->getColor("color_shem_Text").name()));
    txt.append(tr("QScrollBar::handle:horizontal{ background: %1; min-height: 40px; }").arg(customize->getColor("color_shem_Text").name()));
    txt.append("QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }");
    txt.append("QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }");
    txt.append("QScrollBar::right-arrow:horizontal, QScrollBar::left-arrow:horizontal {	border: none; background: none;	color: none; }");
    txt.append("QScrollBar::top-arrow:vertical, QScrollBar::bottom-arrow:vertical {	border: none; background: none;	color: none; }");
    txt.append("QScrollBar::add-line:horizontal { border: none; background: none; }");
    txt.append("QScrollBar::sub-line:horizontal { border: none;	background: none; }");
    txt.append("QScrollBar::add-line:vertical { border: none; background: none; }");
    txt.append("QScrollBar::sub-line:vertical { border: none;	background: none; }");

    txt.append(tr("QToolTip { color: %1; }").arg(customize->getColor("color_shem_Text").name()));
    txt.append(tr("QToolTip { background-color:  %1; }").arg(customize->getColor("color_shem_PanelBackground").name()));
    txt.append(tr("QToolTip { border: 1px solid white; }"));

    qApp->setStyleSheet(txt);
  }
  */

  QuantNEW::~QuantNEW()
  {
    qDebug("Quant closing...");
    core->deleteLater();
  }



  Quant::Quant(QWidget *parent) : QWidget(parent)
  {

    customize = new Customize(this);
    canvan = new Canvan(this);
    bridge = new ScBridge(this);
    udpServer = new UDPServer(this, bridge);

    //customize->initConfig();

    QFontDatabase::addApplicationFont(":/fontText.ttf");
    QFontDatabase::addApplicationFont(":/fontConsole.ttf");

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
    connect(this, SIGNAL(bootServerAct()), bridge, SLOT(changeServerState()));
    connect(canvan, SIGNAL(resizeScreenAct()), this, SLOT(fitGeometry()));
    connect(canvan, SIGNAL(closeAct()), bridge, SLOT(killBridge()));
    connect(bridge, SIGNAL(killBridgeDoneAct()), this, SLOT(onCloseQuant()));
    connect(globalCode, SIGNAL(sendText(QString)), this, SLOT(onRecivedGlobalCode(QString)));

    // CONFIG
    connect(customize, SIGNAL(actCustomizeChanged()), this, SLOT(onCustomize()));

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
    connect(bridge, SIGNAL(actServerStatus(QStringList)), this, SLOT(onServerStatus(QStringList)));

    //    emit customize->actCustomizeChanged();
    emit bootInterpretAct();

    this->initStyleSheet();
    udpServer->initSocket(userName);
  }

  void Quant::initControl()
  {
    nodePanel = new NodePanel(canvan->screen, bridge, customize);
    nodePanel->setTitle("NodePanel");

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
    nodePanel->setGeometry(1, 1, screenRect.width(), screenRect.height() - 1);

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

  void Quant::onCustomize()
  {
    // qDebug("Quant::onCustomize");
    QColor colorAppHeaderBackground;
    QFont fontTextConsole;

    colorAppBackground = customize->getColor("color_shem_AppBackground");
    colorAppHeaderBackground = customize->getColor("color_shem_AppHeaderBackground");
    colorPanelBackground = customize->getColor("color_shem_PanelBackground");
    colorNormal = customize->getColor("color_shem_Normal");
    colorOver = customize->getColor("color_shem_Over");
    colorActive = customize->getColor("color_shem_Active");
    colorText = customize->getColor("color_shem_Text");

    colorMsgNormal = customize->getColor("color_shem_MsgNormal");
    colorMsgStatus = customize->getColor("color_shem_MsgStatus");
    colorMsgEvaluate = customize->getColor("color_shem_MsgEvaluate");
    colorMsgResult = customize->getColor("color_shem_MsgResult");
    colorMsgError = customize->getColor("color_shem_MsgError");
    colorMsgWarning = customize->getColor("color_shem_MsgWarning");
    colorMsgBundle = customize->getColor("color_shem_MsgBundle");

    fontTextBig = customize->getFont("font_shem_TextBig");
    fontTextSmall = customize->getFont("font_shem_TextSmall");
    fontTextCode = customize->getFont("font_shem_TextCode");
    fontTextConsole = customize->getFont("font_shem_TextConsole");

    userName = customize->getString("string_shem_UserName");

    canvan->setColorHeader(colorAppHeaderBackground);
    canvan->setColorNormal(colorNormal);
    canvan->setColorOver(colorOver);
    canvan->setColorActive(colorActive);
    canvan->setColorText(colorText);

    canvan->mConsole->setColorBackground(colorPanelBackground);
    // canvan->mConsole->setColorTitle(colorText);
    canvan->mConsole->setColorText(colorText);
    // canvan->mConsole->setFontTitle(fontTextBig);
    canvan->mConsole->setFont(fontTextConsole);

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

    QPalette palete = this->palette();
    palete.setColor(this->foregroundRole(), colorText);
    labelServerMeter->setPalette(palete);
    labelServerSynths->setPalette(palete);
    labelServerGroups->setPalette(palete);
  }
  void Quant::initStyleSheet()
  {
    QString txt;

    txt.append(tr("QTextEdit { color: %1; }").arg(colorText.name()));
    txt.append(tr("QTextEdit { background-color: %1; }").arg(colorPanelBackground.name()));
    txt.append(tr("QTextEdit { selection-background-color: %1; }").arg(colorActive.name()));

    txt.append("QScrollBar:vertical { width: 2px; }");
    txt.append("QScrollBar:horizontal { height: 2px; }");
    txt.append(tr("QScrollBar:vertical { background: %1; }").arg(colorPanelBackground.name()));
    txt.append(tr("QScrollBar:horizontal { background: %1; }").arg(colorPanelBackground.name()));
    txt.append(tr("QScrollBar::handle:vertical{	background: %1;	min-height: 40px; }").arg(colorText.name()));
    txt.append(tr("QScrollBar::handle:horizontal{ background: %1; min-height: 40px; }").arg(colorText.name()));
    txt.append("QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }");
    txt.append("QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }");
    txt.append("QScrollBar::right-arrow:horizontal, QScrollBar::left-arrow:horizontal {	border: none; background: none;	color: none; }");
    txt.append("QScrollBar::top-arrow:vertical, QScrollBar::bottom-arrow:vertical {	border: none; background: none;	color: none; }");
    txt.append("QScrollBar::add-line:horizontal { border: none; background: none; }");
    txt.append("QScrollBar::sub-line:horizontal { border: none;	background: none; }");
    txt.append("QScrollBar::add-line:vertical { border: none; background: none; }");
    txt.append("QScrollBar::sub-line:vertical { border: none;	background: none; }");

    txt.append(tr("QToolTip { color: %1; }").arg(colorText.name()));
    txt.append(tr("QToolTip { background-color:  %1; }").arg(colorPanelBackground.name()));
    txt.append(tr("QToolTip { border: 1px solid white; }"));

    this->setStyleSheet(txt);
  }

  /*
    void Quant::onConfigData(QMap<QString, QVariant*> config)
    {
    connect(this, SIGNAL(actConfigData(QMap<QString, QVariant*>)),
    canvan, SLOT(onConfigData(QMap<QString, QVariant*>)));
    connect(this, SIGNAL(actConfigData(QMap<QString, QVariant*>)),
    nodePanel, SLOT(onConfigData(QMap<QString, QVariant*>)));

    //customize->copyProperty(this);

    colorAppBackground = this->property("color_shem_AppBackground").value<QColor>();
    //colorAppBackground = QColor(config.value("color_shem_AppBackground")->value<QColor>());
    colorPanelBackground = this->property("color_shem_PanelBackground").value<QColor>();
    //colorPanelBackground = config.value("color_shem_PanelBackground")->value<QColor>();
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


    //connect(bridge, SIGNAL(msgNormalAct(QString)), this, SLOT(onMsgNormal(QString)));
    //connect(bridge, SIGNAL(msgStatusAct(QString)), this, SLOT(onMsgStatus(QString)));
    //connect(bridge, SIGNAL(msgEvaluateAct(QString)), this, SLOT(onMsgEvaluate(QString)));
    //connect(bridge, SIGNAL(msgResultAct(QString)), this, SLOT(onMsgResult(QString)));
    //connect(bridge, SIGNAL(msgErrorAct(QString)), this, SLOT(onMsgError(QString)));
    //connect(bridge, SIGNAL(msgWarningAct(QString)), this, SLOT(onMsgWarning(QString)));
    //connect(bridge, SIGNAL(msgBundleAct(QString)), this, SLOT(onMsgBundle(QString)));




    //connect(&serverTask, SIGNAL(timeout()), this, SLOT(onServerTask()));


    }

    // INTERPRET
    */

  void Quant::onInterpretBootInit()
  {
    onMsgStatus("Interpret boot init...");
  }
  void Quant::onInterpretBootDone()
  {
    onMsgStatus("Interpret boot done...");
    buttLang->setState(Jui::Button::State::ON);
    buttServer->setStateKeeping(Button::StateKeeping::HOLD);

    bridge->initOSC();
    emit bootServerAct();
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

    onMsgStatus(tr("Quant init done [user: %1]").arg(userName));
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

  void Quant::onRecivedGlobalCode(QString code)
  {
    bridge->question(code, true);
    udpServer->sendCode(code);
  }

  Quant::~Quant() { }
}


