#include "Graph.h"

namespace Jui
{

  // GRAPH OBJECT

  GraphObject::GraphObject(QWidget *parent) :
    graph(parent)
  {
    Graph *parentGraph = qobject_cast<Graph*>(parent);
    graphOrigin = parentGraph->boundsGraph().topLeft();
    graphSize = parentGraph->boundsGraph().size();

    domainX = parentGraph->getDomainX();
    domainY = parentGraph->getDomainY();

    modifyAlpha = 0;
    fadeTimeOut = 800;
    fps = 25;
    modifyTime = new QTimer(this);

    connect(this, SIGNAL(actModify()), this, SLOT(onObjectModify()));
    connect(modifyTime, SIGNAL(timeout()), this, SLOT(onModifyTick()));

    connect(
      parentGraph, SIGNAL(actDomainChanged(QPair<float, float>, QPair<float, float>)),
      this, SLOT(onDomainChanged(QPair<float, float>, QPair<float, float>))
      );

    connect(parentGraph, SIGNAL(actResized(QSize)), this, SLOT(onGraphResized(QSize)));
  }

  void GraphObject::onDomainChanged(QPair<float, float> domX, QPair<float, float> domY)
  {
    domainX = domX;
    domainY = domY;
    emit actModify();
  }

  void GraphObject::onGraphResized(QSize newSize)
  {
    graphSize = newSize;
    emit actModify();
  }

  void GraphObject::onObjectModify()
  {
    modify = true;

    modifyAlpha = 255;
    modifyTime->stop();
    modifyTime->setInterval(fadeTimeOut / fps);
    modifyTime->start();
  }

  void GraphObject::onModifyTick()
  {
    modifyAlpha -= 255 / fps;
    if (modifyAlpha <= 0)
    {
      modifyTime->stop();
      modifyAlpha = 0;
      modify = false;
    }
    //qDebug() << "modifyAlpha " << modifyAlpha;
    this->redraw();
  }

  QPoint GraphObject::getPixel()
  {
    float percX = (valueX - domainX.first) / (float)(domainX.second - domainX.first);
    int pixelX = percX * graphSize.width() + graphOrigin.x();;

    float percY = (valueY - domainY.first) / (float)(domainY.second - domainY.first);
    int pixelY = graphSize.height() - (percY * graphSize.height()) + graphOrigin.y();

    return QPoint(pixelX, pixelY);
  }

  void GraphObject::setValue(QPoint pixel)
  {
    float percX = (pixel.x() - graphOrigin.x()) / (float)graphSize.width();
    valueX = percX * (domainX.second - domainX.first) + domainX.first;

    float percY = 1 - (pixel.y() - graphOrigin.y()) / (float)graphSize.height();
    valueY = percY * (domainY.second - domainY.first) + domainY.first;

    emit actModify();
  }

  void GraphObject::draw(QPainter *painter)
  {
    painter->setPen(Qt::white);
    painter->drawEllipse(this->getPixel(), 2, 2);
  }

  void GraphObject::redraw() { graph->update(); }

  GraphObject::~GraphObject(){};

  // GRAPH OBJECT END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH VERTEX

  GraphVertex::GraphVertex(QWidget *parent) : GraphObject(parent)
  {
    Graph *parentGraph = qobject_cast<Graph*>(parent);

    parent->installEventFilter(this);
    isSelcted = false;
    pressed = false;
    modify = false;
    ID = -1;

    type = VertexType::vertex;

    cBoxX = new ControlBox(parent);
    cBoxX->setLabelSize(8);
    cBoxX->setLabel("x");
    cBoxX->hide();
    cBoxY = new ControlBox(parent);
    cBoxY->setLabel("y");
    cBoxY->setLabelSize(8);
    cBoxY->hide();

    connect(this, SIGNAL(actMoved(int)), parentGraph, SLOT(onVertexMoved(int)));
    connect(this, SIGNAL(actSelected(int)), parentGraph, SLOT(onVertexSelected(int)));
    connect(this, SIGNAL(actDeleted(int)), parentGraph, SLOT(onVertexDeleted(int)));

    connect(cBoxX, SIGNAL(actValueChanged(QString)), this, SLOT(onCBoxValXChanged(QString)));
    connect(cBoxY, SIGNAL(actValueChanged(QString)), this, SLOT(onCBoxValYChanged(QString)));
  }

  void GraphVertex::setType(VertexType vertexType)  { type = vertexType; }

  void GraphVertex::onCBoxValXChanged(QString txtVal)
  {
    valueX = txtVal.toFloat();
    emit actModify();
  }
  void GraphVertex::onCBoxValYChanged(QString txtVal)
  {
    valueY = txtVal.toFloat();
    emit actModify();
  }

