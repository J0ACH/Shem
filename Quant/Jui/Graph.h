#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QLabel>
#include <QTimer>

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
    QPoint getPixel();
    void setValue(QPoint pixel);

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

  class GraphVertex : public GraphObject
  {
  public:
    GraphVertex(QWidget *graph);
    ~GraphVertex();

    void draw(QPainter *painter);

  protected:
    virtual bool eventFilter(QObject * watched, QEvent * event);

  private:
    bool isOver(QPointF mouse);
    bool focus, pressed;
  };

  // GRAPH VERTEX END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH CURVE

  class GraphCurve : public GraphObject
  {
  public:
    GraphCurve(QWidget *graph, GraphVertex *pt1, GraphVertex *pt2);
    ~GraphCurve();

    void draw(QPainter *painter);

  private:
    GraphVertex *from, *to;
    QString curvature;
  };

  // GRAPH CURVE END
  ///////////////////////////////////////////////////////////////////////////////////////////////////

  /*
  class GraphPoint : public QWidget
  {
    Q_OBJECT

  public:
    GraphPoint(QWidget *parent, int ptID, int pX, int pY, double valX, double valY);
    ~GraphPoint();

    QRect bounds();
    int ID;
    int pixelX, pixelY;
    double valueX, valueY;
    int pointSize;
    QString curvature;

    enum PointType { vertex, startPoint, endPoint, curvePoint };
    PointType type;

    void setID(int newID);
    void setX(int pX, double valX);
    void setY(int pY, double valY);
    void setType(PointType newType);
    void setCurvature(QString txt);

  signals:
    void actDelete(int ID);
    void actMoved(int ID, int pixelX, int pixelY);

  protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);
    void closeEvent(QCloseEvent *);
    void focusInEvent(QFocusEvent*);
    void focusOutEvent(QFocusEvent*);
    void moveEvent(QMoveEvent * event);

    virtual bool eventFilter(QObject * watched, QEvent * event);

  private:
    QPoint mousePressCoor;
    QPoint mouseGlobalCoor;

    QLabel *labelID;
    QLabel *labelLevel, *labelTime, *labelCurve;
  };
  */

  // GRAPH POINT END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH CURVE

  /*
  class GraphCurve : public QWidget
  {
  Q_OBJECT

  public:
  GraphCurve(QWidget *parent, GraphPoint *ptFrom, GraphPoint *ptTo);
  ~GraphCurve();

  QRect bounds();

  void setFrom(GraphPoint *ptFrom);

  public slots:
  void onFromMoved(int ID, int pixelX, int pixelY);
  //void onEndMoved(int ID, int pixelX, int pixelY);

  protected:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);
  void mousePressEvent(QMouseEvent *);

  private:
  GraphPoint *from, *to;
  };
  */

  // GRAPH CURVE END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH 


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
    QList<double> getDomainX();
    QList<double> getDomainY();

   // GraphPoint *addStartPoint(QPointF newPt);
   // GraphPoint *addVertexPoint(QPointF newPt);
   // GraphPoint *addEndPoint(QPointF newPt);
    void setVertexPoint(int ID, QPointF newPt);
   // void setVertexType(int ID, GraphPoint::PointType newType);
   // GraphPoint *getVertex(int ID);

   // GraphPoint *addCurvePoint(QPointF newPt);
    void setCurvePoint(int ID, QPointF newPt);
    void setCurveCurvature(int ID, QString txt);

    void drawPoint(double valueX, double valueY);
    void drawLine(double valueX1, double valueY1, double valueX2, double valueY2);
    void drawPolyline(QVector<QPointF> collPoints);
    void addPolyline(QVector<QPointF> collPoints);

    void deleteGraph();

    public slots:
    void onDeletePoint(int ID);
    void onMovePoint(int ID, int pixelX, int pixelY);

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

   // GraphPoint *addValuePoint(double valueX, double valueY, GraphPoint::PointType type);

    void makeEnv();
    void sortPointsByX();

    int frameOffset;
    QPair<float, float> domainX, domainY;

    QPainter *painterGraphObject;
 //   GraphObject *testObj;
    GraphVertex *testVertex1, *testVertex2, *testVertex3, *testVertex4;
    GraphCurve *testCurve1, *testCurve2, *testCurve3;
    QList<GraphVertex*> controlVertexs;
    QList<GraphCurve*> controlCurves;

   // QList<GraphPoint*> controlPts;
  //  QList<GraphPoint*> curvePts;
    // QList<GraphCurve*> curves;

    QList<QPointF*> collDrawPoints;
    QList<QLineF*> collDrawLines;
    QPolygonF *graphPolylines;
    QList<QPolygonF*> collPolylinesNEW;

    float getValueX(int displayX);
    float getValueY(int displayY);
    float getPixelX(float valueX);
    float getPixelY(float valueY);

  };


}



#endif // GRAPH_H

