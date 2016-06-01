#include "ControlBox.h"

namespace Jui
{

  ControlBox::ControlBox(QWidget *parent) :
    QWidget(parent)
  {

    txt = new QLabel(this);
    txt->setText("label: ");

    val = new QLineEdit(this);
    //text->setReadOnly(true);
    //text->setOverwriteMode(false);
    //text->setTabStopWidth(30);

    // text->setFrameStyle(QFrame::NoFrame);

    // text->append(tr("ControlBox init..."));

    QTextCharFormat format;
    format.setForeground(QBrush(QColor(230, 30, 30)));
    //  text->setCurrentCharFormat(format);
  }

  QRect ControlBox::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

  void ControlBox::setFont(QFont font)
  {
    txt->setFont(font);
    val->setFont(font);
  }

  void ControlBox::setColorBackground(QColor color)
  {
    colorBackground = color;
    this->updateStyleSheet();
  }

  void ControlBox::setColorText(QColor color)
  {
    colorText = color;
    this->updateStyleSheet();
  }

  void ControlBox::updateStyleSheet()
  {
    QString styleTxt;
    styleTxt.append(tr("QLabel { color: %1; }").arg(colorText.name()));
    styleTxt.append(tr("QLineEdit { color: %1; }").arg(colorText.name()));
    styleTxt.append(tr("QLineEdit { background-color: %1; }").arg(colorBackground.name()));
    styleTxt.append("QLineEdit { border: none;}");

    txt->setStyleSheet(styleTxt);
    val->setStyleSheet(styleTxt);
  }

  void ControlBox::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.fillRect(this->bounds(), colorBackground);

    painter.setPen(QColor(255, 255, 255));
    painter.drawRect(this->bounds());
  }

  void ControlBox::resizeEvent(QResizeEvent *event)
  {
    //Panel::resizeEvent(event); // send event to superclass
    txt->setGeometry(10, 0, this->width() / 2 - 10, this->height());
    val->setGeometry(this->width() / 2, 1, this->width() / 2 - 1, this->height() - 2);
  }

  ControlBox::~ControlBox()	{ }

}