  void GraphVertex::setSelected(bool select)
  {
    isSelcted = select;
    if (isSelcted)
    {
      cBoxX->show();
      cBoxY->show();
    }
    else
    {
      cBoxX->hide();
      cBoxY->hide();
    }
  }

  bool GraphVertex::eventFilter(QObject* target, QEvent* event)
  {
    QMouseEvent *eventMouse;

    switch (event->type())
    {
    case QEvent::MouseButtonPress:
      eventMouse = static_cast<QMouseEvent*>(event);

      if (this->isOver(eventMouse->pos()))
      {
        pressed = true;
        cBoxX->setValue(QString::number(valueX, 'f', 2));
        cBoxY->setValue(QString::number(valueY, 'f', 2));
        emit actSelected(ID);
        this->redraw();
        return true;
      }
      break;

    case QEvent::MouseButtonRelease:
      eventMouse = static_cast<QMouseEvent*>(event);
      pressed = false;
      break;

    case QEvent::MouseMove:
      if (isSelcted && pressed)
      {
        eventMouse = static_cast<QMouseEvent*>(event);
        if (this->type == VertexType::vertex)
        {
          this->setValue(eventMouse->pos());
        }
        else
        {
          QPoint px = this->getPixel();
          this->setValue(QPoint(px.x(), eventMouse->pos().y()));
        }
        cBoxX->setValue(QString::number(valueX, 'f', 2));
        cBoxY->setValue(QString::number(valueY, 'f', 2));

        emit actMoved(ID);
      }
      this->redraw();
      break;

    case QEvent::KeyPress:
      if (isSelcted)
      {
        // qDebug() << "GraphVertex::KeyPress" << ID;

        if (event->type() == QEvent::KeyPress)
        {
          QKeyEvent* eventKey = static_cast<QKeyEvent*>(event);
          quint32 modifers = eventKey->nativeModifiers();

          switch (eventKey->key())
          {
          case Qt::Key::Key_Delete:
            if (type == VertexType::vertex)
            {
              emit actDeleted(ID);
              this->redraw();
              this->deleteLater();
              return true;
            }
            break;

          case Qt::Key::Key_Escape:
            qDebug() << "KeyEvent: Escape PRESSED";
            return true;
            break;
          }
        }
      }
      break;
    }

    return QObject::eventFilter(target, event);
  }

  bool GraphVertex::isOver(QPointF mouse)
  {
    float tolerance = 10;
    QPointF testPt = this->getPixel();

    bool overX = abs(testPt.x() - mouse.x()) < tolerance;
    bool overY = abs(testPt.y() - mouse.y()) < tolerance;

    if (overX && overY) { return true; }
    else { return false; }
  }

  void GraphVertex::draw(QPainter *painter)
  {
    QPoint center = this->getPixel();
    QRect rect(center.x() - 8, center.y() - 8, 16, 16);

    /*
    QRect rectID(center.x() + 8, center.y(), 30, 20);
    QTextOption option;
    option.setAlignment(Qt::AlignCenter);
    painter->setPen(QColor(180, 180, 180));
    painter->drawText(rectID, tr("ID: %1").arg(QString::number(ID)), option);
    */

    QRect rectBoxX(center.x() + 8, center.y() + 8, 40, 17);
    QRect rectBoxY(center.x() + 8, center.y() + 28, 40, 17);
    cBoxX->setGeometry(rectBoxX);
    cBoxY->setGeometry(rectBoxY);

    if (isSelcted) { painter->setPen(QColor(180, 20, 20)); }
    else { painter->setPen(QColor(180, 180, 180)); }

    switch (type)
    {
    case vertex:
      painter->drawEllipse(this->getPixel(), 6, 6);
      break;
    case startPoint:
      painter->drawChord(rect, 16 * 270, 16 * 180);
      break;
    case endPoint:
      painter->drawChord(rect, 16 * 90, 16 * 180);
      break;
    }

    if (modify)
    {
      QColor modifyColor = QColor(220, 200, 20);// modifyColor;
      modifyColor.setAlpha(modifyAlpha);
      painter->setPen(QPen(modifyColor, 2));
      painter->drawEllipse(this->getPixel(), 5, 5);
    }
  }

  GraphVertex::~GraphVertex()
  {
    qDebug() << "Vertex ~DELETED" << ID;
    cBoxX->close();
    cBoxY->close();
  };

  // GRAPH VERTEX END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH CURVE

