#include "CustomizePanel.h"

namespace QuantIDE
{
  CustomziePanel::CustomziePanel(QWidget *parent) : PanelNEW(parent)
  {
    this->initControl();
  }

  void CustomziePanel::initControl()
  {
    buttonConfirm = new Button(this);
    buttonConfirm->setText("Confirm");

    boxName = new ControlBox(this);
    boxName->setLabel("userName");
    boxName->setLabelSize(150);

    boxColorBackground = new ControlBox(this);
    boxColorBackground->setLabel("background");
    boxColorBackground->setLabelSize(150);
  }

  QString CustomziePanel::color2String(QColor color)
  {
    return tr("%1, %2, %3").arg(
      QString::number(color.red()),
      QString::number(color.green()),
      QString::number(color.blue())
      );
  }
  QColor CustomziePanel::string2Color(QString colTxt)
  {
    return QColor(colTxt);
  }

  void CustomziePanel::onCustomize(Customize* custom)
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

  void CustomziePanel::onData(Data data)
  {
    //qDebug() << "CustomziePanel::onData ->" << data.getValue_string(DataKey::USERNAME);


    this->setColorHeader(data.getValue_color(DataKey::COLOR_PANEL_HEADER));
    this->setColorBackground(data.getValue_color(DataKey::COLOR_PANEL_BACKGROUND));
    this->setColorTitle(data.getValue_color(DataKey::COLOR_TEXT));
    this->setFontTitle(data.getValue_font(DataKey::FONT_SMALL));

    buttonConfirm->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));
    buttonConfirm->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    buttonConfirm->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    buttonConfirm->setFont(data.getValue_font(DataKey::FONT_SMALL));

    
    boxName->setValue(data.getValue_string(DataKey::USERNAME));
    boxName->setFont(data.getValue_font(DataKey::FONT_SMALL));

    boxColorBackground->setValue(data.toString(DataKey::COLOR_PANEL_BACKGROUND));
    boxColorBackground->setFont(data.getValue_font(DataKey::FONT_SMALL));

  }

  void CustomziePanel::resizeEvent(QResizeEvent *event)
  {
    buttonConfirm->setGeometry(width() - 100, height() - 50, 80, 20);

    boxName->setGeometry(10, 50, width() - 20, 20);
    boxColorBackground->setGeometry(10, 80, width() - 20, 20);

    PanelNEW::resizeEvent(event);
  }
  CustomziePanel::~CustomziePanel()
  {

  }
}