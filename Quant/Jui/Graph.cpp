#include "Graph.h"

namespace Jui
{
  // GRAPH POINT 

  GraphPoint::GraphPoint(QWidget *parent, int pointID, int pX, int pY, double valX, double valY) :
    QWidget(parent),
    ID(pointID),
    pixelX(pX),
    pixelY(pY),
    valueX(valX),
    valueY(valY)
  {
    parent->installEventFilter(this);
    pointSize = 10;
    curvature = "'lin'";
    this->setGeometry(pixelX - pointSize / 2, pixelY - pointSize / 2, pointSize + 1, pointSize + 1);

    setFocusPolicy(Qt::StrongFocus);
    type = PointType::vertex;

    labelID = new QLabel(parentWidget());
    labelID->setAttribute(Qt::WA_NoMousePropagation);
    labelID->setText(tr("ID: %1").arg(QString::number(ID)));
    labelID->setGeometry(pixelX, pixelY, 80, 25);

    labelTime = new QLabel(parentWidget());
    labelTime->setAttribute(Qt::WA_NoMousePropagation);
    labelTime->setText(tr("x: %1").arg(QString::number(valX)));
    labelTime->setGeometry(pixelX, pixelY + 15, 80, 25);

    labelLevel = new QLabel(parentWidget());
    labelLevel->setAttribute(Qt::WA_NoMousePropagation);
    labelLevel->setText(tr("y: %1").arg(QString::number(valY)));
    labelLevel->setGeometry(pixelX, pixelY + 30, 80, 25);

    labelCurve = new QLabel(parentWidget());
    labelCurve->setAttribute(Qt::WA_NoMousePropagation);
    labelCurve->setText(tr("crv: %1").arg(curvature));
    labelCurve->setGeometry(pixelX, pixelY + 15, 80, 25);

    labelID->hide();
    labelTime->hide();
    labelLevel->hide();
    labelCurve->hide();
  }

  QRect GraphPoint::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

  void GraphPoint::setID(int newID)
  {
    ID = newID;
    labelID->setText(tr("ID: %1").arg(QString::number(ID)));
  }
  void GraphPoint::setX(int pX, double valX)
  {
    pixelX = pX;
    valueX = valX;
    this->setGeometry(pixelX - pointSize / 2, pixelY - pointSize / 2, pointSize + 1, pointSize + 1);
    labelTime->setText(tr("x: %1").arg(QString::number(valX)));
  }
  void GraphPoint::setY(int pY, double valY)
  {
    pixelY = pY;
    valueY = valY;
    this->setGeometry(pixelX - pointSize / 2, pixelY - pointSize / 2, pointSize + 1, pointSize + 1);
    labelLevel->setText(tr("y: %1").arg(QString::number(valY)));
  }
  void GraphPoint::setType(GraphPoint::PointType newType)
  {
    type = newType;
    // update();
  }
  void GraphPoint::setCurvature(QString newCurvature)
  {
    curvature = newCurvature;
    labelCurve->setText(tr("crv: %1").arg(curvature));
  }

  void GraphPoint::mousePressEvent(QMouseEvent *mouseEvent)
  {
    mousePressCoor = mouseEvent->pos();
    mouseGlobalCoor = mouseEvent->globalPos();
  }

  void GraphPoint::mouseMoveEvent(QMouseEvent *mouseEvent)
  {
    QPoint pos = this->parentWidget()->mapFromGlobal(mouseEvent->globalPos());

    switch (type)
    {
    case vertex:
      this->setGeometry(
        pos.x() - mousePressCoor.x(),
        pos.y() - mousePressCoor.y(),
        this->width(),
        this->height()
        );
      break;
    case startPoint:
    case endPoint:
      this->setGeometry(
        pixelX - pointSize / 2,
        pos.y() - mousePressCoor.y(),
        this->width(),
        this->height()
        );
      break;

    default:
      break;
    }

    int tempX = this->pos().x() + pointSize / 2;
    int tempY = this->pos().y() + pointSize / 2;

    labelID->setGeometry(tempX, tempY, 80, 25);
    labelTime->setGeometry(tempX, tempY + 15, 80, 25);
    labelLevel->setGeometry(tempX, tempY + 30, 80, 25);
    labelCurve->setGeometry(tempX, tempY + 15, 80, 25);
    //update();

  }