  GraphCurve::GraphCurve(QWidget *parent, GraphVertex *pt1, GraphVertex *pt2) :
    GraphObject(parent),
    from(pt1),
    to(pt2)
  {
    type = CurveType::lin;
    ID = -1;
    pixelWidth = to->getPixel().x() - from->getPixel().x();
    isMouseInDomain = false;

    cBoxType = new ControlBox(parent);
    cBoxType->setLabelSize(15);
    cBoxType->setLabel("crv");
    cBoxType->setValue("lin");
    cBoxType->show();
    //cBoxType->hide();

    connect(from, SIGNAL(actModify()), this, SLOT(onObjectModify()));
    connect(to, SIGNAL(actModify()), this, SLOT(onObjectModify()));
    connect(cBoxType, SIGNAL(actValueChanged(QString)), this, SLOT(onCBoxTypeChanged(QString)));
  }
  void GraphCurve::setType(CurveType newType)
  {
    type = newType;
    switch (type)
    {
    case CurveType::step:
      cBoxType->setValue("step");
      break;
    default:
    case CurveType::lin:
      cBoxType->setValue("lin");
      break;
    case CurveType::exp:
      cBoxType->setValue("exp");
      break;
    case CurveType::sin:
      cBoxType->setValue("sin");
      break;
    case CurveType::hold:
      cBoxType->setValue("hold");
      break;
    }

    emit actModify();
  }
  void GraphCurve::setFrom(GraphVertex *pt)
  {
    disconnect(from, SIGNAL(actModify()), this, SLOT(onObjectModify()));
    from = pt;
    connect(from, SIGNAL(actModify()), this, SLOT(onObjectModify()));
  }
  void GraphCurve::setTo(GraphVertex *pt)
  {
    disconnect(to, SIGNAL(actModify()), this, SLOT(onObjectModify()));
    to = pt;
    connect(to, SIGNAL(actModify()), this, SLOT(onObjectModify()));
  }
  void GraphCurve::flipEnds()
  {
    disconnect(from, SIGNAL(actModify()), this, SLOT(onObjectModify()));
    disconnect(to, SIGNAL(actModify()), this, SLOT(onObjectModify()));

    GraphVertex *temp = from;
    from = to;
    to = temp;

    connect(from, SIGNAL(actModify()), this, SLOT(onObjectModify()));
    connect(to, SIGNAL(actModify()), this, SLOT(onObjectModify()));
  }

  bool GraphCurve::isOverCurveDomain(float valX)
  {
    isMouseInDomain = true;
    if (from->valueX > valX) { isMouseInDomain = false; return false; }
    if (to->valueX < valX) { isMouseInDomain = false; return false; }
    return true;
  }

  float GraphCurve::at(float valX)
  {
    float percX = (valX - from->valueX) / (to->valueX - from->valueX);
    qreal domFrom, domTo;
    float valAt;

    switch (type)
    {
    case CurveType::step:
      valAt = to->valueY;
      break;

    default:
    case CurveType::lin:
      valAt = (to->valueY - from->valueY)*percX + from->valueY;
      break;

    case CurveType::exp:
      if (from->valueY < to->valueY)
      {
        valAt = (to->valueY - from->valueY)*qPow(percX, 2) + from->valueY;
      }
      else
      {
        valAt = (to->valueY - from->valueY)*qPow(-percX, 2) + from->valueY;
      }
      /*
{ domFrom = -1; domTo = 0; }
else { domFrom = 0; domTo = 1; }


valAt = (to->valueY - from->valueY)*qPow(percX, 2) + from->valueY;
*/
      break;

    case CurveType::hold:
      valAt = from->valueY;
      break;
    }

    // qDebug() << "valX " << valX << " AT: " << valAt;
    return valAt;
  }

  void GraphCurve::onObjectModify()
  {
    GraphObject::onObjectModify();

    QPoint fromPt = from->getPixel();
    QPoint toPt = to->getPixel();
    pixelWidth = toPt.x() - fromPt.x();
    int cntSeg = 20;
    qreal segStep = (to->valueX - from->valueX) / cntSeg;//  pixelWidth;
    qreal domFrom, domTo;
    qreal pX, pY;
    qreal percY;

    polygon = QPolygon();
    switch (type)
    {
    case step:
      polygon.append(fromPt);
      polygon.append(QPoint(fromPt.x(), toPt.y()));
      polygon.append(toPt);
      break;

    default:
    case lin:
      polygon.append(fromPt);
      polygon.append(toPt);
      break;

    case exp:

      if (fromPt.y() < toPt.y()) { domFrom = -1; domTo = 0; }
      else { domFrom = 0; domTo = 1; }

      polygon.append(fromPt);

      for (qreal percX = domFrom + 1 / (qreal)cntSeg; percX < domTo; percX += 1 / (qreal)cntSeg)
      {
        percY = qPow(percX, 2);
        if (fromPt.y() < toPt.y())
        {
          pX = (toPt.x() - fromPt.x())*(1 + percX) + fromPt.x();
          pY = (fromPt.y() - toPt.y())*percY + toPt.y();
        }
        else
        {
          pX = (toPt.x() - fromPt.x())*percX + fromPt.x();
          pY = (fromPt.y() - toPt.y())*(1 - percY) + toPt.y();
        }
        polygon.append(QPoint(pX, pY));
      }
      polygon.append(toPt);
      break;

    case sin:
      if (fromPt.y() < toPt.y()) { domFrom = M_PI_2 * 3; domTo = M_PI_2 * 5; }
      else { domFrom = M_PI_2; domTo = M_PI_2 * 3; }

      for (qreal percX = domFrom; percX < domTo; percX += (domTo - domFrom) / (qreal)cntSeg)
      {
        percY = qSin(percX);
        if (fromPt.y() < toPt.y())
        {
          pX = (toPt.x() - fromPt.x())*(percX - domFrom) / M_PI + fromPt.x();
          pY = (fromPt.y() - toPt.y())*(1 - percY) / 2 + toPt.y();
        }
        else
        {
          pX = (toPt.x() - fromPt.x())*(percX - domFrom) / M_PI + fromPt.x();
          pY = (toPt.y() - fromPt.y())*(1 - percY) / 2 + fromPt.y();
        }
        polygon.append(QPoint(pX, pY));
      }
      polygon.append(toPt);
      break;

    case hold:
      polygon.append(fromPt);
      polygon.append(QPoint(toPt.x(), fromPt.y()));
      polygon.append(toPt);
      break;
    }


  }

