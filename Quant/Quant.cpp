#include "Quant.h"
#include "QuantConfig.h"

int main(int argc, char** argv){

  QApplication app(argc, argv);

#if _DEBUG
  QuantIDE::QuantNEW *win = new QuantIDE::QuantNEW(0, "UserA", 10000, 10001, 10);
  QuantIDE::QuantNEW *win2 = new QuantIDE::QuantNEW(0, "UserB", 10001, 10000, 520);
#else
  QuantIDE::QuantNEW *win = new QuantIDE::QuantNEW(0);
#endif
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
    canvanNEW->setGeometry(10, 10, 1200, 600);
    canvanNEW->show();
    canvanNEW->installEventFilter(this);

    core = new QuantCore(canvanNEW);

    this->initObjects();

    canvanNEW->addPanel(console, "Console");
    canvanNEW->addPanel(customizePanel, "Customize");

    connect(core, SIGNAL(actPrint(QString, QColor, bool)), console, SLOT(addText(QString, QColor, bool)));

    connect(customize, SIGNAL(actDataChanged(DataCustomize)), this, SLOT(onCustomize(DataCustomize)));
    connect(customize, SIGNAL(actDataChanged(DataCustomize)), core, SLOT(onCustomize(DataCustomize)));

    connect(customize, SIGNAL(actDataChanged(DataCustomize)), customizePanel, SLOT(onCustomize(DataCustomize)));
    connect(customizePanel, SIGNAL(actChangeConfirmed(DataCustomize)), customize, SLOT(onModify(DataCustomize)));
    connect(customizePanel, SIGNAL(actUserNameChanged(DataCustomize)), core, SLOT(onChangeUserName(DataCustomize)));
    connect(customizePanel, SIGNAL(actSaveConfirmed(DataCustomize)), customize, SLOT(onSave(DataCustomize)));

    customize->refresh();
  }

  QuantNEW::QuantNEW(QObject *parent, QString userName, int sendPort, int listenPort, int appPosY) : QObject(parent)
  {
    QFontDatabase::addApplicationFont(":/fontText.ttf");
    QFontDatabase::addApplicationFont(":/fontConsole.ttf");

    customize = new Customize(this);

    canvanNEW = new CanvanNEW();
    canvanNEW->setGeometry(10, appPosY, 1000, 500);
    canvanNEW->show();
    canvanNEW->installEventFilter(this);

    core = new QuantCore(canvanNEW, userName, sendPort, listenPort);

    this->initObjects();

    canvanNEW->addPanel(console, "Console");
    canvanNEW->addPanel(customizePanel, "Customize");

    connect(core, SIGNAL(actPrint(QString, QColor, bool)), console, SLOT(addText(QString, QColor, bool)));

    connect(customize, SIGNAL(actDataChanged(DataCustomize)), this, SLOT(onCustomize(DataCustomize)));
    connect(customize, SIGNAL(actDataChanged(DataCustomize)), core, SLOT(onCustomize(DataCustomize)));

    connect(customize, SIGNAL(actDataChanged(DataCustomize)), customizePanel, SLOT(onCustomize(DataCustomize)));
    connect(customizePanel, SIGNAL(actChangeConfirmed(DataCustomize)), customize, SLOT(onModify(DataCustomize)));
    connect(customizePanel, SIGNAL(actSaveConfirmed(DataCustomize)), customize, SLOT(onSave(DataCustomize)));

    customize->refresh();
  }

  void QuantNEW::initObjects()
  {
    console = new Console();
    customizePanel = new CustomizePanel();
    customizePanel->setVisible(false);

    controlsBar = new CanvanNEW_ToolBar();
    canvanNEW->addButtonBar(controlsBar, "Bridge", Qt::ToolBarArea::BottomToolBarArea);
    controlsBar->addButton("Network", QImage(":/network_16px.png"), core, SLOT(onNetChangeState()));
    controlsBar->addButton("Interpretr", QImage(":/lang_16px.png"), core, SLOT(onInterpretChangeState()));
    controlsBar->addButton("Server", QImage(":/server_16px.png"), core, SLOT(onServerChangeState()));
    controlsBar->getButton("Server")->setState(Button::State::FROZEN);

    panelsBar = new CanvanNEW_ToolBar();
    canvanNEW->addButtonBar(panelsBar, "Panels", Qt::ToolBarArea::BottomToolBarArea);

    panelsBar->addButton("Console", QImage(":/console_16px.png"), console, SLOT(onSwitchVisible()));
    panelsBar->getButton("Console")->setState(Button::State::ON);
    connect(console, SIGNAL(actClosed()), panelsBar->getButton("Console"), SLOT(onSwitch()));

    panelsBar->addButton("Customize", QImage(":/customize_16px.png"), customizePanel, SLOT(onSwitchVisible()));
    connect(customizePanel, SIGNAL(actClosed()), panelsBar->getButton("Customize"), SLOT(onSwitch()));

    panelsBar->addButton("NetworkPanel", QImage(":/network_16px.png"), canvanNEW->getPanel("NetworkPanel"), SLOT(onSwitchVisible()));
    panelsBar->getButton("NetworkPanel")->setState(Button::State::ON);
    connect(canvanNEW->getPanel("NetworkPanel"), SIGNAL(actClosed()), panelsBar->getButton("NetworkPanel"), SLOT(onSwitch()));

    panelsBar->addButton("TimePanel", QImage(":/network_16px.png"), canvanNEW->getPanel("TimePanel"), SLOT(onSwitchVisible()));
    panelsBar->getButton("TimePanel")->setState(Button::State::OFF);
    connect(canvanNEW->getPanel("TimePanel"), SIGNAL(actClosed()), panelsBar->getButton("TimePanel"), SLOT(onSwitch()));

    panelsBar->addButton("NodePanel", QImage(":/network_16px.png"), canvanNEW->getPanel("NodePanel"), SLOT(onSwitchVisible()));
    panelsBar->getButton("NodePanel")->setState(Button::State::ON);
    connect(canvanNEW->getPanel("NodePanel"), SIGNAL(actClosed()), panelsBar->getButton("NodePanel"), SLOT(onSwitch()));
  }

  void QuantNEW::onCustomize(DataCustomize data)
  {
    QString qPropertyColors = tr(
      "Jui--Button {qproperty-colorNormal: %1;}"
      "Jui--Button {qproperty-colorOver: %2;}"
      "Jui--Button {qproperty-colorActive: %3;}"
      //"Jui--Button {qproperty-colorFrozen: %3;}"

      "Jui--CanvanNEW {qproperty-colorBackground: %6;}"
      "Jui--CanvanNEW_MenuBar {qproperty-colorBackground: %5;}"
      "Jui--CanvanNEW_StatusBar {qproperty-colorBackground: %5;}"
      "Jui--CanvanNEW_ToolBar {qproperty-colorBackground: %7;}"

      "Jui--PanelNEW {qproperty-colorHeader: %7;}"
      "Jui--PanelNEW {qproperty-colorBackground: %8;}"
      "Jui--PanelNEW {qproperty-colorTitle: %4;}"

      "Jui--Text {qproperty-colorText: %4;}"

      "Jui--ControlBox {qproperty-colorNormal: %1;}"
      "Jui--ControlBox {qproperty-colorOver: %2;}"
      "Jui--ControlBox {qproperty-colorActive: %3;}"
      "Jui--ControlBox {qproperty-colorText: %4;}"

      "QuantIDE--Library {qproperty-colorBackground: %8;}"
      ).arg(
      data.toStyleSheet(DataCustomize::Key::COLOR_NORMAL),
      data.toStyleSheet(DataCustomize::Key::COLOR_OVER),
      data.toStyleSheet(DataCustomize::Key::COLOR_ACTIVE),
      data.toStyleSheet(DataCustomize::Key::COLOR_TEXT),

      data.toStyleSheet(DataCustomize::Key::COLOR_APP_HEADER),
      data.toStyleSheet(DataCustomize::Key::COLOR_APP_BACKGROUND),
      data.toStyleSheet(DataCustomize::Key::COLOR_PANEL_HEADER),
      data.toStyleSheet(DataCustomize::Key::COLOR_PANEL_BACKGROUND)
      );

    QString qPropertyFonts = tr(
      "Jui--PanelNEW {qproperty-fontTitle: %2;}"
      "Jui--PanelNEW {qproperty-fontBig: %1;}"
      "Jui--PanelNEW {qproperty-fontSmall: %2;}"

      "Jui--Console {qproperty-fontConsole: %3;}"

      "Jui--Text {qproperty-font: %2;}"

      "Jui--Button {qproperty-font: %2;}"
      "Jui--ControlBox {qproperty-font: %2;}"
      ).arg(
      data.toStyleSheet(DataCustomize::Key::FONT_BIG),
      data.toStyleSheet(DataCustomize::Key::FONT_SMALL),
      data.toStyleSheet(DataCustomize::Key::FONT_CONSOLE)
      );

    QString qtStyleSheet =
      "QMenuBar { background-color: rgba(0,0,0,0); }"

      "QLineEdit { background-color: rgba(0,0,0,0); border: none;}"

      "QScrollArea { background-color: rgba(0,0,0,0); }"

      "QScrollBar:vertical { width: 4px; }"
      "QScrollBar:horizontal { height: 4px; }"
      "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"
      "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }"
      "QScrollBar::right-arrow:horizontal, QScrollBar::left-arrow:horizontal {	border: none; background: none;	color: none; }"
      "QScrollBar::top-arrow:vertical, QScrollBar::bottom-arrow:vertical {	border: none; background: none;	color: none; }"
      "QScrollBar::add-line:horizontal { border: none; background: none; }"
      "QScrollBar::sub-line:horizontal { border: none;	background: none; }"
      "QScrollBar::add-line:vertical { border: none; background: none; }"
      "QScrollBar::sub-line:vertical { border: none;	background: none; }"

      "QToolButton { border: 0px; }"
      ;

    QString qtStyleSheet_Colors = tr(
      "QTextEdit { color: %3; }"
      "QTextEdit { background-color: %4; }"
      "QTextEdit { selection-background-color: %2; }"

      "QLineEdit { color: %3; }"

      "QScrollBar:vertical { background: %4; }"
      "QScrollBar:horizontal { background: %4; }"
      "QScrollBar::handle:vertical{	background: %1;	min-height: 40px; }"
      "QScrollBar::handle:horizontal{ background: %1; min-height: 40px; }"

      //"QTabBar::tab{ background: %4; border: 1px solid %1; border-bottom-color: %3}"

      "QToolTip { color: %3; }"
      "QToolTip { background-color: %4; }"
      "QToolTip { border: 1px solid %1; }"
      ).arg(
      data.toStyleSheet(DataCustomize::Key::COLOR_NORMAL),
      data.toStyleSheet(DataCustomize::Key::COLOR_ACTIVE),
      data.toStyleSheet(DataCustomize::Key::COLOR_TEXT),

      data.toStyleSheet(DataCustomize::Key::COLOR_PANEL_BACKGROUND)
      );

    QString qtStyleSeet_Fonts = tr(
      "QToolTip { font: %1; }"
      ).arg(
      data.toStyleSheet(DataCustomize::Key::FONT_SMALL)
      );

    qApp->setStyleSheet(qPropertyColors + qPropertyFonts + qtStyleSheet + qtStyleSheet_Colors + qtStyleSeet_Fonts);
  }

  QuantNEW::~QuantNEW()
  {
    qDebug("Quant closing...");
    core->deleteLater();
  }

  //////////////////////////////////////////////////////////////////////////////////

  Quant::Quant(QWidget *parent) : QWidget(parent)
  {

    customize = new Customize(this);
    canvan = new Canvan(this);
    bridge = new ScBridge(this, "");
    // udpServer = new UDPServer(this, bridge);

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
    // connect(this, SIGNAL(bootServerAct()), bridge, SLOT(changeServerState()));
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
    //  connect(buttServer, SIGNAL(pressAct()), bridge, SLOT(changeServerState()));
    connect(bridge, SIGNAL(serverBootInitAct()), this, SLOT(onServerBootInit()));
    connect(bridge, SIGNAL(serverBootDoneAct()), this, SLOT(onServerBootDone()));
    connect(bridge, SIGNAL(serverKillInitAct()), this, SLOT(onServerKillInit()));
    connect(bridge, SIGNAL(serverKillDoneAct()), this, SLOT(onServerKillDone()));
    connect(bridge, SIGNAL(actServerStatus(QStringList)), this, SLOT(onServerStatus(QStringList)));

    //    emit customize->actCustomizeChanged();
    emit bootInterpretAct();

    this->initStyleSheet();
    //    udpServer->initSocket(userName);
  }

  void Quant::initControl()
  {
//    nodePanel = new NodePanel(canvan->screen, bridge, customize);
  //  nodePanel->setTitle("NodePanel");

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

//    globalCode = new CodeEditor(nodePanel);
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
  //  nodePanel->setGeometry(1, 1, screenRect.width(), screenRect.height() - 1);

    buttLang->setGeometry(5, 5, 24, 24);
    buttServer->setGeometry(35, 5, 24, 24);
    buttConsole->setGeometry(95, 5, 24, 24);
    buttCustomize->setGeometry(125, 5, 24, 24);
    buttNetwork->setGeometry(155, 5, 24, 24);

  //  globalCode->setGeometry(10, nodePanel->height() - 40, 350, 30);

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
    // QColor colorAppHeaderBackground;
    //QFont fontTextConsole;
    /*
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
    */
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
    //    udpServer->sendCode(code);
  }

  Quant::~Quant() { }
}


