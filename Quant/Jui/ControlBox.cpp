#include "ControlBox.h"

namespace Jui
{

  ControlBox::ControlBox(QWidget *parent) : QWidget(parent)
  {
    label = new Text(this);
    label->setText("label");

    value = new QLineEdit(this);
    value->setText("NaN");
    oldValue = "NaN";

    labelSizeX = 30;
    //isFocused = false;

    backgroundAlpha = 0;
    fadeTimeOut = 800;
    fps = 25;
    timer = new QTimer(this);

    label->installEventFilter(this);
    value->installEventFilter(this);

    colorText = Qt::white;
    colorNormal = Qt::gray;
    colorOver = Qt::white;
    colorActive = QColor(120, 30, 30);
    colorText = QColor(255, 30, 30);

    connect(timer, SIGNAL(timeout()), this, SLOT(alphaUpdate()));
  }

  QRect ControlBox::bounds() { return QRect(0, 0, width() - 1, height() - 1); }
  QPoint ControlBox::origin() {
    //qDebug() << "ControlBox::origin()" << this->geometry().topLeft();
    return this->geometry().topLeft();
  }

  bool ControlBox::hasFocus() {
    //qDebug() << "ControlBox::hasFocus()" << value->hasFocus();
    return value->hasFocus();
  }

  void ControlBox::setLabel(QString txt) { label->setText(txt); }
  void ControlBox::setValue(QString val)
  {
    value->setText(val);
    oldValue = val;
  }

  QString ControlBox::getValue_string() { return value->displayText(); }
  int ControlBox::getValue_int() { return value->displayText().toInt(); }
  double ControlBox::getValue_double()  { return value->displayText().toDouble(); }
  bool ControlBox::getValue_bool()
  {
    if (value->displayText() == "true") { return true; }
    else { return false; }
  };

  void ControlBox::setColorNormal(QColor color) { colorNormal = color; }
  void ControlBox::setColorOver(QColor color) { colorOver = color; }
  void ControlBox::setColorActive(QColor color) { colorActive = color; }
  void ControlBox::setColorText(QColor color) { colorText = color; }
  void ControlBox::setFont(QFont f){
    font = f;
    label->setFont(font);
    value->setFont(font);
  }

  QColor ControlBox::getColorNormal()  { return colorNormal; }
  QColor ControlBox::getColorOver()  { return colorOver; }
  QColor ControlBox::getColorActive()  { return colorActive; }
  QColor ControlBox::getColorText()  { return colorActive; }
  QFont ControlBox::getFont()  { return font; }

  void ControlBox::setLabelSize(int size)  { labelSizeX = size; }

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
        emit actValueEvaluate(value->displayText());
        value->setFocus();
        this->update();
        return true;
      case Qt::Key::Key_Escape:
        qDebug() << "ControlBox Esc";
        this->parentWidget()->setFocus();
        //        this->update();
        return true;
        break;
      case Qt::Key::Key_Up:
        //  qDebug() << "ControlBox::eventFilter UP on " << target->objectName();
        //  qDebug() << "ControlBox::eventFilter children " << target->children();
        //QWidget parentWidget = static_cast<QWidget> (target);
        //this->previousInFocusChain()->setFocus();
        /*
        if (!library.at(0)->hasFocus()) { this->focusPreviousChild(); }
        else { library.at(library.size() - 1)->setFocus(); }
        */
        break;
      case Qt::Key::Key_Down:
        //   qDebug() << "ControlBox::eventFilter DOWN on " << target->objectName();
        //   qDebug() << "ControlBox::eventFilter children " << target->children();
        //this->nextInFocusChain()->setFocus();
        /*
        if (watched->children().size() > 0)
        {
        firstChildren = static_cast<QWidget*>(watched->children().at(0));
        firstChildren->setFocus();
        //          firstChildren->update();
        }
        */
        break;
      }

    };
    if (event->type() == QEvent::KeyRelease)
    {
      emit actValueChanged(value->displayText());
    }

    if (event->type() == QEvent::MouseButtonRelease)
    {
      // qDebug("ControlBox Pressed");
      value->setFocus();
      this->update();
    }
    if (event->type() == QEvent::FocusOut)
    {
      //qDebug("ControlBox FocusOut");
      value->setText(oldValue);
      emit actValueChanged(value->displayText());
      this->update();
    }
    return QWidget::eventFilter(target, event);
  }

  void ControlBox::focusInEvent(QFocusEvent * event)
  {
    //qDebug("ControlBox focusInEvent");
    value->setFocus();
    value->update();
  }

  void ControlBox::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    QColor fillColor = colorActive;
    fillColor.setAlpha(backgroundAlpha);
    painter.fillRect(this->bounds(), fillColor);

    painter.setPen(colorText);
    /*
    if (isFocused) { painter.setPen(colorOver); }
    else { painter.setPen(colorNormal); }
    */

    if (value->hasFocus()) { painter.setPen(QPen(QColor(220, 30, 30), 3)); }
    else { painter.setPen(QPen(QColor(120, 30, 30), 1)); }

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