  void GraphCurve::onCBoxTypeChanged(QString txtType)
  {
    if (txtType == "step") { this->setType(CurveType::step); }
    else if (txtType == "exp") { this->setType(CurveType::exp); }
    else if (txtType == "sin") { this->setType(CurveType::sin); }
    else if (txtType == "hold") { this->setType(CurveType::hold); }
    else { this->setType(CurveType::lin); }

    this->onObjectModify();
  }

  void GraphCurve::draw(QPainter *painter)
  {
    QPoint fromPt = from->getPixel();
    QPoint toPt = to->getPixel();
    QPoint center;
    center.setX((toPt.x() + fromPt.x()) / 2);
    center.setY((toPt.y() + fromPt.y()) / 2);

    QRect rectBoxType(center.x() - 20, center.y() - 8, 40, 16);
    cBoxType->setGeometry(rectBoxType);

    QRect rectID(center.x(), center.y(), 30, 20);
    QRect rectWidth(center.x(), center.y() + 15, 50, 20);

    QTextOption option;
    option.setAlignment(Qt::AlignLeft);
    painter->setPen(QColor(180, 180, 180));
    //painter->drawText(rectID, tr("ID: %1").arg(QString::number(ID)), option);
    //painter->drawText(rectWidth, tr("px: %1").arg(QString::number(pixelWidth)), option);

    if (isMouseInDomain) { painter->setPen(QColor(180, 20, 20)); }
    else { painter->setPen(QColor(40, 200, 40)); }

    painter->drawPolyline(polygon);

    if (modify)
    {
      QColor modifyColor = QColor(220, 200, 20);// modifyColor;
      modifyColor.setAlpha(modifyAlpha);
      painter->setPen(QPen(modifyColor, 2));
      painter->drawPolyline(polygon);
    }
  }

  GraphCurve::~GraphCurve(){};

  // GRAPH CURVE END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH AXIS

  GraphAxis::GraphAxis(QWidget *parent, float value, AxisType direction = AxisType::horizontal) :
    GraphObject(parent),
    type(direction)
  {
    Graph *parentGraph = qobject_cast<Graph*>(parent);

    from = new GraphVertex(parent);
    to = new GraphVertex(parent);

    switch (type)
    {
    case AxisType::horizontal:
      from->valueX = 0;
      from->valueY = value;
      to->valueX = 1;
      to->valueY = value;
      break;
    case AxisType::vertical:
      from->valueX = value;
      from->valueY = 1;
      to->valueX = value;
      to->valueY = 0;
      break;
    }

    connect(
      parentGraph, SIGNAL(actDomainChanged(QPair<float, float>, QPair<float, float>)),
      this, SLOT(onDomainChanged(QPair<float, float>, QPair<float, float>))
      );
  }
  void GraphAxis::setValue(float newValue)
  {
    switch (type)
    {
    case AxisType::horizontal:
      from->valueY = newValue;
      to->valueY = newValue;
      break;
    case AxisType::vertical:
      from->valueX = newValue;
      to->valueX = newValue;
      break;
    }
  }

  void GraphAxis::onDomainChanged(QPair<float, float> domX, QPair<float, float> domY)
  {
    GraphObject::onDomainChanged(domX, domY);

    switch (type)
    {
    case AxisType::horizontal:
      from->valueX = domX.first;
      to->valueX = domX.second;
      break;
    case AxisType::vertical:
      from->valueY = domY.second;
      to->valueY = domY.first;
      break;
    }
  }

