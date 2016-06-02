#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QLabel>

namespace Jui
{
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


  // GRAPH POINT END
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
    void setDomainX(double min, double max);
    void setDomainY(double min, double max);
    QList<double> getDomainX();
    QList<double> getDomainY();

    GraphPoint *addVertexPoint(QPointF newPt);
    void setVertexPoint(int ID, QPointF newPt);
    void setVertexType(int ID, GraphPoint::PointType newType);
    GraphPoint *getVertex(int ID);

    GraphPoint *addCurvePoint(QPointF newPt);
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

  protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

  private:
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);

    GraphPoint *addValuePoint(double valueX, double valueY, GraphPoint::PointType type);

    void makeEnv();
    void sortPointsByX();

    int frameOffset;
    double minDomainX, maxDomainX, minDomainY, maxDomainY;

    QList<GraphPoint*> controlPts;
    QList<GraphPoint*> curvePts;

    QList<QPointF*> collDrawPoints;
    QList<QLineF*> collDrawLines;
    QPolygonF *graphPolylines;
    QList<QPolygonF*> collPolylinesNEW;

    double getValueX(int displayX);
    double getValueY(int displayY);
    double getPixelX(double valueX);
    double getPixelY(double valueY);

  };

}
#endif // GRAPH_H

