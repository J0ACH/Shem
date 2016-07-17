#include "CustomizePanel.h"

namespace QuantIDE
{
  CustomizePanel::CustomizePanel(QWidget *parent) : PanelNEW(parent)
  {
    this->initControl();
  }

  void CustomizePanel::initControl()
  {
    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setStyleSheet("background-color: rgba(0,0,0,0)");

    scrollWidget = new QWidget(this);
    scrollWidget->setAutoFillBackground(true);
    scrollWidget->setStyleSheet("background-color: rgba(0,0,0,0)");
    scrollWidget->setGeometry(0, 0, 300, 1000);

    scrollArea->setWidget(scrollWidget);

    buttonSave = new Button(this);
    buttonSave->setText("Save");
    connect(buttonSave, SIGNAL(pressAct()), this, SLOT(onSavePressed()));

    textApp = new Text(scrollWidget);
    textApp->setText("Application");
    textColor = new Text(scrollWidget);
    textColor->setText("Colors");
    textFont = new Text(scrollWidget);
    textFont->setText("Fonts");

    subTextCanvan = new Text(scrollWidget);
    subTextCanvan->setText("Canvan");
    subTextPanels = new Text(scrollWidget);
    subTextPanels->setText("Panels");
    subTextMsg = new Text(scrollWidget);
    subTextMsg->setText("Messages");

    boxName = new ControlBox(scrollWidget);
    boxName->setLabel("userName");
    boxName->setLabelSize(150);
    connect(boxName, SIGNAL(actValueChanged(QString)), this, SLOT(onChangePressed()));

    boxColorNormal = new ControlBox(scrollWidget);
    boxColorNormal->setLabel("normal");
    boxColorNormal->setLabelSize(150);
    connect(boxColorNormal, SIGNAL(actValueChanged(QString)), this, SLOT(onChangePressed()));

    boxColorOver = new ControlBox(scrollWidget);
    boxColorOver->setLabel("over");
    boxColorOver->setLabelSize(150);
    connect(boxColorOver, SIGNAL(actValueChanged(QString)), this, SLOT(onChangePressed()));

    boxColorActive = new ControlBox(scrollWidget);
    boxColorActive->setLabel("active");
    boxColorActive->setLabelSize(150);
    connect(boxColorActive, SIGNAL(actValueChanged(QString)), this, SLOT(onChangePressed()));

    boxColorText = new ControlBox(scrollWidget);
    boxColorText->setLabel("text");
    boxColorText->setLabelSize(150);
    connect(boxColorText, SIGNAL(actValueChanged(QString)), this, SLOT(onChangePressed()));

    boxColorPanelBackground = new ControlBox(scrollWidget);
    boxColorPanelBackground->setLabel("background");
    boxColorPanelBackground->setLabelSize(150);
    connect(boxColorPanelBackground, SIGNAL(actValueChanged(QString)), this, SLOT(onChangePressed()));
  }