  void GraphAxis::draw(QPainter *painter)
  {
    QPoint pxFrom = from->getPixel();
    QPoint pxTo = to->getPixel();

    painter->setPen(QColor(70, 70, 70));
    painter->drawLine(pxFrom, pxTo);

    QTextOption option;
    option.setAlignment(Qt::AlignCenter);

    QRect rect;
    switch (type)
    {
    case AxisType::horizontal:
      rect = QRect(pxFrom.x() - 50, pxFrom.y() - 10, 50, 20);
      painter->drawText(rect, QString::number(from->valueY, 'f', 2), option);
      break;
    case AxisType::vertical:
      rect = QRect(pxTo.x() - 25, pxTo.y(), 50, 20);
      painter->drawText(rect, QString::number(from->valueX, 'f', 2), option);
      break;
    }
  }

  GraphAxis::~GraphAxis(){}

  // GRAPH AXIS END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH MOUSE 

  GraphMouse::GraphMouse(QWidget *parent) : GraphObject(parent)
  {
    Graph *parentGraph = qobject_cast<Graph*>(parent);

    parent->installEventFilter(this);

    axisX = new GraphAxis(parent, 0, horizontal);
    axisY = new GraphAxis(parent, 0, vertical);
    curvePt = new GraphObject(parent);

    connect(
      this, SIGNAL(actPositionChanged(QPair<float, float>)),
      parentGraph, SLOT(onMouseMoved(QPair<float, float>))
      );
  }

  bool GraphMouse::eventFilter(QObject* target, QEvent* event)
  {
    QMouseEvent *eventMouse;
    QPair<float, float> mouseVal;

    switch (event->type())
    {
    case QEvent::MouseMove:

      eventMouse = static_cast<QMouseEvent*>(event);
      this->setValue(eventMouse->pos());

      mouseVal.first = this->valueX;
      mouseVal.second = this->valueY;

      emit actPositionChanged(mouseVal);

      axisX->setValue(this->valueY);
      axisY->setValue(this->valueX);

      return true;
      break;
    }
    return QObject::eventFilter(target, event);
  }

  void GraphMouse::draw(QPainter *painter)
  {
    axisX->draw(painter);
    axisY->draw(painter);

    curvePt->valueX = this->valueX;
    curvePt->valueY = this->crvVal;
    QPoint centerPt = curvePt->getPixel();
    QRect rectVal(centerPt.x() - 15, centerPt.y() - 20, 30, 20);

    QTextOption option;
    option.setAlignment(Qt::AlignCenter);
    painter->setPen(QColor(230, 230, 230));
    painter->drawEllipse(centerPt, 2, 2);
    painter->drawText(rectVal, QString::number(crvVal, 'f', 2), option);
  }
  GraphMouse::~GraphMouse(){}

  // GRAPH MOUSE END
  ///////////////////////////////////////////////////////////////////////////////////////////////////


  // GRAPH POINT 
  /*
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
  */

  // GRAPH POINT END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH CURVE	
  /*
  GraphCurve::GraphCurve(QWidget *parent, GraphPoint *pt1, GraphPoint *pt2) :
  QWidget(parent),
  from(pt1),
  to(pt2)
  {
  int fromX = pt1->pixelX;
  int fromY;
  int newWidth = pt2->pixelX - pt1->pixelX;
  int newHeight = pt2->pixelY - pt1->pixelY;

  if (pt1->pixelY > pt2->pixelY) { fromY = pt1->pixelY; }
  else { fromY = pt1->pixelY; }

  qDebug() << "newWidth " << newWidth;
  //this->setGeometry(fromX, fromY, newWidth, newHeight);
  this->setGeometry(fromX, fromY, 50, 50);

  }

  QRect GraphCurve::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

  void GraphCurve::setFrom(GraphPoint *fromPt)
  {
  from = fromPt;
  this->setGeometry(from->pixelX, from->pixelY, 50, 50);
  }
  void GraphCurve::onFromMoved(int ID, int pixelX, int pixelY)
  {
  qDebug() << "GraphCurve::onFromMoved pX " << pixelX << " pY " << pixelY;
  this->setGeometry(pixelX, pixelY, 50, 50);
  }


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
  */

  // GRAPH CURVE END
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // GRAPH 


