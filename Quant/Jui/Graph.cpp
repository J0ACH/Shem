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
    //  qDebug() << "GraphObject::onDomainChanged domX" << domX << "domY" << domY;
    domainX = domX;
    domainY = domY;
    emit actModify();
  }

  void GraphObject::onGraphResized(QSize newSize)
  {
    //  qDebug() << "GraphObject::onGraphResized newSize" << newSize;
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
  QPoint GraphObject::getPixel(QPointF valuePoint)
  {
    valueX = valuePoint.x();
    valueY = valuePoint.y();
    return this->getPixel();
  }

  float GraphObject::getDomainX_min()  { return this->domainX.first; }
  float GraphObject::getDomainX_max()  { return this->domainX.second; }
  float GraphObject::getDomainY_min()  { return this->domainY.first; }
  float GraphObject::getDomainY_max()  { return this->domainY.second; }

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
    Graph *parentGraph = qobject_cast<Graph*>(parent);

    type = CurveType::lin;
    ID = -1;
    pixelWidth = to->getPixel().x() - from->getPixel().x();
    isMouseInDomain = false;

    cBoxType = new ControlBox(parent);
    cBoxType->setLabelSize(15);
    cBoxType->setLabel("crv");
    cBoxType->setValue("lin");
    cBoxType->hide();

    connect(from, SIGNAL(actModify()), this, SLOT(onObjectModify()));
    connect(to, SIGNAL(actModify()), this, SLOT(onObjectModify()));
    connect(cBoxType, SIGNAL(actValueChanged(QString)), this, SLOT(onCBoxTypeChanged(QString)));
    connect(this, SIGNAL(actTypeChanged(int)), parentGraph, SLOT(onCurveTypeChanged(int)));
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
    case CurveType::welch:
      cBoxType->setValue("welch");
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
    if (from->valueX > valX) {
      isMouseInDomain = false;
      cBoxType->hide();
      return false;
    }
    if (to->valueX < valX) {
      isMouseInDomain = false;
      cBoxType->hide();
      return false;
    }
    cBoxType->show();
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
        valAt = (from->valueY - to->valueY)*qPow((1 - percX), 2) + to->valueY;
      }
      break;

    case CurveType::sin:
      if (from->valueY < to->valueY)
      {
        domFrom = 3 * M_PI_2;
        domTo = 5 * M_PI_2;
        percX = (domTo - domFrom) * percX + domFrom;
        valAt = ((to->valueY - from->valueY)*qSin(percX) + (to->valueY - from->valueY)) / 2 + from->valueY;
      }
      else
      {
        domFrom = M_PI_2;
        domTo = 3 * M_PI_2;
        percX = (domTo - domFrom) * percX + domFrom;
        valAt = ((from->valueY - to->valueY)*qSin(percX) + (from->valueY - to->valueY)) / 2 + to->valueY;
      }
      break;

    case CurveType::welch:

      if (from->valueY < to->valueY)
      {
        valAt = (from->valueY - to->valueY)*qPow((1 - percX), 2) + to->valueY;
      }
      else
      {
        valAt = (to->valueY - from->valueY)*qPow(percX, 2) + from->valueY;
      }

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
    //qDebug() << "GraphCurve::onObjectModify type: " << type;

    GraphObject::onObjectModify();

    QPoint fromPt = from->getPixel();
    QPoint toPt = to->getPixel();
    pixelWidth = toPt.x() - fromPt.x();
    int cntSeg = 20;
    qreal segStep = (to->valueX - from->valueX) / cntSeg;//  pixelWidth;

    polygon = QPolygon();
    polygon.append(fromPt);
    switch (type)
    {
    case step:
      polygon.append(QPoint(fromPt.x(), toPt.y()));
      break;

    default:
    case lin:
      break;

    case exp:
      for (float xCoor = from->valueX + segStep; xCoor < to->valueX; xCoor += segStep)
      {
        float yCoor = this->at(xCoor);
        polygon.append(this->getPixel(QPointF(xCoor, yCoor)));
      }
      break;

    case sin:
      for (float xCoor = from->valueX + segStep; xCoor < to->valueX; xCoor += segStep)
      {
        float yCoor = this->at(xCoor);
        polygon.append(this->getPixel(QPointF(xCoor, yCoor)));
      }
      break;

    case welch:
      for (float xCoor = from->valueX + segStep; xCoor < to->valueX; xCoor += segStep)
      {
        float yCoor = this->at(xCoor);
        polygon.append(this->getPixel(QPointF(xCoor, yCoor)));
      }
      break;

    case hold:
      polygon.append(QPoint(toPt.x(), fromPt.y()));
      break;
    }

    polygon.append(toPt);
  }

  void GraphCurve::onCBoxTypeChanged(QString txtType)
  {
    if (txtType == "step") { this->setType(CurveType::step); }
    else if (txtType == "exp") { this->setType(CurveType::exp); }
    else if (txtType == "sin") { this->setType(CurveType::sin); }
    else if (txtType == "welch") { this->setType(CurveType::welch); }
    else if (txtType == "hold") { this->setType(CurveType::hold); }
    else { this->setType(CurveType::lin); }

    this->onObjectModify();

    emit actTypeChanged(ID);
  }

  void GraphCurve::draw(QPainter *painter)
  {
    QPoint fromPt = from->getPixel();
    QPoint toPt = to->getPixel();
    QPoint center;
    center.setX((toPt.x() + fromPt.x()) / 2);
    center.setY((toPt.y() + fromPt.y()) / 2);

    QRect rectBoxType(center.x() - 20, center.y() - 8, 50, 16);
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

  GraphCurve::~GraphCurve()
  {
    qDebug() << "Curve ~DELETED" << ID;
    cBoxType->close();
  };

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

      //}
      return true;
      break;
    }
    return QObject::eventFilter(target, event);
  }

  void GraphMouse::draw(QPainter *painter)
  {
    if (valueY < this->getDomainY_max() && valueY > this->getDomainY_min()) {
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
  }
  GraphMouse::~GraphMouse(){}

  // GRAPH MOUSE END
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

    //  graphPolylines = new QPolygonF();

    setFocusPolicy(Qt::StrongFocus);

    graphMouse = new GraphMouse(this); // musi byt zadan pred vertexema

    testVertex1 = new GraphVertex(this);
    testVertex1->valueX = 0.0;
    testVertex1->valueY = 0.75;
    testVertex1->ID = 0;
    testVertex1->setType(VertexType::startPoint);
    controlVertexs.append(testVertex1);

    testVertex2 = new GraphVertex(this);
    testVertex2->valueX = 0.55;
    testVertex2->valueY = 0.15;
    testVertex2->ID = 1;
    controlVertexs.append(testVertex2);

    testVertex3 = new GraphVertex(this);
    testVertex3->valueX = 0.75;
    testVertex3->valueY = 0.85;
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
    testCurve1->setType(CurveType::exp);
    controlCurves.append(testCurve1);
    testCurve2 = new GraphCurve(this, testVertex2, testVertex3);
    testCurve2->ID = 1;
    testCurve2->setType(CurveType::sin);
    controlCurves.append(testCurve2);
    testCurve3 = new GraphCurve(this, testVertex3, testVertex4);
    testCurve3->ID = 2;
    testCurve3->setType(CurveType::welch);
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
    if (controlVertexs[ID]->valueY < domainY.first) { controlVertexs[ID]->valueY = domainY.first; }
    if (controlVertexs[ID]->valueY > domainY.second) { controlVertexs[ID]->valueY = domainY.second; }
    // qDebug() << "Graph::onVertexMoved: "<< ID;
    this->sortVertexByX();
  }

  void Graph::onVertexDeleted(int ID)
  {
    //     qDebug() << "Graph::onVertexDeleted: " << ID;
    controlVertexs.removeAt(ID);
    for (int i = ID; i < controlVertexs.size(); i++)
    {
      controlVertexs[i]->ID = i;
    }

    controlCurves[ID - 1]->setTo(controlVertexs[ID]);
    controlCurves[ID - 1]->onObjectModify();
    controlCurves[ID]->deleteLater();
    controlCurves.removeAt(ID);
    for (int i = ID - 1; i < controlVertexs.size() - 1; i++)
    {
      controlCurves[i]->ID = i;
    }
    this->makeEnv();
  }

  void Graph::onCurveTypeChanged(int ID)
  {
    this->makeEnv();    
   // graphMouse->event(new QEvent(QEvent::MouseMove));    
   // graphMouse->eventFilter(this, new QEvent(QEvent::MouseMove));
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

    for (int i = 0; i < controlVertexs.size(); i++)
    {
      levels.append(controlVertexs[i]->valueY);
      //qDebug() << "Graph::makeEnv -> i: " << i;
      if (i != 0)
      {
        double previousTime = 0;
        foreach(GraphVertex *onePt, controlVertexs)
        {
          if (onePt->valueX < controlVertexs[i]->valueX)
          {
            if (onePt->valueX > previousTime)
            {
              previousTime = onePt->valueX;
            }
          }
        }
        times.append(controlVertexs[i]->valueX - previousTime);
      }
    };

    foreach(GraphCurve *oneCrv, controlCurves)
    {
      switch (oneCrv->type)
      {
      case step:
        curves.append("'step'");
        break;
      case lin:
        curves.append("'lin'");
        break;
      case exp:
        curves.append("'exp'");
        break;
      case sin:
        curves.append("'sin'");
        break;
      case welch:
        curves.append("'welch'");
        break;
      case hold:
        curves.append("'hold'");
        break;
      }
    };

    qDebug() << "Graph::makeEnv -> "
      << "levels: " << levels
      << "times: " << times
      << "curves: " << curves;

    emit actEnvGraphChanged(levels, times, curves);
  }

  void Graph::onEnvChanged(QList<double> levels, QList<double> times, QList<QString> curves)
  {
    qDebug() << "Graph::onEnvChanged -> "
      << "levels: " << levels
      << "times: " << times
      << "curves: " << curves;

    double previousTime = 0.0;
    foreach(double oneT, times)
    {
      oneT += previousTime;
      previousTime = oneT;
      qDebug() << "Graph::oneT " << oneT << "previousTime" << previousTime;
    }
    qDebug() << "Graph::times " << times;


    for (int i = 0; i < levels.size(); i++)
    {
      if (i < controlVertexs.size())
      {
        qDebug() << "Graph::onEnvChanged -> set ID " << i;
      }
      else
      {
        qDebug() << "Graph::onEnvChanged -> add ID " << i;
      }
    }
  }

  void Graph::resizeEvent(QResizeEvent *resizeEvent)
  {
    emit actResized(this->boundsGraph().size());

    foreach(GraphCurve *oneC, controlCurves)
    {
      oneC->onObjectModify();
    }
  }

  void Graph::paintEvent(QPaintEvent *event)
  {
    //qDebug() << "Graph::paintEvent";

    QPainter painter(this);

    if (this->hasFocus()) { painter.setPen(QColor(120, 20, 20)); }
    else { painter.setPen(QColor(60, 60, 60)); }
    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, height() - 1, width(), height() - 1);

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
          insertCurve->setType(controlCurves[i - 1]->type);
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
  }

  void Graph::mouseReleaseEvent(QMouseEvent *mouseEvent)
  {
    this->makeEnv();
  }

  Graph::~Graph() { }

}