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
    /*
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
    */

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


  }
  void CustomizePanel::onCustomize(DataCustomize data)
  {
    customizeData = data;

    /*
    foreach(QString oneKey, DataCustomize::keys2())
    {
    qDebug() << "ENUM keys:" << oneKey;
    }

    foreach(QVariant oneValue, customizeData.values())
    {
    qDebug() << "ENUM values:" << oneValue;
    qDebug() << "ENUM type:" << oneValue.typeName();
    //qDebug() << "ENUM value:" << metaEnum.value(i);
    }

    for (int i = 0; i < customizeData.values().size(); i++)
    {
    qDebug() << "CustomizePanel::onCustomize KEYS:" << DataCustomize::allKeys()[i];
    qDebug() << "ENUM values:" << customizeData.values()[i] << "type : " << customizeData.values()[i].typeName();
    //qDebug() << "ENUM keys:" << DataCustomize::allKeys()[i] << "values:" << customizeData.values()[i] << "type : " << customizeData.values()[i].typeName();
    }
    */

    if (firstOnCustomizeFillBoxes)
    {
      boxName->setValue(data.getValue_string(DataCustomize::Key::USERNAME));
    //  boxBootInterpret->setValue(QString::number(data.getValue_bool(DataCustomize::Key::BOOL_BOOT_INTERPRETR)));
    //  boxBootServer->setValue(QString::number(data.getValue_bool(DataCustomize::Key::BOOL_BOOT_SERVER)));

      boxColorNormal->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_NORMAL)));
      boxColorOver->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_OVER)));
      boxColorActive->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_ACTIVE)));
      boxColorText->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_TEXT)));

      boxColorAppHeader->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_APP_HEADER)));
      boxColorAppBackground->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_APP_BACKGROUND)));

      boxColorPanelHeader->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_PANEL_HEADER)));
      boxColorPanelBackground->setValue(this->color2String(data.getValue_color(DataCustomize::Key::COLOR_PANEL_BACKGROUND)));

      firstOnCustomizeFillBoxes = false;
    }
  }

  void CustomizePanel::onChangePressed()
  {
    // qDebug("CustomizePanel::onChangePressed()");

    customizeData.setValue(DataCustomize::Key::USERNAME, boxName->getValue_string()); // PREPIS USERNAME ZPUSOBUJE PAD APP !!!!!!!!!!!!

    customizeData.setValue(DataCustomize::Key::COLOR_NORMAL, this->string2Color(boxColorNormal->getValue_string()));
    customizeData.setValue(DataCustomize::Key::COLOR_OVER, this->string2Color(boxColorOver->getValue_string()));
    customizeData.setValue(DataCustomize::Key::COLOR_ACTIVE, this->string2Color(boxColorActive->getValue_string()));
    customizeData.setValue(DataCustomize::Key::COLOR_TEXT, this->string2Color(boxColorText->getValue_string()));

    customizeData.setValue(DataCustomize::Key::COLOR_APP_HEADER, this->string2Color(boxColorAppHeader->getValue_string()));
    customizeData.setValue(DataCustomize::Key::COLOR_APP_BACKGROUND, this->string2Color(boxColorAppBackground->getValue_string()));

    customizeData.setValue(DataCustomize::Key::COLOR_PANEL_HEADER, this->string2Color(boxColorPanelHeader->getValue_string()));
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
    boxColorAppHeader->setGeometry(20, 265, width() - 30, 20);
    boxColorAppBackground->setGeometry(20, 295, width() - 30, 20);


    subTextPanels->setGeometry(20, 335, width() - 30, 20);
    boxColorPanelBackground->setGeometry(20, 365, width() - 30, 20);
    boxColorPanelHeader->setGeometry(20, 395, width() - 30, 20);


    subTextMsg->setGeometry(20, 445, width() - 30, 20);

    /////////////////////////////
    textFont->setGeometry(5, 485, width() - 20, 20);


    scrollWidget->setFixedHeight(510); // end of scroll

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
  QColor CustomizePanel::string2Color(QString colTxt)
  {
    QStringList txtColors = colTxt.split(",");
    int red = txtColors[0].remove(" ").toInt();
    int green = txtColors[1].remove(" ").toInt();
    int blue = txtColors[2].remove(" ").toInt();

    return QColor(red, green, blue);
  }

  CustomizePanel::~CustomizePanel() { }
}