  Graph::Graph(QWidget *parent) : QWidget(parent)
  {
    this->setMouseTracking(true);
    frameOffset = 25;

    domainX.first = 0;
    domainX.second = 1;
    domainY.first = 0;
    domainY.second = 1;

    numGraphAxisX = 4;
    numGraphAxisY = 4;

    graphPolylines = new QPolygonF();

    setFocusPolicy(Qt::StrongFocus);

    graphMouse = new GraphMouse(this); // musi byt zadan pred vertexema

    testVertex1 = new GraphVertex(this);
    testVertex1->valueX = 0.01;
    testVertex1->valueY = 0.5;
    testVertex1->ID = 0;
    testVertex1->setType(VertexType::startPoint);
    controlVertexs.append(testVertex1);

    testVertex2 = new GraphVertex(this);
    testVertex2->valueX = 0.25;
    testVertex2->valueY = 0.85;
    testVertex2->ID = 1;
    controlVertexs.append(testVertex2);

    testVertex3 = new GraphVertex(this);
    testVertex3->valueX = 0.5;
    testVertex3->valueY = 0.5;
    testVertex3->ID = 2;
    controlVertexs.append(testVertex3);

    testVertex4 = new GraphVertex(this);
    testVertex4->valueX = 0.99;
    testVertex4->valueY = 0.25;
    testVertex4->ID = 3;
    testVertex4->setType(VertexType::endPoint);
    controlVertexs.append(testVertex4);

    testCurve1 = new GraphCurve(this, testVertex1, testVertex2);
    testCurve1->ID = 0;
    controlCurves.append(testCurve1);
    testCurve2 = new GraphCurve(this, testVertex2, testVertex3);
    testCurve2->ID = 1;
    testCurve2->setType(CurveType::sin);
    controlCurves.append(testCurve2);
    testCurve3 = new GraphCurve(this, testVertex3, testVertex4);
    testCurve3->ID = 2;
    controlCurves.append(testCurve3);

    for (double i = 0; i <= 1; i += 0.25)
    {
      GraphAxis *vAxis = new GraphAxis(this, i, AxisType::vertical);
      graphVerticalAxis.append(vAxis);
    }
    for (double i = 0; i <= 1; i += 0.25)
    {
      GraphAxis *hAxis = new GraphAxis(this, i, AxisType::horizontal);
      graphHorizontalAxis.append(hAxis);
    }

    this->sortVertexByX();

    emit actDomainChanged(domainX, domainY);
  }

  QRect Graph::bounds() { return QRect(0, 0, width(), height()); }
  QRect Graph::boundsGraph()
  {
    return bounds().adjusted(frameOffset * 2, 14, -14, -frameOffset);
  }

  void Graph::setDomainX(float min, float max)
  {
    domainX.first = min;
    domainX.second = max;

    float tempAxisVal = min;
    foreach(GraphAxis *oneAxis, graphVerticalAxis)
    {
      oneAxis->setValue(tempAxisVal);
      tempAxisVal += (max - min) / (float)numGraphAxisX;
    }

    update();

    emit actDomainChanged(domainX, domainY);

    // qDebug() << "GraphObject test return maxDomainX : " << testObj->test();
  }
  void Graph::setDomainY(float min, float max)
  {
    domainY.first = min;
    domainY.second = max;

    float tempAxisVal = min;
    foreach(GraphAxis *oneAxis, graphHorizontalAxis)
    {
      oneAxis->setValue(tempAxisVal);
      tempAxisVal += (max - min) / (float)numGraphAxisY;
    }

    update();

    emit actDomainChanged(domainX, domainY);
  }

  QPair<float, float> Graph::getDomainX() { return domainX; }
  QPair<float, float> Graph::getDomainY() { return domainY; }

  float Graph::getValueX(int displayX)
  {
    float perc = (displayX - boundsGraph().left()) / (float)boundsGraph().width();
    return perc * (domainX.second - domainX.first) + domainX.first;
  }
  float Graph::getValueY(int displayY)
  {
    float perc = 1 - (displayY - boundsGraph().top()) / (float)boundsGraph().height();
    return perc * (domainY.second - domainY.first) + domainY.first;
  }
  float Graph::getPixelX(float valueX)
  {
    float perc = (valueX - domainX.first) / (float)(domainX.second - domainX.first);
    return perc * boundsGraph().width() + boundsGraph().left();
  }
  float Graph::getPixelY(float valueY)
  {
    float perc = (valueY - domainY.first) / (float)(domainY.second - domainY.first);
    return boundsGraph().height() - (perc * boundsGraph().height()) + boundsGraph().top();
  }
  /*
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

  if (type == GraphPoint::PointType::curvePoint) { curvePts.append(pt); }
  else
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

  return pt;
  }

  GraphPoint *Graph::addStartPoint(QPointF pt)
  {
  return this->addValuePoint(pt.x(), pt.y(), GraphPoint::PointType::startPoint);
  }
  GraphPoint *Graph::addVertexPoint(QPointF pt)
  {
  GraphPoint *to = this->addValuePoint(pt.x(), pt.y(), GraphPoint::PointType::vertex);

  return to;

  }
  GraphPoint *Graph::addEndPoint(QPointF pt)
  {
  GraphPoint *to = this->addValuePoint(pt.x(), pt.y(), GraphPoint::PointType::endPoint);
  GraphPoint *from = controlPts[controlPts.size() - 2];

  return to;
  }
  */
  /*
  void Graph::setVertexPoint(int ID, QPointF pt)
  {
  int pX = getPixelX(pt.x());
  int pY = getPixelX(pt.y());
  //    controlPts[ID]->setX(pX, pt.x());
  //  controlPts[ID]->setY(pY, pt.y());

  //if (controlPts[ID]->type != GraphPoint::PointType::startPoint)
  //{
  // curves[ID - 1]->setFrom(controlPts[ID]);
  //}

  }
  */
  /*
  void Graph::setVertexType(int ID, GraphPoint::PointType newType)
  {
  controlPts[ID]->setType(newType);
  }
  GraphPoint *Graph::getVertex(int ID) { return controlPts.at(ID); }

  GraphPoint *Graph::addCurvePoint(QPointF pt)
  {
  return this->addValuePoint(pt.x(), pt.y(), GraphPoint::PointType::curvePoint);
  }
  */
  /*
  void Graph::setCurvePoint(int ID, QPointF pt)
  {
  // curvePts[ID]->setX(getPixelX(pt.x()), pt.x());
  // curvePts[ID]->setY(getPixelY(pt.y()), pt.y());
  }
  void Graph::setCurveCurvature(int ID, QString txt)
  {
  // curvePts[ID]->setCurvature(txt);
  }
  */

