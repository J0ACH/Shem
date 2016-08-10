#include "CustomizePanel.h"

namespace QuantIDE
{
  CustomizePanel::CustomizePanel(QWidget *parent) : PanelNEW(parent)
  {
    this->setMinimumWidth(400);
    this->setMinimumHeight(150);
    this->initControl();
  }

  void CustomizePanel::initControl()
  {
    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);

    scrollWidget = new QWidget(this);
    scrollWidget->setStyleSheet("background-color: rgba(0,0,0,0)");
    scrollWidget->setGeometry(0, 0, 300, 1000);

    scrollArea->setWidget(scrollWidget);

    buttonSave = new Button(this);
    buttonSave->setText("Save");
    connect(buttonSave, SIGNAL(pressAct()), this, SLOT(onSavePressed()));

    textApp = new Text(scrollWidget);
    textApp->setText("Application");
    textApp->setFont(getFontBig());

    textColor = new Text(scrollWidget);
    textColor->setText("Colors");
    textColor->setFont(getFontBig());

    textFont = new Text(scrollWidget);
    textFont->setText("Fonts");
    textColor->setFont(getFontBig());

    subTextCanvan = new Text(scrollWidget);
    subTextCanvan->setText("Canvan");
    subTextPanels = new Text(scrollWidget);
    subTextPanels->setText("Panels");
    subTextMsg = new Text(scrollWidget);
    subTextMsg->setText("Messages");

