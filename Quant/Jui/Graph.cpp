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
		this->setGeometry(pixelX - 50, pixelY - 25, 100, 35);
		region = QRegion(
			50 - pointSize / 2,
			25 - pointSize / 2,
			pointSize,
			pointSize,
			QRegion::RegionType::Ellipse
			);
		setMouseTracking(true);
	}

	QRect GraphPoint::bounds() { return QRect(1, 1, width() - 2, height() - 2); }

	void GraphPoint::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		painter.setPen(QPen(Qt::white, 1));
		if (isOver) {
			QPainterPath path;
			path.addRegion(region);
			painter.fillPath(path, Qt::red);

			QString text = tr("%1 || %2").arg(QString::number(valueX, 'f', 2), QString::number(valueY, 'f', 2));
			QTextOption option;
			option.setAlignment(Qt::AlignCenter);
			painter.drawText(QRect(0, 0, 100, 25), text, option);

			painter.drawRect(bounds());
		}
		painter.drawEllipse(QPoint(50, 25), pointSize / 2, pointSize / 2);

	}

	void GraphPoint::mouseMoveEvent(QMouseEvent * mouseEvent)
	{
		/*
		qDebug() << tr("%1 ; %2").arg(
		QString::number(mouseEvent->pos().x()),
		QString::number(mouseEvent->pos().y())
		);
		*/

		if (region.contains(mouseEvent->pos())) { isOver = true; }
		else {
			isOver = false;

		};
		update();
	}

	bool GraphPoint::eventFilter(QObject* target, QEvent* event)
	{
		if (isOver) {
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

					break;

				case Qt::Key::Key_Escape:
					qDebug() << "KeyEvent: Escape PRESSED";
					event->accept();
					return true;
					break;
				}
			}
			/*
			if (event->type() == QEvent::KeyRelease)
			{
				//event->ignore();
				//highlightText(this->toPlainText(), Qt::red);
				//fitTextFormat();
			}
			*/
		}
		else
		{
			//event->ignore();

		}

		return QWidget::eventFilter(target, event);
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
		double perc = (valueY - minDomainY )/ (double)(maxDomainY - minDomainY);
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

	void Graph::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		painter.fillRect(bounds(), QColor(10, 10, 10));
		painter.drawRect(this->boundsGraph());

		painter.setPen(QPen(Qt::white, 1));

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
		cursorPos = mouseEvent->pos();
		update();
	}

	void Graph::mousePressEvent(QMouseEvent *mouseEvent)
	{
		this->addPixelPoint(mouseEvent->pos().x(), mouseEvent->pos().y());
		if (collectionPts.size() > 0)
		{
			//this->addCurve();
		}

		update();
		mouseEvent->accept();
	}

	void Graph::mouseReleaseEvent(QMouseEvent *mouseEvent)
	{
		update();
		mouseEvent->accept();
	}

	Graph::~Graph() { }
}