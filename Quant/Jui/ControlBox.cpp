#include "ControlBox.h"

namespace Jui
{

  ControlBox::ControlBox(QWidget *parent) :
    QWidget(parent)
  {
    label = new QLabel(this);
    label->setText("label");

    value = new QLineEdit(this);
    value->setText("NaN");

    labelSizeX = 30;
    //text->setTabStopWidth(30);

    value->installEventFilter(this);
  }

  QRect ControlBox::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

  void ControlBox::setFont(QFont font)
  {
    label->setFont(font);
    value->setFont(font);
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
  void ControlBox::setLabelSize(int size)
  {
    labelSizeX = size;
    //  QWidget::resizeEvent;
    // label->setGeometry(0, 0, labelSizeX, this->height());
    //  value->setGeometry(labelSizeX, 1, this->width() - labelSizeX - 6, this->height() - 2);    
  }
  void ControlBox::updateStyleSheet()
  {
    QString styleTxt;
    styleTxt.append(tr("QLabel { color: %1; }").arg(colorText.name()));
    styleTxt.append(tr("QLineEdit { color: %1; }").arg(colorText.name()));
    styleTxt.append(tr("QLineEdit { background-color: %1; }").arg(colorBackground.name()));
    styleTxt.append("QLineEdit { border: none;}");

    label->setStyleSheet(styleTxt);
    value->setStyleSheet(styleTxt);
  }

  bool ControlBox::eventFilter(QObject* target, QEvent* event)
  {
    if (event->type() == QEvent::KeyPress)
    {
      QKeyEvent* eventKey = static_cast<QKeyEvent*>(event);
      quint32 modifers = eventKey->nativeModifiers();

      //qDebug() << eventKey->key();

      switch (eventKey->key())
      {
      case Qt::Key::Key_Return:
        //qDebug() << "ControlBox ENTER";
        oldValue = value->displayText();
        emit actValueChanged(value->displayText());
        value->clearFocus();
        //qDebug() << "OldValue" << oldValue;

        return true;
      case Qt::Key::Key_Escape:
        //qDebug() << "ControlBox Esc";
        //qDebug() << "OldValue" << oldValue;
        value->setText(oldValue);
        value->clearFocus();
        //this->parentWidget()::focusInEvent;
        return true;
      }
    }
    return QWidget::eventFilter(target, event);
  }
  /*
  void ControlBox::focusInEvent(QFocusEvent *event)
  {
    qDebug() << "ControlBox Focus IN";
    //value->setFocus(:: focusInEvent(event);
    //value->parentWidget().focusNextChild();
  }
  */
  /*
  void ControlBox::mousePressEvent(QMouseEvent *event)
  {
    qDebug() << "ControlBox CLICK";
    //value->parentWidget().focusNextChild();
  }
  */

  void ControlBox::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.fillRect(this->bounds(), colorBackground);

    painter.setPen(colorText);
    if (this->hasFocus()) { painter.setPen(QColor(120, 20, 20)); }

    painter.drawLine(QLine(0, 0, labelSizeX - 1, 0));
    painter.drawLine(QLine(0, this->height() - 1, labelSizeX - 1, this->height() - 1));

    painter.drawLine(QLine(labelSizeX + 5, 0, this->width() - 1, 0));

    painter.drawLine(QLine(
      labelSizeX + 5,
      this->height() - 1,
      this->width() - 1,
      this->height() - 1)
      );
    //painter.drawRect(this->bounds());
  }

  void ControlBox::resizeEvent(QResizeEvent *event)
  {
    //Panel::resizeEvent(event); // send event to superclass
    label->setGeometry(0, 0, labelSizeX, this->height());
    value->setGeometry(labelSizeX + 5, 1, this->width() - labelSizeX - 2, this->height() - 2);
  }

  ControlBox::~ControlBox()	{ }

}