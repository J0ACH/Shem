#include "Button.h"

#define DISPLAY_FRAMES 0

namespace Jui
{
  Button::Button(QWidget *parent) : QWidget(parent)
  {
    buttonState = OFF;
    buttonDisplay = NORMAL;
    buttonKeeping = TOUCH;

    text = "button";
    isPressed = false;
    isOver = false;

    colorNormal = QColor(120, 120, 120);
    colorOver = QColor(230, 230, 230);
    colorActive = QColor(255, 60, 60);

    iconOffset = 0;

    ratio = 0;
    //backgroundAlpha = 0;
    fadeTimeIn = 150;
    fadeTimeOut = 800;
    fps = 25;

    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(fadeRatio()));
  }

  QRect Button::bounds() { return QRect(1, 1, width() - 2, height() - 2); }

  void Button::setText(QString buttonName) { text = buttonName; }
  void Button::setFont(QFont font) { fontText = font;	update(); }
  void Button::setIcon(QImage img, int offset = 0) { icon = img; iconOffset = offset; }
  void Button::setState(State newState) { buttonState = newState; update(); }
  void Button::setStateKeeping(StateKeeping mode) { buttonKeeping = mode; }
  void Button::setButtonGroup(QList<Button*> others)
  {
    foreach(Button *oneB, others)
    {
      if (oneB != this)
      {
        connect(oneB, SIGNAL(pressAct()), this, SLOT(onSwitchOFF()));
      }
    }
  }

  void Button::setColorNormal(QColor color){ colorNormal = color; penColor = colorNormal; update(); }
  void Button::setColorOver(QColor color){ colorOver = color; update(); }
  void Button::setColorActive(QColor color){ colorActive = color; update(); }

  QColor Button::getColorNormal()  { return colorNormal; }
  QColor Button::getColorOver()  { return colorOver; }
  QColor Button::getColorActive()  { return colorActive; }

  // bude odstraneno
  void Button::onSwitchOFF()
  {
    if (buttonKeeping == SWITCH)
    {
      buttonState = OFF;
      this->update();
    }
  }

  void Button::onSwitch()
  {
    if (buttonKeeping == Button::StateKeeping::HOLD)
    {
      if (buttonState = OFF) { buttonState = ON; }
      else { buttonState = OFF; }
      this->update();
    }
  }

  void Button::fadeRatio()
  {
    if (isOver)
    {
      ratio += 1.0 / fps;
      if (ratio >= 1)
      {
        timer->stop();
        ratio = 1;
      }
    }
    else
    {
      ratio -= 1.0 / fps;
      if (ratio <= 0)
      {
        timer->stop();
        ratio = 0;
      }
    }
    //qDebug() << tr("buttonAlpha %1").arg(QString::number(ratio));
    update();
  }

  QColor Button::blendColor(QColor color1, QColor color2, double ratio)
  {
    int r = color1.red()*(1 - ratio) + color2.red()*ratio;
    int g = color1.green()*(1 - ratio) + color2.green()*ratio;
    int b = color1.blue()*(1 - ratio) + color2.blue()*ratio;

    return QColor(r, g, b, 255);
  }

  void Button::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setFont(fontText);

    penColor = blendColor(colorNormal, colorOver, ratio);
    painter.setPen(QPen(penColor, 1));

    if (icon.isNull())
    {
      switch (buttonState)
      {
      case ON:
        fillColor = colorActive;
        break;
      case OFF:
        fillColor = Qt::transparent;;
        break;
      }
      painter.fillRect(bounds(), fillColor);

      QTextOption opt;
      opt.setAlignment(Qt::AlignCenter);
      painter.drawText(bounds(), text, opt);
      painter.drawRect(bounds());
    }
    else
    {
      if (buttonState == ON) { penColor = colorActive; };
      float moveX = (this->width() - icon.width()) / 2;
      float moveY = (this->height() - icon.height()) / 2;

      QRectF target(moveX, moveY, icon.width(), icon.height());
      QRectF source(0, 0, icon.width(), icon.height());

      QImage renderedIcon(icon);
      renderedIcon.fill(penColor);
      renderedIcon.setAlphaChannel(icon);
      painter.drawImage(target, renderedIcon, source);  // draw image to QWidget
    };

#if DISPLAY_FRAMES
    painter.setPen(Qt::red);
    painter.drawRect(bounds());
#endif
  }

  void Button::mousePressEvent(QMouseEvent *mouseEvent)
  {
    isPressed = true;

    switch (buttonState)
    {
    case OFF: buttonState = ON; /*qDebug("Button STATE(ON)");*/ break;
    case ON: buttonState = OFF;	/*qDebug("Button STATE(OFF)");*/ break;
    };

    buttonDisplay = PRESSED; /*qDebug("Button Display(PRESSED)");*/

    update();

    float time = 1.5;
    int frames = 10;
    float stepTime = time / frames;
    float stemAdd = 1 / frames;

    emit pressAct(); // bude odsraneno
    emit actPressed();

    mouseEvent->accept();
  }

  void Button::mouseReleaseEvent(QMouseEvent *mouseEvent)
  {
    isPressed = false;

    if (buttonKeeping == TOUCH)
    {
      //qDebug("Button Keeping(TOUCH)");
      switch (buttonState)
      {
      case ON: buttonState = OFF;	/*qDebug("Button STATE(OFF)");*/ break;
      case OFF: buttonState = ON;/* qDebug("Button STATE(ON)");*/ break;
      };
    }

    switch (buttonState)
    {
    case ON: buttonDisplay = PRESSED; /*qDebug("Button Display(PRESSED)");*/ break;
    case OFF: buttonDisplay = OVER; /*qDebug("Button Display(OVER)");*/ break;
    }

    update();
    mouseEvent->accept();
  }

  void Button::enterEvent(QEvent *event)
  {
    timer->stop();
    timer->setInterval(fadeTimeIn / fps);
    timer->start();

    isOver = true;
    buttonDisplay = OVER;
    //qDebug("Button Display(OVER)");
    //qDebug() << tr("Button::enterEvent");
  }

  void Button::leaveEvent(QEvent *event)
  {
    timer->stop();
    timer->setInterval(fadeTimeOut / fps);
    timer->start();

    isOver = false;
    switch (buttonState)
    {
    case OFF: buttonDisplay = NORMAL; break;
    case ON: buttonDisplay = PRESSED; break;
    }

    //qDebug("Button Display(NORMAL)");
    //qDebug() << tr("Button (%1, icon: %2)::leaveEvent");
  }

  Button::~Button() {	}
  }