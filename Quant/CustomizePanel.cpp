#include "CustomizePanel.h"

namespace QuantIDE
{
  CustomizePanel::CustomizePanel(QWidget *parent) : PanelNEW(parent)
  {
    this->setMinimumWidth(400);
    this->setMinimumHeight(150);
    this->initControl();

    firstOnCustomizeFillBoxes = true;
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

    boxColorPanelBackground = new ControlBox(scrollWidget);
    boxColorPanelBackground->setLabel("background");
    boxColorPanelBackground->setLabelSize(150);
    connect(boxColorPanelBackground, SIGNAL(actValueEvaluate(QString)), this, SLOT(onChangePressed()));
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

  void CustomizePanel::onCustomize(DataCustomize data)
  {
    customizeData = data;

    if (firstOnCustomizeFillBoxes)
    {
      boxName->setValue(data.getValue_string(DataCustomize::Key::USERNAME));

      boxColorNormal->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_NORMAL)));
      boxColorOver->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_OVER)));
      boxColorActive->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_ACTIVE)));
      boxColorText->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_TEXT)));

      boxColorPanelBackground->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_PANEL_BACKGROUND)));

      firstOnCustomizeFillBoxes = false;
    }
  }

  void CustomizePanel::onChangePressed()
  {
    // qDebug("CustomizePanel::onChangePressed()");

    customizeData.setValue(DataCustomize::Key::USERNAME, boxName->getValue_string());

    customizeData.setValue(DataCustomize::Key::COLOR_PANEL_BACKGROUND, this->string2Color(boxColorPanelBackground->getValue_string()));

    qDebug() << customizeData.print("CustomizePanel::onChangePressed");
    emit actChangeConfirmed(customizeData);
  }
  void CustomizePanel::onSavePressed()
  {
    // qDebug("CustomizePanel::onSavePressed()");
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
  CustomizePanel::~CustomizePanel() { }
}