    boxName = new ControlBox(scrollWidget);
    boxName->setLabel("userName");
    boxName->setLabelSize(150);
    connect(boxName, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxBootNetwork = new ControlBox(scrollWidget);
    boxBootNetwork->setLabel("bootNetwork");
    boxBootNetwork->setLabelSize(150);
    connect(boxBootNetwork, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxBootInterpret = new ControlBox(scrollWidget);
    boxBootInterpret->setLabel("bootInterpret");
    boxBootInterpret->setLabelSize(150);
    connect(boxBootInterpret, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxBootServer = new ControlBox(scrollWidget);
    boxBootServer->setLabel("bootServer");
    boxBootServer->setLabelSize(150);
    connect(boxBootServer, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    // COLORS ////////////////////////////////////////

    boxColorNormal = new ControlBox(scrollWidget);
    boxColorNormal->setLabel("normal");
    boxColorNormal->setLabelSize(150);
    connect(boxColorNormal, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxColorOver = new ControlBox(scrollWidget);
    boxColorOver->setLabel("over");
    boxColorOver->setLabelSize(150);
    connect(boxColorOver, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxColorActive = new ControlBox(scrollWidget);
    boxColorActive->setLabel("active");
    boxColorActive->setLabelSize(150);
    connect(boxColorActive, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxColorText = new ControlBox(scrollWidget);
    boxColorText->setLabel("text");
    boxColorText->setLabelSize(150);
    connect(boxColorText, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxColorAppHeader = new ControlBox(scrollWidget);
    boxColorAppHeader->setLabel("AppHeader");
    boxColorAppHeader->setLabelSize(150);
    connect(boxColorAppHeader, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxColorAppBackground = new ControlBox(scrollWidget);
    boxColorAppBackground->setLabel("AppBackground");
    boxColorAppBackground->setLabelSize(150);
    connect(boxColorAppBackground, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxColorPanelHeader = new ControlBox(scrollWidget);
    boxColorPanelHeader->setLabel("header");
    boxColorPanelHeader->setLabelSize(150);
    connect(boxColorPanelHeader, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxColorPanelBackground = new ControlBox(scrollWidget);
    boxColorPanelBackground->setLabel("background");
    boxColorPanelBackground->setLabelSize(150);
    connect(boxColorPanelBackground, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    // MSG ////////////////////////////////////////

    // FONTS ////////////////////////////////////////

    boxFontAntialias = new ControlBox(scrollWidget);
    boxFontAntialias->setLabel("antialias");
    boxFontAntialias->setLabelSize(150);
    connect(boxFontAntialias, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxFontBig = new ControlBox(scrollWidget);
    boxFontBig->setLabel("big");
    boxFontBig->setLabelSize(150);
    connect(boxFontBig, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxFontSmall = new ControlBox(scrollWidget);
    boxFontSmall->setLabel("small");
    boxFontSmall->setLabelSize(150);
    connect(boxFontSmall, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxFontConsole = new ControlBox(scrollWidget);
    boxFontConsole->setLabel("console");
    boxFontConsole->setLabelSize(150);
    connect(boxFontConsole, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

    boxFontCode = new ControlBox(scrollWidget);
    boxFontCode->setLabel("code");
    boxFontCode->setLabelSize(150);
    connect(boxFontCode, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));

  }
  void CustomizePanel::onCustomize(DataCustomize data)
  {
    customizeData = data;

    boxName->setValue(data.getValue_string(DataCustomize::Key::USERNAME));
    boxBootNetwork->setValue(data.toStyleSheet(DataCustomize::Key::BOOL_BOOT_NETWORK));
    boxBootInterpret->setValue(data.toStyleSheet(DataCustomize::Key::BOOL_BOOT_INTERPRETR));
    boxBootServer->setValue(data.toStyleSheet(DataCustomize::Key::BOOL_BOOT_SERVER));

    boxColorNormal->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_NORMAL)));
    boxColorOver->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_OVER)));
    boxColorActive->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_ACTIVE)));
    boxColorText->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_TEXT)));

    boxColorAppHeader->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_APP_HEADER)));
    boxColorAppBackground->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_APP_BACKGROUND)));

    boxColorPanelHeader->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_PANEL_HEADER)));
    boxColorPanelBackground->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_PANEL_BACKGROUND)));

    boxFontAntialias->setValue(data.toStyleSheet(DataCustomize::Key::BOOL_TEXT_ANTIALIASING));
    boxFontBig->setValue(this->font2String(data.getValue_font(DataCustomize::Key::FONT_BIG)));
    boxFontSmall->setValue(this->font2String(data.getValue_font(DataCustomize::Key::FONT_SMALL)));
    boxFontConsole->setValue(this->font2String(data.getValue_font(DataCustomize::Key::FONT_CONSOLE)));
    boxFontCode->setValue(this->font2String(data.getValue_font(DataCustomize::Key::FONT_CODE)));

  }

  void CustomizePanel::onChangePressed()
  {
    // qDebug("CustomizePanel::onChangePressed()");

    customizeData.setValue(DataCustomize::Key::BOOL_BOOT_NETWORK, boxBootNetwork->getValue_bool());
    customizeData.setValue(DataCustomize::Key::BOOL_BOOT_INTERPRETR, boxBootInterpret->getValue_bool());
    customizeData.setValue(DataCustomize::Key::BOOL_BOOT_SERVER, boxBootServer->getValue_bool());

    customizeData.setValue(DataCustomize::Key::COLOR_NORMAL, this->string2Color(boxColorNormal->getValue_string()));
    customizeData.setValue(DataCustomize::Key::COLOR_OVER, this->string2Color(boxColorOver->getValue_string()));
    customizeData.setValue(DataCustomize::Key::COLOR_ACTIVE, this->string2Color(boxColorActive->getValue_string()));
    customizeData.setValue(DataCustomize::Key::COLOR_TEXT, this->string2Color(boxColorText->getValue_string()));

    customizeData.setValue(DataCustomize::Key::COLOR_APP_HEADER, this->string2Color(boxColorAppHeader->getValue_string()));
    customizeData.setValue(DataCustomize::Key::COLOR_APP_BACKGROUND, this->string2Color(boxColorAppBackground->getValue_string()));

    customizeData.setValue(DataCustomize::Key::COLOR_PANEL_HEADER, this->string2Color(boxColorPanelHeader->getValue_string()));
    customizeData.setValue(DataCustomize::Key::COLOR_PANEL_BACKGROUND, this->string2Color(boxColorPanelBackground->getValue_string()));

    customizeData.setValue(DataCustomize::Key::BOOL_TEXT_ANTIALIASING, boxFontAntialias->getValue_bool());
    customizeData.setValue(DataCustomize::Key::FONT_BIG, this->string2Font(boxFontBig->getValue_string()));
    customizeData.setValue(DataCustomize::Key::FONT_SMALL, this->string2Font(boxFontSmall->getValue_string()));
    customizeData.setValue(DataCustomize::Key::FONT_CONSOLE, this->string2Font(boxFontConsole->getValue_string()));
    customizeData.setValue(DataCustomize::Key::FONT_CODE, this->string2Font(boxFontCode->getValue_string()));

    if (customizeData.getValue_string(DataCustomize::Key::USERNAME) != boxName->getValue_string())
    {
      customizeData.setValue(DataCustomize::Key::USERNAME, boxName->getValue_string());
      emit actUserNameChanged(customizeData);
    }
    else
    {
      //qDebug() << customizeData.print("CustomizePanel::onChangePressed");
      emit actChangeConfirmed(customizeData);
    }
  }
  void CustomizePanel::onSavePressed()
  {
    // qDebug("CustomizePanel::onSavePressed()");
    this->onChangePressed();
    emit actSaveConfirmed(customizeData);
  }

  void CustomizePanel::resizeEvent(QResizeEvent *event)
  {


    scrollArea->setGeometry(10, 30, width() - 20, height() - 70);
    scrollWidget->setFixedWidth(scrollArea->width() - 10);

    if (height() < 60) { buttonSave->hide(); }
    else { buttonSave->show(); }
    buttonSave->setGeometry(width() - 100, height() - 30, 80, 20);

    /////////////////////////////
    textApp->setGeometry(5, 5, width() - 20, 20);

    boxName->setGeometry(20, 45, width() - 30, 20);
    boxBootNetwork->setGeometry(20, 75, width() - 30, 20);
    boxBootInterpret->setGeometry(20, 105, width() - 30, 20);
    boxBootServer->setGeometry(20, 135, width() - 30, 20);

    /////////////////////////////
    textColor->setGeometry(5, 175, width() - 20, 20);

    subTextCanvan->setGeometry(20, 205, width() - 30, 20);
    boxColorNormal->setGeometry(20, 235, width() - 30, 20);
    boxColorOver->setGeometry(20, 265, width() - 30, 20);
    boxColorActive->setGeometry(20, 295, width() - 30, 20);
    boxColorText->setGeometry(20, 325, width() - 30, 20);
    boxColorAppHeader->setGeometry(20, 355, width() - 30, 20);
    boxColorAppBackground->setGeometry(20, 385, width() - 30, 20);


    subTextPanels->setGeometry(20, 425, width() - 30, 20);
    boxColorPanelHeader->setGeometry(20, 455, width() - 30, 20);
    boxColorPanelBackground->setGeometry(20, 485, width() - 30, 20);


    subTextMsg->setGeometry(20, 525, width() - 30, 20);

    /////////////////////////////
    textFont->setGeometry(5, 565, width() - 20, 20);

    boxFontAntialias->setGeometry(20, 595, width() - 30, 20);
    boxFontBig->setGeometry(20, 625, width() - 30, 20);
    boxFontSmall->setGeometry(20, 655, width() - 30, 20);
    boxFontConsole->setGeometry(20, 685, width() - 30, 20);
    boxFontCode->setGeometry(20, 715, width() - 30, 20);


    scrollWidget->setFixedHeight(750); // end of scroll

    PanelNEW::resizeEvent(event);
  }

  QString CustomizePanel::color2String(QColor color)
  {
    return tr("%1, %2, %3").arg(
      QString::number(color.red()),
      QString::number(color.green()),
      QString::number(color.blue())
      );
  }

  QString CustomizePanel::font2String(QFont font)
  {
    return tr("%1, %2").arg(
      font.family(),
      QString::number(font.pointSize())
      );
  }

  QColor CustomizePanel::string2Color(QString colTxt)
  {
    QStringList txtColors = colTxt.split(",");
    int red = txtColors[0].remove(" ").toInt();
    int green = txtColors[1].remove(" ").toInt();
    int blue = txtColors[2].remove(" ").toInt();

    return QColor(red, green, blue);
  }

  QFont CustomizePanel::string2Font(QString fontTxt)
  {
    QStringList parts = fontTxt.split(",");
    QFont font;
    if (parts.size() > 0)
    {
      font.setFamily(parts[0]);
      font.setPointSize(parts[1].toInt());
    }
    return font;
  }

  CustomizePanel::~CustomizePanel() { }
}