  void GraphPoint::mouseReleaseEvent(QMouseEvent *mouseEvent)
  {
    int deltaX, deltaY;
    bool moved = false;

    switch (type)
    {
    case vertex:
      deltaX = mouseEvent->globalPos().x() - mouseGlobalCoor.x();
      deltaY = mouseEvent->globalPos().y() - mouseGlobalCoor.y();
      pixelX += deltaX;
      pixelY += deltaY;
      if (deltaX != 0 || deltaY != 0) { moved = true; }
      break;

    case startPoint:
    case endPoint:
      deltaY = mouseEvent->globalPos().y() - mouseGlobalCoor.y();
      pixelY += deltaY;
      if (deltaY != 0) { moved = true; }
      break;
    }

    if (moved) { emit actMoved(ID, pixelX, pixelY); }
  }

  bool GraphPoint::eventFilter(QObject* target, QEvent* event)
  {
    if (hasFocus())
    {
      if (event->type() == QEvent::KeyPress)
      {
        QKeyEvent* eventKey = static_cast<QKeyEvent*>(event);
        quint32 modifers = eventKey->nativeModifiers();

        switch (eventKey->key())
        {
        case Qt::Key::Key_Delete:

          if (type == PointType::vertex)
          {
            emit actDelete(ID);
            this->close();
            event->accept();
            return true;
          }
          break;

        case Qt::Key::Key_Escape:
          qDebug() << "KeyEvent: Escape PRESSED";
          event->accept();
          return true;
          break;
        }
      }
    }

    return QObject::eventFilter(target, event);
  }

