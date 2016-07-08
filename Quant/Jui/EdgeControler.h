#ifndef EDGECONTROLER_H
#define EDGECONTROLER_H


#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QList>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

namespace Jui
{


  class EdgeControler : public QWidget
  {
    Q_OBJECT

  public:
    enum Direction
    {
      LEFT,
      TOP,
      RIGHT,
      BOTTOM
    };

    EdgeControler(QWidget *parent = 0);
    ~EdgeControler();

    QRect bounds();
    bool isActive;

    void setDirection(Direction);
    void setEdgeOffset(int);
    void setParentObject(QObject*);

  signals:
    void pressAct();
    void moveAct(EdgeControler::Direction, int);

    public slots:
    void alphaUpdate();
    void fitGeometry();

  protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

  private:
    QObject *conteiner;
    Direction side;
    bool isOver;
    int edgeOffset;

    int fadeTimeIn;
    int fadeTimeOut;
    int fps;
    int backgroundAlpha;

    QTimer *timer;

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

  };
}

#endif // EDGECONTROLER_H



