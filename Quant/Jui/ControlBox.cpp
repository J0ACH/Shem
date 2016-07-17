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

    backgroundAlpha = 0;
    fadeTimeOut = 800;
    fps = 25;
    timer = new QTimer(this);

    value->installEventFilter(this);

    colorText = Qt::white;
    colorBackground = Qt::black;
    this->updateStyleSheet();

    connect(timer, SIGNAL(timeout()), this, SLOT(alphaUpdate()));
  }

  QRect ControlBox::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

  void ControlBox::setLabel(QString txt) { label->setText(txt); }
  void ControlBox::setValue(QString val)
  {
    value->setText(val);
    oldValue = val;
  }

  double ControlBox::getValue()  { qDebug("ControlBox::getValue() BUDE ODSTANENO -> POUZIT ControlBox::getValue_double()");  return value->displayText().toDouble(); }
  QString ControlBox::getValue_string()  { return value->displayText(); }
  int ControlBox::getValue_int()  { return value->displayText().toInt(); }
  double ControlBox::getValue_double()  { return value->displayText().toDouble(); }

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
    styleTxt.append("QLineEdit { background-color: rgba(0,0,0,0); }");
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
      case Qt::Key::Key_Enter:
      case Qt::Key::Key_Return:
        //qDebug() << "ControlBox ENTER";
        backgroundAlpha = 255;
        timer->stop();
        timer->setInterval(fadeTimeOut / fps);
        timer->start();

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
    QColor fillColor = QColor(255, 30, 30);// activeColor;
    fillColor.setAlpha(backgroundAlpha);
    painter.fillRect(this->bounds(), fillColor);

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

  void ControlBox::alphaUpdate()
  {
    backgroundAlpha -= 255 / fps;
    if (backgroundAlpha <= 0)
    {
      timer->stop();
      backgroundAlpha = 0;
    }
    //qDebug() << tr("buttonAlpha %1").arg(QString::number(backgroundAlpha));
    update();
  }

  ControlBox::~ControlBox()	{ }

}