  void GraphPoint::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);

    if (this->hasFocus()) { painter.setPen(QColor(120, 20, 20)); }
    else { painter.setPen(QColor(Qt::white)); }

    switch (type)
    {
    case Jui::GraphPoint::vertex:
      painter.drawEllipse(bounds());
      break;
    case Jui::GraphPoint::startPoint:
      //painter.drawRect(bounds());
      painter.drawChord(bounds(), 16 * 270, 16 * 180);
      break;
    case Jui::GraphPoint::endPoint:
      //painter.drawRect(bounds());
      painter.drawChord(bounds(), 16 * 90, 16 * 180);
      break;
    case Jui::GraphPoint::curvePoint:
      painter.drawRect(bounds());
      break;
    }

  }
  void GraphPoint::closeEvent(QCloseEvent *event)
  {
    labelID->close();
    labelTime->close();
    labelLevel->close();
    labelCurve->close();
  }

  void GraphPoint::focusInEvent(QFocusEvent* event)
  {
    switch (type)
    {
    case Jui::GraphPoint::vertex:
    case Jui::GraphPoint::startPoint:
    case Jui::GraphPoint::endPoint:
      labelID->show();
      labelTime->show();
      labelLevel->show();
      labelCurve->hide();
      break;
    case Jui::GraphPoint::curvePoint:
      labelID->show();
      labelTime->hide();
      labelLevel->hide();
      labelCurve->show();
      break;
    }
    update();
  }

  void GraphPoint::focusOutEvent(QFocusEvent* event)
  {
    labelID->hide();
    labelTime->hide();
    labelLevel->hide();
    labelCurve->hide();
    update();
  }

  void GraphPoint::moveEvent(QMoveEvent * event)
  {
    labelID->setGeometry(pixelX, pixelY, 80, 25);
    labelTime->setGeometry(pixelX, pixelY + 15, 80, 25);
    labelLevel->setGeometry(pixelX, pixelY + 30, 80, 25);
    labelCurve->setGeometry(pixelX, pixelY + 15, 80, 25);
    update();
  }

  GraphPoint::~GraphPoint(){}

  // GRAPH POINT END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH CURVE	

  GraphCurve::GraphCurve(QWidget *parent, GraphPoint *pt1, GraphPoint *pt2) :
    QWidget(parent),
    from(pt1),
    to(pt2)
  {
    int fromX = pt1->x() + pt1->pointSize;
    int fromY;
    int newWidth = pt2->x() - pt1->x();
    int newHeight = pt2->y() - pt1->y();

    if (pt1->y() > pt2->y()) { fromY = pt1->y(); }
    else { fromY = pt2->y(); }

    this->setGeometry(fromX, fromY, newWidth, newHeight);

  }

  QRect GraphCurve::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

  void GraphCurve::mousePressEvent(QMouseEvent *mouseEvent)
  {
    //mouseEvent->ignore();
  }

  void GraphCurve::resizeEvent(QResizeEvent *resizeEvent)
  {
    // this->setGeometry(from->x(), from->y(), 100, 100);
  }

  void GraphCurve::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setPen(QColor(120, 20, 20));
    //painter.fillRect(this->bounds(), QColor(120, 30, 30));
    painter.drawRect(this->bounds());
  }

  GraphCurve::~GraphCurve(){}

  // GRAPH CURVE END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH 


  Graph::Graph(QWidget *parent) : QWidget(parent)
  {
    this->setMouseTracking(true);
    frameOffset = 25;
    minDomainX = 0;
    maxDomainX = 1;
    minDomainY = 0;
    maxDomainY = 1;

    controlPts = QList<GraphPoint*>();
    graphPolylines = new QPolygonF();
    collPolylinesNEW = QList<QPolygonF*>();

    setFocusPolicy(Qt::StrongFocus);
  }

  QRect Graph::bounds() { return QRect(0, 0, width(), height()); }
  QRect Graph::boundsGraph()
  {
    return bounds().adjusted(frameOffset * 2, 14, -14, -frameOffset);
  }

  void Graph::setDomainX(double min, double max)
  {
    minDomainX = min;
    maxDomainX = max;
    update();
  }
  void Graph::setDomainY(double min, double max)
  {
    minDomainY = min;
    maxDomainY = max;
    update();
  }

  QList<double> Graph::getDomainX()
  {
    QList<double> range;
    range.append(minDomainX);
    range.append(maxDomainX);
    return range;
  }
  QList<double> Graph::getDomainY()
  {
    QList<double> range;
    range.append(minDomainY);
    range.append(maxDomainY);
    return range;
  }

  double Graph::getValueX(int displayX)
  {
    double perc = (displayX - boundsGraph().left()) / (double)boundsGraph().width();
    return perc * (maxDomainX - minDomainX) + minDomainX;
  }
  double Graph::getValueY(int displayY)
  {
    double perc = 1 - (displayY - boundsGraph().top()) / (double)boundsGraph().height();
    return perc * (maxDomainY - minDomainY) + minDomainY;
  }
  double Graph::getPixelX(double valueX)
  {
    double perc = (valueX - minDomainX) / (double)(maxDomainX - minDomainX);
    return perc * boundsGraph().width() + boundsGraph().left();
  }
  double Graph::getPixelY(double valueY)
  {
    double perc = (valueY - minDomainY) / (double)(maxDomainY - minDomainY);
    return boundsGraph().height() - (perc * boundsGraph().height()) + boundsGraph().top();
  }

  GraphPoint *Graph::addValuePoint(double valueX, double valueY, GraphPoint::PointType type = GraphPoint::PointType::vertex)
  {
    GraphPoint *pt = new GraphPoint(
      this,
      controlPts.size(),
      getPixelX(valueX),
      getPixelY(valueY),
      valueX,
      valueY
      );
    pt->type = type;
    pt->show();
    this->connect(pt, SIGNAL(actDelete(int)), this, SLOT(onDeletePoint(int)));
    this->connect(pt, SIGNAL(actMoved(int, int, int)), this, SLOT(onMovePoint(int, int, int)));

    qDebug() << "GraphPoint -> valueX: " << pt->valueX
      << " valueY: " << pt->valueY
      << " pixelX: " << pt->pixelX
      << " pixelY: " << pt->pixelY;

    if (type == GraphPoint::PointType::vertex)
    {
      if (controlPts.size() == 0) { controlPts.append(pt); }
      else
      {
        for (int i = 0; i <= controlPts.size(); i++)
        {
          if (i == controlPts.size()) {
            controlPts.append(pt);
            break;
          }
          if (controlPts[i]->valueX > pt->valueX)
          {
            controlPts.insert(i, pt);
            break;
          }
        }
      }
    }
    else if (type == GraphPoint::PointType::curvePoint)
    {
      curvePts.append(pt);
    }
    return pt;
  }

  GraphPoint *Graph::addVertexPoint(QPointF pt)
  {
    return this->addValuePoint(pt.x(), pt.y(), GraphPoint::PointType::vertex);
  }
  void Graph::setVertexPoint(int ID, QPointF pt)
  {
    controlPts[ID]->setX(getPixelX(pt.x()), pt.x());
    controlPts[ID]->setY(getPixelY(pt.y()), pt.y());
  }
  void Graph::setVertexType(int ID, GraphPoint::PointType newType)
  {
    controlPts[ID]->setType(newType);
  }
  GraphPoint *Graph::getVertex(int ID) { return controlPts.at(ID); }

  GraphPoint *Graph::addCurvePoint(QPointF pt)
  {
    return this->addValuePoint(pt.x(), pt.y(), GraphPoint::PointType::curvePoint);
  }
  void Graph::setCurvePoint(int ID, QPointF pt)
  {
    curvePts[ID]->setX(getPixelX(pt.x()), pt.x());
    curvePts[ID]->setY(getPixelY(pt.y()), pt.y());
  }
  void Graph::setCurveCurvature(int ID, QString txt)
  {
    curvePts[ID]->setCurvature(txt);
  }

  void Graph::drawPoint(double valueX, double valueY)
  {
    collDrawPoints.append(new QPointF(valueX, valueY));
    update();
  }
  void Graph::drawLine(double valueX1, double valueY1, double valueX2, double valueY2)
  {
    collDrawLines.append(new QLineF(valueX1, valueY1, valueX2, valueY2));
    update();
  }
  void Graph::drawPolyline(QVector<QPointF> collPoints)
  {
    graphPolylines = new QPolygonF(collPoints);
    update();
  }
  void Graph::addPolyline(QVector<QPointF> collPoints)
  {
    collPolylinesNEW.append(new QPolygonF(collPoints));
  }

  void Graph::deleteGraph()
  {
    foreach(GraphPoint *onePoint, controlPts) { onePoint->close(); }
    controlPts = QList<GraphPoint*>();

    foreach(GraphPoint *onePoint, curvePts) { onePoint->close(); }
    curvePts = QList<GraphPoint*>();

    collDrawPoints = QList<QPointF*>();
    collDrawLines = QList<QLineF*>();
    graphPolylines = new QPolygonF();
    collPolylinesNEW = QList<QPolygonF*>();

    update();
  }

  void Graph::onDeletePoint(int ID)
  {
    qDebug() << "Graph::onDeletePoint: " << QString::number(ID);
    controlPts.removeAt(ID);
    curvePts[ID - 1]->close();
    curvePts.removeAt(ID - 1);
    this->makeEnv();
    update();
  }
  void Graph::onMovePoint(int ID, int pixelX, int pixelY)
  {
    qDebug() << "Graph::onMovePoint: " << QString::number(ID);

    double newValX = getValueX(pixelX);
    double newValY = getValueY(pixelY);
    int newPixelX = pixelX;
    int newPixelY = pixelY;

    if (newValX > maxDomainX) { newValX = maxDomainX - 0.01; newPixelX = getPixelX(maxDomainX - 0.01); };
    if (newValX < minDomainX) { newValX = minDomainX; newPixelX = getPixelX(minDomainX); };

    if (newValY > maxDomainY) { newValY = maxDomainY; newPixelY = getPixelY(maxDomainY); };
    if (newValY < minDomainY) { newValY = minDomainY; newPixelY = getPixelY(minDomainY); };

    controlPts[ID]->setX(newPixelX, newValX);
    controlPts[ID]->setY(newPixelY, newValY);

    this->sortPointsByX();
    this->makeEnv();
  }

  void Graph::sortPointsByX()
  {
    bool sorted = false;
    if (controlPts.size() == 0) { sorted = true; }

    while (!sorted)
    {
      sorted = true;
      for (int i = 1; i < controlPts.size(); i++)
      {
        if (controlPts[i]->valueX < controlPts[i - 1]->valueX)
        {
          sorted = false;
          controlPts[i]->setID(i - 1);
          controlPts[i - 1]->setID(i);
          controlPts.swap(i, i - 1);
          break;
        }
      }
    }
  }

  void Graph::makeEnv()
  {

    QList<double> levels;
    QList<double> times;
    QList<QString> curves;

    for (int i = 0; i < controlPts.size(); i++)
    {
      levels.append(controlPts[i]->valueY);

      if (i != 0)
      {
        double previousTime = 0;
        foreach(GraphPoint *onePt, controlPts)
        {
          if (onePt->valueX < controlPts[i]->valueX)
          {
            if (onePt->valueX > previousTime)
            {
              previousTime = onePt->valueX;
            }
          }
        }
        times.append(controlPts[i]->valueX - previousTime);
      }
    };

    foreach(GraphPoint *onePt, curvePts)
    {
      curves.append(onePt->curvature);
    };

    qDebug() << "Graph::makeEnv -> "
      << "levels: " << levels
      << "times: " << times
      << "curves: " << curves;

    emit actEnvGraphChanged(levels, times, curves);
  }

  void Graph::resizeEvent(QResizeEvent *resizeEvent)
  {
    //qDebug() << "Graph::resizeEvent";
    foreach(GraphPoint *onePt, controlPts)
    {
      int pointSize = onePt->pointSize;
      onePt->pixelX = getPixelX(onePt->valueX);
      onePt->pixelY = getPixelY(onePt->valueY);
      onePt->setGeometry(
        onePt->pixelX - pointSize / 2,
        onePt->pixelY - pointSize / 2,
        pointSize + 1,
        pointSize + 1
        );
    }
    foreach(GraphPoint *onePt, curvePts)
    {
      int pointSize = onePt->pointSize;
      onePt->pixelX = getPixelX(onePt->valueX);
      onePt->pixelY = getPixelY(onePt->valueY);
      onePt->setGeometry(
        onePt->pixelX - pointSize / 2,
        onePt->pixelY - pointSize / 2,
        pointSize + 1,
        pointSize + 1
        );
    }
  }

  void Graph::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);

    //painter.fillRect(bounds(), QColor(10, 10, 10));
    if (this->hasFocus()) { painter.setPen(QColor(120, 20, 20)); }
    else { painter.setPen(QColor(60, 60, 60)); }
    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, height() - 1, width(), height() - 1);

    painter.setPen(QPen(Qt::white, 1));

    painter.drawRect(this->boundsGraph());

    QTextOption option;
    option.setAlignment(Qt::AlignCenter);

    //painter.drawText(QRect(10, 10, 50, 30), tr("numPts: %1").arg(controlPts.size()), option);

    painter.setPen(QPen(QColor(70, 70, 70), 1));

    // verticals
    int cntSegX = 4;
    for (int i = 0; i <= cntSegX; i++)
    {
      int pixelX = boundsGraph().width() / cntSegX * i + boundsGraph().left();
      double valX = getValueX(pixelX);
      painter.drawText(
        QRect(pixelX - 25, boundsGraph().bottom() + 5, 50, 20),
        QString::number(valX, 'f', 2),
        option
        );
      painter.drawLine(pixelX, boundsGraph().top(), pixelX, boundsGraph().height() + boundsGraph().top());
    };

    // horizontals
    int cntSegY = 4;
    for (int i = 0; i <= cntSegY; i++)
    {
      int pixelY = boundsGraph().height() / cntSegY * i + boundsGraph().top();
      //int pixelY = boundsGraph().height() / cntSegY * i + 14;
      double valY = getValueY(pixelY);
      painter.drawText(
        QRect(5, pixelY - 10, 40, 20),
        QString::number(valY, 'f', 2),
        option
        );
      painter.drawLine(boundsGraph().left(), pixelY, boundsGraph().right(), pixelY);
    };

    //collDrawPoints
    painter.setPen(QColor(150, 150, 150));
    foreach(QPointF *onePoint, collDrawPoints)
    {
      painter.drawEllipse(getPixelX(onePoint->x()) - 3, getPixelY(onePoint->y()) - 3, 6, 6);
    };

    // collDrawLines
    painter.setPen(QColor(70, 170, 70));
    foreach(QLineF *oneLine, collDrawLines)
    {
      QPointF pt1 = QPoint(getPixelX(oneLine->p1().x()), getPixelY(oneLine->p1().y()));
      QPointF pt2 = QPoint(getPixelX(oneLine->p2().x()), getPixelY(oneLine->p2().y()));
      painter.drawLine(pt1, pt2);
    };

    // collPolylines

    painter.setPen(QColor(40, 240, 40));
    foreach(QPolygonF *onePoly, collPolylinesNEW)
    {
      QPolygonF poly;
      foreach(QPointF onePt, onePoly->toList())
      {
        //onePt.setX(getPixelX(onePt.x()));
        //onePt.setY(getPixelY(onePt.y()));
        poly.append(onePt);
      }
      painter.drawPolyline(poly);
    }


    //graphPolylines
    painter.setPen(QColor(240, 70, 70));
    QPolygonF poly;
    foreach(QPointF onePt, graphPolylines->toList())
    {
      onePt.setX(getPixelX(onePt.x()));
      onePt.setY(getPixelY(onePt.y()));
      //qDebug() << "DrawonePt " << onePt;
      poly.append(onePt);
    }
    painter.drawPolyline(poly);
  }

  void Graph::mousePressEvent(QMouseEvent *mouseEvent)
  {
    qDebug() << "Graph::mousePressEvent NEW POINT ADD";
    this->addValuePoint(getValueX(mouseEvent->pos().x()), getValueY(mouseEvent->pos().y()));
    this->makeEnv();
  }

  void Graph::mouseReleaseEvent(QMouseEvent *mouseEvent)
  {
    //update();
    //mouseEvent->accept();
  }

  Graph::~Graph() { }
}