  QString CustomizePanel::color2String(QColor color)
  {
    return tr("%1, %2, %3").arg(
      QString::number(color.red()),
      QString::number(color.green()),
      QString::number(color.blue())
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

  //bude odstraneno
  /*
  void CustomizePanel::onCustomize(Customize* custom)
  {
  this->setColorHeader(custom->getColor(Customize::colorHeaders));
  this->setColorTitle(custom->getColor(Customize::colorText));
  this->setFontTitle(custom->getFont(Customize::fontSmall));

  buttonConfirm->setColorActive(custom->getColor(Customize::colorActive));
  buttonConfirm->setColorOver(custom->getColor(Customize::colorOver));
  buttonConfirm->setColorNormal(custom->getColor(Customize::colorNormal));
  buttonConfirm->setFont(custom->getFont(Customize::fontSmall));

  //boxName->setValue(custom->getString(Customize::userName));
  boxName->setFont(custom->getFont(Customize::fontSmall));

  boxColorBackground->setValue(this->color2String(custom->getColor(Customize::colorBackground)));
  boxColorBackground->setFont(custom->getFont(Customize::fontSmall));
  // connect(boxColorBackground, SIGNAL(actValueChanged(Customize::colorBackground, QString)), custom, SLOT(setColor(Customize::colorBackground, QString)));
  }
  */

  void CustomizePanel::onCustomize(Data data)
  {
    this->setColorHeader(data.getValue_color(DataKey::COLOR_PANEL_HEADER));
    this->setColorBackground(data.getValue_color(DataKey::COLOR_PANEL_BACKGROUND));
    this->setColorTitle(data.getValue_color(DataKey::COLOR_TEXT));
    this->setFontTitle(data.getValue_font(DataKey::FONT_SMALL));
    this->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    this->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    this->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));

    buttonSave->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));
    buttonSave->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    buttonSave->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    buttonSave->setFont(data.getValue_font(DataKey::FONT_SMALL));

    textApp->setFont(data.getValue_font(DataKey::FONT_BIG));
    textApp->setColorText(data.getValue_color(DataKey::COLOR_TEXT));
    textColor->setFont(data.getValue_font(DataKey::FONT_BIG));
    textColor->setColorText(data.getValue_color(DataKey::COLOR_TEXT));
    textFont->setFont(data.getValue_font(DataKey::FONT_BIG));
    textFont->setColorText(data.getValue_color(DataKey::COLOR_TEXT));

    subTextCanvan->setFont(data.getValue_font(DataKey::FONT_SMALL));
    subTextCanvan->setColorText(data.getValue_color(DataKey::COLOR_TEXT));
    subTextPanels->setFont(data.getValue_font(DataKey::FONT_SMALL));
    subTextPanels->setColorText(data.getValue_color(DataKey::COLOR_TEXT));
    subTextMsg->setFont(data.getValue_font(DataKey::FONT_SMALL));
    subTextMsg->setColorText(data.getValue_color(DataKey::COLOR_TEXT));

    boxName->setValue(data.getValue_string(DataKey::USERNAME));
    boxName->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    boxName->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    boxName->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));
    boxName->setColorText(data.getValue_color(DataKey::COLOR_TEXT));
    boxName->setFont(data.getValue_font(DataKey::FONT_SMALL));

    boxColorNormal->setValue(data.toString(DataKey::COLOR_NORMAL));
    boxColorNormal->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    boxColorNormal->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    boxColorNormal->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));
    boxColorNormal->setColorText(data.getValue_color(DataKey::COLOR_TEXT));
    boxColorNormal->setFont(data.getValue_font(DataKey::FONT_SMALL));

    boxColorOver->setValue(data.toString(DataKey::COLOR_OVER));
    boxColorOver->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    boxColorOver->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    boxColorOver->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));
    boxColorOver->setColorText(data.getValue_color(DataKey::COLOR_TEXT));
    boxColorOver->setFont(data.getValue_font(DataKey::FONT_SMALL));

    boxColorActive->setValue(data.toString(DataKey::COLOR_ACTIVE));
    boxColorActive->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    boxColorActive->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    boxColorActive->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));
    boxColorActive->setColorText(data.getValue_color(DataKey::COLOR_TEXT));
    boxColorActive->setFont(data.getValue_font(DataKey::FONT_SMALL));

    boxColorText->setValue(data.toString(DataKey::COLOR_TEXT));
    boxColorText->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    boxColorText->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    boxColorText->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));
    boxColorText->setColorText(data.getValue_color(DataKey::COLOR_TEXT));
    boxColorText->setFont(data.getValue_font(DataKey::FONT_SMALL));

    boxColorPanelBackground->setValue(data.toString(DataKey::COLOR_PANEL_BACKGROUND));
    boxColorPanelBackground->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    boxColorPanelBackground->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    boxColorPanelBackground->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));
    boxColorPanelBackground->setColorText(data.getValue_color(DataKey::COLOR_TEXT));
    boxColorPanelBackground->setFont(data.getValue_font(DataKey::FONT_SMALL));
  }

  Data CustomizePanel::makeData()
  {
    Data data;
    // bude nahrazeno hodnotami dole
    data.setValue(DataKey::COLOR_APP_HEADER, QColor(40, 40, 40));
    data.setValue(DataKey::COLOR_APP_BACKGROUND, QColor(20, 20, 20));
    data.setValue(DataKey::COLOR_PANEL_HEADER, QColor(30, 30, 30));

    data.setValue(DataKey::COLOR_MSG_NORMAL, QColor(70, 70, 70));
    data.setValue(DataKey::COLOR_MSG_STATUS, QColor(230, 230, 230));
    data.setValue(DataKey::COLOR_MSG_EVALUATE, QColor(170, 230, 230));
    data.setValue(DataKey::COLOR_MSG_ANSWER, QColor(170, 200, 160));
    data.setValue(DataKey::COLOR_MSG_ERROR, QColor(230, 30, 30));
    data.setValue(DataKey::COLOR_MSG_WARNINIG, QColor(230, 130, 30));
    data.setValue(DataKey::COLOR_MSG_BUNDLE, QColor(170, 160, 20));

    data.setValue(DataKey::BOOL_TEXT_ANTIALIASING, true);
    data.setValue(DataKey::FONT_BIG, QFont("Univers Condensed", 13));
    data.setValue(DataKey::FONT_SMALL, QFont("Univers Condensed", 10));
    data.setValue(DataKey::FONT_CODE, QFont("Consolas", 8));
    data.setValue(DataKey::FONT_CONSOLE, QFont("Univers 57 Condensed", 9));

    data.setValue(DataKey::BOOL_BOOT_INTERPRETR, false);
    data.setValue(DataKey::BOOL_BOOT_SERVER, false);
    ////////////////////////////////////////

    data.setValue(DataKey::USERNAME, boxName->getValue_string());
    data.setValue(DataKey::COLOR_NORMAL, this->string2Color(boxColorNormal->getValue_string()));
    data.setValue(DataKey::COLOR_OVER, this->string2Color(boxColorOver->getValue_string()));
    data.setValue(DataKey::COLOR_ACTIVE, this->string2Color(boxColorActive->getValue_string()));
    data.setValue(DataKey::COLOR_TEXT, this->string2Color(boxColorText->getValue_string()));

    data.setValue(DataKey::COLOR_PANEL_BACKGROUND, this->string2Color(boxColorPanelBackground->getValue_string()));
    return data;
  }

  void CustomizePanel::onChangePressed()
  {
    qDebug("CustomizePanel::onChangePressed()");
    emit actChangeConfirmed(this->makeData());
  }
  void CustomizePanel::onSavePressed()
  {
    qDebug("CustomizePanel::onSavePressed()");
    emit actSaveConfirmed(this->makeData());
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

    /////////////////////////////
    textColor->setGeometry(5, 85, width() - 20, 20);
    
    subTextCanvan->setGeometry(20, 115, width() - 30, 20);
    boxColorNormal->setGeometry(20, 145, width() - 30, 20);
    boxColorOver->setGeometry(20, 175, width() - 30, 20);
    boxColorActive->setGeometry(20, 205, width() - 30, 20);
    boxColorText->setGeometry(20, 235, width() - 30, 20);
    
    subTextPanels->setGeometry(20, 275, width() - 30, 20);
    boxColorPanelBackground->setGeometry(20, 305, width() - 30, 20);


    subTextMsg->setGeometry(20, 345, width() - 30, 20);

    /////////////////////////////
    textFont->setGeometry(5, 385, width() - 20, 20);





    scrollWidget->setFixedHeight(410); // end of scroll

    PanelNEW::resizeEvent(event);
  }
  CustomizePanel::~CustomizePanel()
  {

  }
}