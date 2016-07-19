#ifndef BUTTON_H
#define BUTTON_H

#include <QWidget>
#include <QPainter>
#include <QFrame>
#include <QTimer>
#include <QDebug>

#include <QMouseEvent>

namespace Jui
{

  class Button : public QWidget
  {
    Q_OBJECT

  public:
    Button(QWidget *parent = 0);
    ~Button();

    enum State { OFF, ON };
    enum StateDisplay { NORMAL, OVER, PRESSED };
    enum StateKeeping { TOUCH, HOLD, SWITCH };

    QRect bounds();

    void setState(State);
    void setStateKeeping(StateKeeping);
    void setIcon(QImage, int);
    void setText(QString name);
    void setFont(QFont);
    void setColorNormal(QColor);
    void setColorOver(QColor);
    void setColorActive(QColor);
    void setButtonGroup(QList<Button *> other);

  signals:
    void pressAct();
    void actPressed(Button::State);
    //void enterAct(QString name);
    //void leaveAct(QString name);

    public slots:
    void fadeRatio();
    void onSwitch();
    void onSwitchOFF(); // bude odstraneno

  protected:
    void paintEvent(QPaintEvent *);

  private:
    State buttonState;
    StateDisplay buttonDisplay;
    StateKeeping buttonKeeping;

    bool isPressed, isOver;

    QString name;
    QFont fontText;
    QImage icon;
    QColor normalColor, overColor, activeColor;
    QColor blendColor(QColor, QColor, qreal);
    QColor penColor, fillColor;
    int iconOffset;

    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    //void addAction();
    int fadeTimeIn;
    int fadeTimeOut;
    int fps;
    //int backgroundAlpha;
    double ratio;

    QTimer *timer;




  };

}
#endif // BUTTON_H

