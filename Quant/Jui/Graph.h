#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QMouseEvent>
#include <QLabel>
#include <QTimer>
#include <QtCore/qmath.h> 

#include "ControlBox.h"

namespace Jui
{
  // GRAPH OBJECT

  class GraphObject : public QObject
  {
    Q_OBJECT

  public:
    GraphObject(QWidget *graph);
    ~GraphObject();

    float valueX, valueY;
    QPointF getValuePt();
    QPoint getPixel();
    QPoint getPixel(QPointF valuePoint);
    void setValue(QPoint pixel);
    void setValueX(float valX);
    void setValueY(float valY);

    float getDomainX_min(), getDomainX_max(), getDomainY_min(), getDomainY_max();

    bool modify;
    int modifyAlpha;

    void draw(QPainter *painter);
    void redraw();

    public slots:
    void onObjectModify();
    void onDomainChanged(QPair<float, float>, QPair<float, float>);
    void onGraphResized(QSize);
    void onModifyTick();

  signals:
    void actModify();

  private:
    QWidget *graph;
    QPointF graphOrigin;
    QSize graphSize;
    QPair<float, float> domainX, domainY;

    QTimer *modifyTime;
    int fadeTimeOut, fps;
  };


  // GRAPH OBJECT END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH VERTEX
  enum VertexType { vertex, startPoint, endPoint };

  class GraphVertex : public GraphObject
  {
    Q_OBJECT

  public:
    GraphVertex(QWidget *graph);
    ~GraphVertex();

    int ID;
    VertexType type;

    void setType(VertexType newType);
    void setSelected(bool);

    void draw(QPainter *painter);

    public slots:
    void onCBoxValXChanged(QString);
    void onCBoxValYChanged(QString);

  signals:
    void actMoved(int ID);
    void actSelected(int ID);
    void actDeleted(int ID);

  protected:
    virtual bool eventFilter(QObject * watched, QEvent * event);

  private:
    bool isOver(QPointF mouse);
    bool isSelcted, pressed;

    ControlBox *cBoxX, *cBoxY;

  };

  // GRAPH VERTEX END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH CURVE

  enum CurveType { step, lin, exp, sin, welch, hold };

  class GraphCurve : public GraphObject
  {
    Q_OBJECT

  public:
    GraphCurve(QWidget *graph, GraphVertex *pt1, GraphVertex *pt2);
    ~GraphCurve();

    int ID;
    CurveType type;

    void setType(CurveType newType);
    void setFrom(GraphVertex *pt);
    void setTo(GraphVertex *pt);
    void flipEnds();

    bool isOverCurveDomain(float valX);
    float at(float valX);

    void draw(QPainter *painter);

    public slots:
    void onObjectModify();
    void onCBoxTypeChanged(QString);

  signals:
    void actTypeChanged(int);

  private:
    GraphVertex *from, *to;

    int pixelWidth;
    QPolygon polygon;

    bool isMouseInDomain;

    ControlBox *cBoxType;
  };

  // GRAPH CURVE END
  ///////////////////////////////////////////////////////////////////////////////////////////////////

  enum AxisType { horizontal, vertical };

  class GraphAxis : public GraphObject
  {
    Q_OBJECT

  public:
    GraphAxis(QWidget *graph, float value, AxisType direction);
    ~GraphAxis();

    void draw(QPainter *painter);
    void setValue(float newValue);

    public slots:
    void onDomainChanged(QPair<float, float>, QPair<float, float>);

  private:
    GraphObject *from, *to;
    AxisType type;
  };

  class GraphMouse : public GraphObject
  {
    Q_OBJECT

  public:
    GraphMouse(QWidget *graph);
    ~GraphMouse();

    float crvVal;

    void draw(QPainter *painter);

  signals:
    void actPositionChanged(QPair<float, float>);
    // void actLeaveGraph();

  protected:
    virtual bool eventFilter(QObject *watched, QEvent * event);

  private:
    GraphAxis *axisX, *axisY;
    GraphObject *curvePt;
  };

  class Graph : public QWidget
  {
    Q_OBJECT

  public:
    Graph(QWidget *parent);
    ~Graph();

    QRect bounds();
    QRect boundsGraph();
    void setDomainX(float min, float max);
    void setDomainY(float min, float max);
    QPair<float, float> getDomainX();
    QPair<float, float> getDomainY();

    void addVertex(QPointF valuePt);

    public slots:
    void onMouseMoved(QPair<float, float>);
    void onVertexSelected(int ID);
    void onVertexMoved(int ID);
    void onVertexDeleted(int ID);
    void onCurveTypeChanged(int ID);
    void onDurationTimerTick();

    void onEnvChanged(QList<double> levels, QList<double> times, QList<QString> curves);

  signals:
    void actEnvGraphChanged(QList<double> levels, QList<double> times, QList<QString> curves);
    void actDomainChanged(QPair<float, float>, QPair<float, float>);
    void actResized(QSize);

  protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

  private:
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);

    void makeEnv();
    void sortVertexByX();

    int frameOffset;
    QPair<float, float> domainX, domainY;
    int numGraphAxisX, numGraphAxisY;

    QPainter *painterGraphObject;
    QList<GraphVertex*> controlVertexs;
    QList<GraphCurve*> controlCurves;
    QList<GraphAxis*> graphVerticalAxis;
    QList<GraphAxis*> graphHorizontalAxis;
    GraphMouse *graphMouse;

    QTimer *durationTimer, *startTimer;
    float currentTime, beatTime, currentTime_step;
    GraphAxis *timeAxis;

  };


}



#endif // GRAPH_H

