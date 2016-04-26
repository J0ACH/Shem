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
		isOver = false;
		parent->installEventFilter(this);
		this->grabKeyboard();
		pointSize = 10;
		this->setGeometry(pixelX - pointSize / 2, pixelY - pointSize / 2, pointSize, pointSize);
		
		setFocusPolicy(Qt::StrongFocus);
	}

	QRect GraphPoint::bounds() { return QRect(0, 0, width() - 1, height() - 1); }


	void GraphPoint::mousePressEvent(QMouseEvent *mouseEvent)
	{
		mousePressCoor = mouseEvent->pos();
	}
	void GraphPoint::mouseMoveEvent(QMouseEvent * mouseEvent)
	{
		QRect rect = this->geometry();
		rect.moveCenter(QPoint(
			mouseEvent->pos().x(),
			mouseEvent->pos().y()
			//mousePressCoor.x() - mouseEvent->pos().x(),
			//mousePressCoor.y() - mouseEvent->pos().y()
			));
		//this->bounds().moveCenter(mouseEvent->pos());
		this->setGeometry(rect);
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

		painter.fillRect(bounds(), QColor(60, 60, 160));

		if (this->hasFocus()) { painter.setPen(QColor(120, 20, 20)); }
		else { painter.setPen(QColor(60, 60, 60)); }
		painter.drawRect(bounds());

	}

	GraphPoint::~GraphPoint(){}

	// GRAPH POINT END
	///////////////////////////////////////////////////////////
	// GRAPH CURVE
	/*
	GraphCurve::GraphCurve(QWidget *parent, GraphPoint *from, GraphPoint *to) :
	QWidget(parent),
	startPoint(from),
	endPoint(to)
	{
	isOver = false;
	}
	GraphCurve::~GraphCurve() { }
	*/

	// GRAPH CURVE END
	///////////////////////////////////////////////////////////
	// GRAPH 

	Graph::Graph(QWidget *parent) : QWidget(parent)
	{
		this->setMouseTracking(true);
		frameOffset = 50;
		minDomainX = 10;
		maxDomainX = 20;
		minDomainY = 1;
		maxDomainY = 2;
		cursorPos = QPoint();
		collectionPts = QMap<int, GraphPoint*>();
		newPointID = 0;

		setFocusPolicy(Qt::StrongFocus);

	}

	QRect Graph::bounds() { return QRect(0, 0, width(), height()); }
	QRect Graph::boundsGraph()
	{
		return bounds().adjusted(frameOffset, 14, -14, -frameOffset);
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
	double Graph::getDisplayX(double valueX)
	{
		//qDebug() << "point valueX: " << valueX;
		double perc = (valueX - minDomainX) / (double)(maxDomainX - minDomainX);
		//qDebug() << "point percX: " << perc;
		return perc * boundsGraph().width() + boundsGraph().left();
	}
	double Graph::getDisplayY(double valueY)
	{
		//qDebug() << "point valueY: " << valueY;
		double perc = (valueY - minDomainY) / (double)(maxDomainY - minDomainY);
		//double perc = valueY  / (double)maxDomainY;
		//qDebug() << "point percY: " << perc;
		return boundsGraph().height() - (perc * boundsGraph().height()) + boundsGraph().top();
	}

	void Graph::addPixelPoint(int pixelX, int pixelY)
	{
		GraphPoint *pt = new GraphPoint(
			this,
			newPointID,
			pixelX,
			pixelY,
			getValueX(pixelX),
			getValueY(pixelY)
			);
		pt->show();


		//		this->installEventFilter(pt);
		this->connect(pt, SIGNAL(actDelete(int)), this, SLOT(onDeletePoint(int)));

		collectionPts.insert(collectionPts.size(), pt);

		newPointID++;
		//return *pt;
	}
	void Graph::addValuePoint(double valueX, double valueY)
	{
		GraphPoint *pt = new GraphPoint(
			this,
			newPointID,
			getDisplayX(valueX),
			getDisplayY(valueY),
			valueX,
			valueY
			);
		pt->show();
		this->connect(pt, SIGNAL(actDelete(int)), this, SLOT(onDeletePoint(int)));
		collectionPts.insert(collectionPts.size(), pt);
		newPointID++;
	}
	/*
	GraphCurve Graph::addCurve(GraphPoint *from, GraphPoint *to)
	{
	GraphCurve *curve = new GraphCurve(this, from, to);
	return *curve;
	}
	*/
	void Graph::onDeletePoint(int ID)
	{
		qDebug() << "Graph::onDeletePoint: " << QString::number(ID);
		collectionPts.remove(ID);
		update();
	}

	/*
	void Graph::focusInEvent(QFocusEvent * event)
	{
	qDebug() << "Graph::focusInEvent";
	//this->parent()->installEventFilter(this);
	//update();
	}

	void Graph::focusOutEvent(QFocusEvent * event)
	{
	qDebug() << "Graph::focusOutEvent";
	//this->parent()->removeEventFilter(this);
	//update();
	}
	*/

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


		QString posPixel = tr("%1 ; %2").arg(
			QString::number(cursorPos.x()),
			QString::number(cursorPos.y())
			);

		double dValX = this->getValueX(cursorPos.x());
		double dValY = this->getValueY(cursorPos.y());

		QString posValue = tr("%1 ; %2").arg(
			QString::number(dValX, 'f', 2),
			QString::number(dValY, 'f', 2)
			);

		QTextOption option;
		option.setAlignment(Qt::AlignCenter);
		painter.drawText(QRect(cursorPos.x(), cursorPos.y() - 30, 80, 30), posPixel, option);
		painter.drawText(QRect(cursorPos.x(), cursorPos.y() - 20, 80, 30), posValue, option);

		painter.drawText(QRect(10, 10, 50, 30), tr("numPts: %1").arg(collectionPts.size()), option);

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

	}

	void Graph::mouseMoveEvent(QMouseEvent * mouseEvent)
	{
		//cursorPos = mouseEvent->pos();
		//update();
	}

	void Graph::mousePressEvent(QMouseEvent *mouseEvent)
	{
		bool selectPoint = false;
		foreach(GraphPoint *onePoint, collectionPts)
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
		}
		/*
		if (collectionPts.size() > 0)
		{
		//this->addCurve();
		}
		*/

		update();
		//mouseEvent->accept();
	}

	void Graph::mouseReleaseEvent(QMouseEvent *mouseEvent)
	{
		update();
		//mouseEvent->accept();
	}

	Graph::~Graph() { }
}