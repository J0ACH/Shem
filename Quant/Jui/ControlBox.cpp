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
    isFocused = false;
    //text->setTabStopWidth(30);

    backgroundAlpha = 0;
    fadeTimeOut = 800;
    fps = 25;
    timer = new QTimer(this);

    label->installEventFilter(this);
    value->installEventFilter(this);

    colorText = Qt::white;
    colorBackground = Qt::black;
    colorNormal = Qt::gray;
    colorOver = Qt::white;
    colorActive = QColor(255, 30, 30);
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
  void ControlBox::setColorNormal(QColor color){ colorNormal = color; }
  void ControlBox::setColorOver(QColor color){ colorOver = color; }
  void ControlBox::setColorActive(QColor color){ colorActive = color; }
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
    update();
      }

  bool ControlBox::eventFilter(QObject* target, QEvent* event)
  {
    //qDebug() << "ControlBox target " << target << "event" << event;
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
        value->setFocus();
        //value->clearFocus();
        //qDebug() << "OldValue" << oldValue;
        isFocused = true;
        this->update();
        return true;
      case Qt::Key::Key_Escape:
        //qDebug() << "ControlBox Esc";
        //qDebug() << "OldValue" << oldValue;
        value->setText(oldValue);
        value->clearFocus();
        isFocused = false;
        this->update();
        return true;
      }
    };

    if (event->type() == QEvent::MouseButtonRelease)
    {
      // qDebug("ControlBox Pressed");
      isFocused = true;
      value->setFocus();
      this->update();
    }
    if (event->type() == QEvent::FocusOut)
    {
      // qDebug("ControlBox FocusOut");
      isFocused = false;
      this->update();
    }
    return QWidget::eventFilter(target, event);
  }

  void ControlBox::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    QColor fillColor = colorActive;
    fillColor.setAlpha(backgroundAlpha);
    painter.fillRect(this->bounds(), fillColor);

    painter.setPen(colorText);
    if (isFocused) { painter.setPen(colorOver); }
    else { painter.setPen(colorNormal); }

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