  /*
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

  void Graph::addPolyline(QVector<QPointF> collPoints)
  {
  collPolylinesNEW.append(new QPolygonF(collPoints));
  }
  */
  void Graph::drawPolyline(QVector<QPointF> collPoints)
  {
    graphPolylines = new QPolygonF(collPoints);
    update();
  }

  void Graph::deleteGraph()
  {
    /*
    foreach(GraphPoint *onePoint, controlPts) { onePoint->close(); }
    controlPts = QList<GraphPoint*>();

    foreach(GraphPoint *onePoint, curvePts) { onePoint->close(); }
    curvePts = QList<GraphPoint*>();
    */
    /*
    foreach(GraphCurve *oneCrv, curves) { oneCrv->close(); }
    curves = QList<GraphCurve*>();
    */

    //collDrawPoints = QList<QPointF*>();
    //collDrawLines = QList<QLineF*>();
    graphPolylines = new QPolygonF();
    // collPolylinesNEW = QList<QPolygonF*>();

    update();
  }

  /*
  void Graph::onDeletePoint(int ID)
  {
  qDebug() << "Graph::onDeletePoint: " << QString::number(ID);
  // controlPts.removeAt(ID);
  // curvePts[ID - 1]->close();
  // curvePts.removeAt(ID - 1);
  // this->makeEnv();
  // update();
  }
  */
  /*
  void Graph::onMovePoint(int ID, int pixelX, int pixelY)
  {
  qDebug() << "Graph::onMovePoint: " << QString::number(ID);

  double newValX = getValueX(pixelX);
  double newValY = getValueY(pixelY);
  int newPixelX = pixelX;
  int newPixelY = pixelY;

  if (newValX > domainX.second) { newValX = domainX.second - 0.01; newPixelX = getPixelX(domainX.second - 0.01); };
  if (newValX < domainX.first) { newValX = domainX.first; newPixelX = getPixelX(domainX.first); };

  if (newValY > domainY.second) { newValY = domainY.second; newPixelY = getPixelY(domainY.second); };
  if (newValY < domainY.first) { newValY = domainY.first; newPixelY = getPixelY(domainY.first); };

  // controlPts[ID]->setX(newPixelX, newValX);
  // controlPts[ID]->setY(newPixelY, newValY);

  // this->sortPointsByX();
  this->makeEnv();
  }
  */

  void Graph::onMouseMoved(QPair<float, float> mouseValue)
  {
    //qDebug() << "Graph::onMouseMoved x: " << mouseValue.first << " y: " << mouseValue.second;
    foreach(GraphCurve *oneC, controlCurves)
    {
      if (oneC->isOverCurveDomain(mouseValue.first))
      {
        // qDebug() << "Graph::onMouseMoved ID " << oneC->ID << " -> at: " << oneC->at(mouseValue.first);
        graphMouse->crvVal = oneC->at(mouseValue.first);
      }
    }
    // this->update();
  }

  void Graph::onVertexSelected(int ID)
  {
    // qDebug() << "Graph::onVertexSelected" << ID;
    foreach(GraphVertex *oneV, controlVertexs)
    {
      if (oneV->ID == ID) { oneV->setSelected(true); }
      else { oneV->setSelected(false); }
    }
  }

  void Graph::onVertexMoved(int ID)
  {
    // qDebug() << "Graph::onVertexMoved: "<< ID;
    this->sortVertexByX();
  }

