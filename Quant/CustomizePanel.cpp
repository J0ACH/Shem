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
    scrollWidget->setGeometry(0, 0, 300, 100);

    scrollArea->setWidget(scrollWidget);

    buttonSave = new Button(this);
    buttonSave->setText("Save");
    connect(buttonSave, SIGNAL(pressAct()), this, SLOT(onSavePressed()));

    boxName = new ControlBox(scrollWidget);
    boxName->setLabel("userName");
    boxName->setLabelSize(150);
    connect(boxName, SIGNAL(actValueChanged(QString)), this, SLOT(onChangePressed()));

    boxColorBackground = new ControlBox(scrollWidget);
    boxColorBackground->setLabel("background");
    boxColorBackground->setLabelSize(150);
    connect(boxColorBackground, SIGNAL(actValueChanged(QString)), this, SLOT(onChangePressed()));
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
    //qDebug() << "CustomziePanel::onData ->" << data.getValue_string(DataKey::USERNAME);

    this->setColorHeader(data.getValue_color(DataKey::COLOR_PANEL_HEADER));
    this->setColorBackground(data.getValue_color(DataKey::COLOR_PANEL_BACKGROUND));
    this->setColorTitle(data.getValue_color(DataKey::COLOR_TEXT));
    this->setFontTitle(data.getValue_font(DataKey::FONT_SMALL));

    buttonSave->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));
    buttonSave->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    buttonSave->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    buttonSave->setFont(data.getValue_font(DataKey::FONT_SMALL));

    boxName->setValue(data.getValue_string(DataKey::USERNAME));
    boxName->setFont(data.getValue_font(DataKey::FONT_SMALL));

    boxColorBackground->setValue(data.toString(DataKey::COLOR_PANEL_BACKGROUND));
    boxColorBackground->setFont(data.getValue_font(DataKey::FONT_SMALL));
  }

  Data CustomizePanel::makeData()
  {
    Data data;
    // bude nahrazeno hodnotami dole
    data.setValue(DataKey::COLOR_APP_HEADER, QColor(40, 40, 40));
    data.setValue(DataKey::COLOR_APP_BACKGROUND, QColor(20, 20, 20));
    data.setValue(DataKey::COLOR_PANEL_HEADER, QColor(30, 30, 30));

    data.setValue(DataKey::COLOR_NORMAL, QColor(120, 120, 120));
    data.setValue(DataKey::COLOR_OVER, QColor(255, 255, 255));
    data.setValue(DataKey::COLOR_ACTIVE, QColor(70, 140, 210));
    data.setValue(DataKey::COLOR_TEXT, QColor(230, 230, 230));

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
    data.setValue(DataKey::COLOR_PANEL_BACKGROUND, this->string2Color(boxColorBackground->getValue_string()));
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

    boxName->setGeometry(10, 10, width() - 20, 20);
    boxColorBackground->setGeometry(10, 50, width() - 20, 20);

    PanelNEW::resizeEvent(event);
  }
  CustomizePanel::~CustomizePanel()
  {

  }
}