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
		this->setGeometry(pixelX - pointSize / 2, pixelY - pointSize / 2, pointSize + 1, pointSize + 1);

		mousePressCoor = QPoint(0, 0);

		setFocusPolicy(Qt::StrongFocus);
	}

	QRect GraphPoint::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

	void GraphPoint::mousePressEvent(QMouseEvent *mouseEvent)
	{
		mousePressCoor = mouseEvent->pos();
		mouseGlobalCoor = mouseEvent->globalPos();
	}

	void GraphPoint::mouseMoveEvent(QMouseEvent *mouseEvent)
	{
		QPoint pos = this->parentWidget()->mapFromGlobal(mouseEvent->globalPos());

		this->setGeometry(
			pos.x() - mousePressCoor.x(),
			pos.y() - mousePressCoor.y(),
			this->width(),
			this->height()
			);
	}

	void GraphPoint::mouseReleaseEvent(QMouseEvent *mouseEvent)
	{
		int deltaX = mouseEvent->globalPos().x() - mouseGlobalCoor.x();
		int deltaY = mouseEvent->globalPos().y() - mouseGlobalCoor.y();
		qDebug() << "point ID: " << ID << "dX: " << deltaX << "dY: " << -deltaY;
		pixelX += deltaX;
		pixelY += deltaY;
		emit actMoved(ID, pixelX, pixelY);
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
					qDebug() << "event: DELTE PRESSED";

					emit actDelete(ID);
					this->close();
					event->accept();
					return true;
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
		else
		{
			painter.setPen(QColor(Qt::white));
			this->setGeometry(
				pixelX - pointSize / 2,
				pixelY - pointSize / 2,
				pointSize + 1,
				pointSize + 1
				);
		}

		painter.drawEllipse(bounds());
	}

	GraphPoint::~GraphPoint(){}


	// GRAPH POINT END
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
		//cursorPos = QPoint();
		controlPts = QMap<int, GraphPoint*>();
		graphPolylines = new QPolygonF();
		graphValues = QList<double>();

		//newPointID = 0;

		setFocusPolicy(Qt::StrongFocus);

	}

	QRect Graph::bounds() { return QRect(0, 0, width(), height()); }
	QRect Graph::boundsGraph()
	{
		return bounds().adjusted(frameOffset * 2, 14, -14, -frameOffset);
	}

	void Graph::setDomainX(int min, int max)
	{
		minDomainX = min;
		maxDomainX = max;
		update();
	}
	void Graph::setDomainY(int min, int max)
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

	/*
	void Graph::setLevels(QList<double> levels)
	{
	qDebug() << "Graph::setLevels: " << levels;
	qDebug() << "Graph::controlPts.size: " << controlPts.size();

	if (levels.size() != controlPts.values().size())
	{
	qDebug() << "Graph::setLevels NEW PTS";
	this->deleteGraph();
	for each (double oneVal in levels)	{ this->addValuePoint(0, oneVal); }
	}

	int cnt = 0;
	for each (GraphPoint *onePt in controlPts.values())
	{
	this->disconnect(onePt, SIGNAL(actMoved(double, int, int)), this, SLOT(onMovePoint(double, int, int)));
	onePt->valueY = levels[cnt];
	onePt->pixelY = getPixelY(levels[cnt]);
	qDebug() << "Graph::setLevels: " << onePt->valueX << " || " << onePt->valueY;
	this->connect(onePt, SIGNAL(actMoved(double, int, int)), this, SLOT(onMovePoint(double, int, int)));
	//onePt->update();
	cnt++;
	}
	qDebug() << "Graph::controlPts.size: " << controlPts.size();
	update();
	}
	QList<double> Graph::getLevels()
	{
	QList<double> answ;
	for each (GraphPoint *onePt in controlPts.values()) { answ.append(onePt->valueY); }
	return answ;
	}
	void Graph::setTimes(QList<double> times)
	{
	qDebug() << "Graph::setTimes: " << times;
	qDebug() << "Graph::controlPts.size: " << controlPts.size();
	int cnt = 0;
	double currentTime = 0;
	times.prepend(0);

	if (times.size() != controlPts.values().size())
	{
	qDebug() << "Graph::setTimes NEW PTS";
	this->deleteGraph();
	//this->addValuePoint(this->getDomainX()[0], 0);
	for each (double oneVal in times) { this->addValuePoint(oneVal, 0); }
	}
	qDebug() << "Graph::controlPts.size: " << controlPts.size();

	for each (GraphPoint *onePt in controlPts.values())
	{
	this->disconnect(onePt, SIGNAL(actMoved(double, int, int)), this, SLOT(onMovePoint(double, int, int)));
	currentTime += times[cnt];
	//qDebug() << "Graph::currentTime: " << currentTime;
	onePt->valueX = currentTime;
	onePt->pixelX = getPixelX(times[cnt]);
	qDebug() << "Graph::setTimes: " << onePt->valueX << " || " << onePt->valueY;
	drawLine(currentTime, 0, currentTime, 1);
	//onePt->update();
	this->connect(onePt, SIGNAL(actMoved(double, int, int)), this, SLOT(onMovePoint(double, int, int)));
	cnt++;
	}
	qDebug() << "Graph::controlPts.size: " << controlPts.size();
	update();
	}
	QList<double> Graph::getTimes()
	{
	QList<double> answ;
	for each (GraphPoint *onePt in controlPts.values()) { answ.append(onePt->valueX); }
	return answ;
	}
	void Graph::setCurves(QList<double> levels) { }
	*/

	void Graph::addPixelPoint(int pixelX, int pixelY)
	{
		GraphPoint *pt = new GraphPoint(
			this,
			controlPts.size(),
			pixelX,
			pixelY,
			getValueX(pixelX),
			getValueY(pixelY)
			);
		pt->show();

		this->connect(pt, SIGNAL(actDelete(int)), this, SLOT(onDeletePoint(double)));
		this->connect(pt, SIGNAL(actMoved(int, int, int)), this, SLOT(onMovePoint(int, int, int)));

		//controlPts.insert(getValueX(pixelX), pt);
		controlPts.insert(controlPts.size(), pt);
		//newPointID++;
	}
	void Graph::addValuePoint(double valueX, double valueY)
	{
		GraphPoint *pt = new GraphPoint(
			this,
			controlPts.size(),
			getPixelX(valueX),
			getPixelY(valueY),
			valueX,
			valueY
			);
		pt->show();
		this->connect(pt, SIGNAL(actDelete(int)), this, SLOT(onDeletePoint(int)));
		this->connect(pt, SIGNAL(actMoved(int, int, int)), this, SLOT(onMovePoint(int, int, int)));

		//controlPts.insert(valueX, pt);
		controlPts.insert(controlPts.size(), pt);
		//newPointID++;
	}

	void Graph::drawPoint(double valueX, double valueY)
	{
		//collDrawPoints.append(new QPointF(valueX, valueY));
		this->addValuePoint(valueX, valueY);
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
		//graphPolylines.append(polygon);
		update();
	}

	void Graph::deleteGraph()
	{
		//newPointID = 0;
		for each(GraphPoint *onePoint in controlPts.values()) { onePoint->close(); }
		controlPts = QMap<int, GraphPoint*>();
		graphPolylines = new QPolygonF();
		graphValues = QList<double>();

		//collDrawPoints = QList<QPointF*>();
		//collDrawLines = QList<QLineF*>();

		update();
	}

	void Graph::onDeletePoint(int ID)
	{
		qDebug() << "Graph::onDeletePoint: " << QString::number(ID);
		controlPts.remove(ID);
		update();
	}
	void Graph::onMovePoint(int ID, int pixelX, int pixelY)
	{
		qDebug() << "Graph::onMovePoint: " << QString::number(ID);
		controlPts.value(ID)->valueX = getValueX(pixelX);
		controlPts.value(ID)->valueY = getValueY(pixelY);
		/*
		GraphPoint *tempPt = controlPts.value(ID);
		tempPt->ID = getValueX(pixelX);
		tempPt->valueX = getValueX(pixelX);
		tempPt->valueY = getValueY(pixelY);
		*/

		//controlPts.remove(ID);
		//controlPts.insert(getValueX(pixelX), tempPt);

		this->makeEnv();
	}

	void Graph::onGraphEnv(QList<double> envLevels, QList<double> envTimes, QList<double> envCurves)
	{
		qDebug() << "Graph::onGraphEnv";
		qDebug() << "levels: " << envLevels;
		qDebug() << "times: " << envTimes;
		qDebug() << "curves: " << envCurves;

		this->deleteGraph();
		for (int i = 0; i < envLevels.size(); i++)
		{
			this->addValuePoint(envTimes[i], envLevels[i]);
		}
	}


	void Graph::makeEnv()
	{
		qDebug() << "Graph::makeEnv";
		QList<double> levels;
		QList<double> times;
		QList<double> curves;

		for (int i = 0; i < controlPts.values().size(); i++)
		{
			qDebug()
				<< "ID: " << controlPts.values()[i]->ID
				<< " time: " << controlPts.values()[i]->valueX;
		}

		for (int i = 0; i < controlPts.values().size(); i++)
		{
			levels.append(controlPts.values()[i]->valueY);

			if (i != 0)
			{
				double previousTime = 0;
				for each (GraphPoint *onePt in controlPts.values())
				{
					if (onePt->valueX < controlPts.values()[i]->valueX)
					{
						if (onePt->valueX > previousTime)
						{
							previousTime = onePt->valueX;
						}
					}
				}

				//qDebug() << "previousTime:" << previousTime;
				//qDebug() << "collectionPts.values()[i]->valueX - previousTime" << controlPts.values()[i]->valueX - previousTime;
				times.append(controlPts.values()[i]->valueX - previousTime);
				curves.append(-2);
			}
			//currentTime += controlPts.values()[i]->valueX;
		}

		//qDebug() << "Graph::actGraphEnv";
		emit actGraphEnv(levels, times, curves);
	}

	void Graph::resizeEvent(QResizeEvent *resizeEvent)
	{
		//qDebug() << "Graph::resizeEvent";

		graphValues = QList<double>();
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

		painter.drawText(QRect(10, 10, 50, 30), tr("numPts: %1").arg(controlPts.size()), option);

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
		painter.setPen(QColor(70, 70, 170));
		for each(QPointF *onePoint in collDrawPoints)
		{
			painter.drawEllipse(getPixelX(onePoint->x()) - 5, getPixelY(onePoint->y()) - 5, 10, 10);
		};

		// collDrawLines
		painter.setPen(QColor(70, 170, 70));
		for each(QLineF *oneLine in collDrawLines)
		{
			QPointF pt1 = QPoint(getPixelX(oneLine->p1().x()), getPixelY(oneLine->p1().y()));
			QPointF pt2 = QPoint(getPixelX(oneLine->p2().x()), getPixelY(oneLine->p2().y()));
			painter.drawLine(pt1, pt2);
		};

		//collPolygons
		painter.setPen(QColor(240, 70, 70));
		QPolygonF poly;
		for each(QPointF onePt in graphPolylines->toList())
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
		bool selectPoint = false;
		foreach(GraphPoint *onePoint, controlPts)
		{
			if (onePoint->hasFocus())
			{
				selectPoint = true;
				break;
			}
		}
		if (!selectPoint)
		{
			this->addPixelPoint(mouseEvent->pos().x(), mouseEvent->pos().y());

			this->makeEnv();
			//void actPointAdded(double valueX, double valueY);
		}

		update();
		//mouseEvent->accept();
	}

	void Graph::mouseReleaseEvent(QMouseEvent *mouseEvent)
	{
		//update();
		//mouseEvent->accept();
	}

	Graph::~Graph() { }
}