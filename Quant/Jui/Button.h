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
      Q_PROPERTY(QColor colorNormal READ getColorNormal WRITE setColorNormal)
      Q_PROPERTY(QColor colorOver READ getColorOver WRITE setColorOver)
      Q_PROPERTY(QColor colorActive READ getColorActive WRITE setColorActive)
      Q_PROPERTY(QFont font READ getFont WRITE setFont)
      
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
    void setButtonGroup(QList<Button *> other);

    void setColorNormal(QColor);
    void setColorOver(QColor);
    void setColorActive(QColor);
    void setFont(QFont);

    QColor getColorNormal();
    QColor getColorOver();
    QColor getColorActive();
    QFont getFont();

    State getState();


  signals:
    void pressAct(); // bude odstraneno
    void actPressed();
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

    QString text;
    QFont font;
    QImage icon;
    QColor colorNormal, colorOver, colorActive;
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

    //QColor customColor, customColor2;


  };

}
#endif // BUTTON_H