  void Graph::onVertexDeleted(int ID)
  {
    // qDebug() << "Graph::onVertexDeleted: " << ID;
    controlVertexs.removeAt(ID);
    controlCurves.removeAt(ID);
    controlCurves[ID - 1]->setTo(controlVertexs[ID]);
    controlCurves[ID - 1]->onObjectModify();

    for (int i = ID; i < controlVertexs.size(); i++)
    {
      controlVertexs[i]->ID = i;
    }
    for (int i = ID - 1; i < controlVertexs.size() - 1; i++)
    {
      controlCurves[i]->ID = i;
    }
  }

  void Graph::sortVertexByX()
  {
    bool sorted = false;

    if (controlVertexs.size() == 0) { sorted = true; }

    while (!sorted)
    {
      sorted = true;
      for (int i = 1; i < controlVertexs.size(); i++)
      {
        if (controlVertexs[i]->valueX < controlVertexs[i - 1]->valueX)
        {
          sorted = false;

          controlCurves[i - 2]->setTo(controlVertexs[i]);
          controlCurves[i - 1]->flipEnds();
          controlCurves[i]->setFrom(controlVertexs[i - 1]);
          controlCurves[i - 2]->onObjectModify();
          controlCurves[i]->onObjectModify();

          controlVertexs[i]->ID = i - 1;
          controlVertexs[i - 1]->ID = i;
          controlVertexs.swap(i, i - 1);

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

    // qDebug() << "Graph::makeEnv -> controlPts.size(): " << controlPts.size();
    /*
    for (int i = 0; i < controlPts.size(); i++)
    {
    levels.append(controlPts[i]->valueY);
    qDebug() << "Graph::makeEnv -> i: " << i;
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
    */

    qDebug() << "Graph::makeEnv -> "
      << "levels: " << levels
      << "times: " << times
      << "curves: " << curves;

    emit actEnvGraphChanged(levels, times, curves);
  }

  void Graph::resizeEvent(QResizeEvent *resizeEvent)
  {
    // qDebug() << "Graph::resizeEvent";
    /*
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
    */


    emit actResized(this->boundsGraph().size());
  }

  void Graph::paintEvent(QPaintEvent *event)
  {
    //qDebug() << "Graph::paintEvent";

    QPainter painter(this);

    if (this->hasFocus()) { painter.setPen(QColor(120, 20, 20)); }
    else { painter.setPen(QColor(60, 60, 60)); }
    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, height() - 1, width(), height() - 1);

    /*
    // painter.setPen(QPen(Qt::white, 1));
    // painter.drawRect(this->boundsGraph());
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
    */

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

    //qDebug() << "Graph::paintEvent NEW";
    painterGraphObject = new QPainter(this);
    foreach(GraphAxis *oneAxis, graphVerticalAxis)
    {
      oneAxis->draw(painterGraphObject);
    }
    foreach(GraphAxis *oneAxis, graphHorizontalAxis)
    {
      oneAxis->draw(painterGraphObject);
    }
    foreach(GraphCurve *oneC, controlCurves)
    {
      oneC->draw(painterGraphObject);
    }
    foreach(GraphVertex *oneV, controlVertexs)
    {
      oneV->draw(painterGraphObject);
    }
    //testObj->draw(painterGraphObject);

    graphMouse->draw(painterGraphObject);
  }

  void Graph::mousePressEvent(QMouseEvent *mouseEvent)
  {
    //qDebug() << "Graph::mousePressEvent NEW POINT ADD " << mouse;
    QPoint mouse = mouseEvent->pos();

    GraphVertex *vertex = new GraphVertex(this);
    vertex->setValue(mouse);

    int tempInsertPos = -1;
    if (controlVertexs.size() == 0) { controlVertexs.append(vertex); }
    else
    {
      for (int i = 0; i <= controlVertexs.size(); i++)
      {
        if (i == controlVertexs.size()) {
          controlVertexs.append(vertex);
          break;
        }
        if (controlVertexs[i]->valueX > vertex->valueX)
        {
          vertex->ID = i;
          controlVertexs.insert(i, vertex);

          controlCurves[i - 1]->setTo(vertex);
          GraphCurve *insertCurve = new GraphCurve(this, vertex, controlVertexs[i + 1]);
          insertCurve->ID = i - 1;
          controlCurves.insert(i, insertCurve);

          controlCurves[i - 1]->onObjectModify();
          controlCurves[i]->onObjectModify();

          tempInsertPos = i + 1;
          break;
        }
      }
    }

    if (tempInsertPos != -1)
    {
      for (int i = tempInsertPos; i < controlVertexs.size(); i++)
      {
        controlVertexs[i]->ID = i;
      }
      for (int i = tempInsertPos - 2; i < controlVertexs.size() - 1; i++)
      {
        controlCurves[i]->ID = i;
      }
    }

    //this->makeEnv();
  }

  void Graph::mouseReleaseEvent(QMouseEvent *mouseEvent)
  {
    //update();
    //mouseEvent->accept();
  }

  Graph::~Graph() { }



}