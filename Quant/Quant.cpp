#include "Quant.h"
#include "QuantConfig.h"

int main(int argc, char** argv){

  QApplication app(argc, argv);

#if _DEBUG
  QuantIDE::Quant *win = new QuantIDE::Quant(0, "UserA", 10000, 10001, 10);
  QuantIDE::Quant *win2 = new QuantIDE::Quant(0, "UserB", 10001, 10000, 520);
#else
  QuantIDE::Quant *win = new QuantIDE::Quant(0);
#endif
 
  return app.exec();
}

namespace QuantIDE
{
  Quant::Quant(QObject *parent) : QObject(parent)
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

  Quant::Quant(QObject *parent, QString userName, int sendPort, int listenPort, int appPosY) : QObject(parent)
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

  void Quant::initObjects()
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

  void Quant::onCustomize(DataCustomize data)
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

  Quant::~Quant()
  {
    qDebug("Quant closing...");
    core->deleteLater();
  }

}


