#include "Graph.h"

namespace Jui
{

	// GRAPH POINT	
	GraphPoint::GraphPoint(QWidget *parent, int pointID) :
		QWidget(parent),
		ID(pointID)
	{
		isOver = false;
		//this->installEventFilter(this);
		this->grabKeyboard();
	}

	QRect GraphPoint::bounds() { return QRect(1, 1, width() - 2, height() - 2); }

	void GraphPoint::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		if (isOver) { painter.fillRect(bounds(), Qt::red); }

		painter.setPen(QPen(Qt::white, 1));
		painter.drawRect(bounds());
	}

	void GraphPoint::enterEvent(QEvent *event)
	{
		qDebug() << "ID: " << ID;
		isOver = true;
		update();
	}

	void GraphPoint::leaveEvent(QEvent *event)
	{
		isOver = false;
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

			if (event->type() == QEvent::KeyRelease)
			{
				//highlightText(this->toPlainText(), Qt::red);
				//fitTextFormat();
			}
		}

		return QWidget::eventFilter(target, event);
	}

	GraphPoint::~GraphPoint(){}

	// GRAPH POINT END
	///////////////////////////////////////////////////////////
	// GRAPH CURVE

	GraphCurve::GraphCurve(QWidget *parent, GraphPoint *from, GraphPoint *to) :
		QWidget(parent),
		startPoint(from),
		endPoint(to)
	{
		isOver = false;
	}
	GraphCurve::~GraphCurve(){}
	// GRAPH CURVE END
	///////////////////////////////////////////////////////////
	// GRAPH 

	Graph::Graph(QWidget *parent) : QWidget(parent)
	{
		this->setMouseTracking(true);
		frameOffset = 50;
		minDomainX = 10;
		maxDomainX = 20;
		minDomainY = 0;
		maxDomainY = 1;
		cursorPos = QPoint();
		collectionPts = QMap<int, GraphPoint*>();
	}

	QRect Graph::bounds() { return QRect(0, 0, width(), height()); }
	QRect Graph::boundsGraph()
	{
		return bounds().adjusted(frameOffset, frameOffset, -frameOffset, -frameOffset);
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

	void Graph::onDeletePoint(int ID)
	{
		qDebug() << "Graph::onDeletePoint: " << QString::number(ID);
		collectionPts.remove(ID);
		update();
	}

	void Graph::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		painter.fillRect(bounds(), QColor(60, 20, 20));
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

		// verticals
		int cntSegX = 5;
		for (int i = 0; i <= cntSegX; i++)
		{
			int pixelX = boundsGraph().width() / cntSegX * i + boundsGraph().left();
			double valX = getValueX(pixelX);
			painter.drawText(
				QRect(pixelX-25, boundsGraph().bottom() + 5, 50, 20),
				QString::number(valX, 'f', 2),
				option
				);
			painter.drawLine(pixelX, boundsGraph().top(), pixelX, boundsGraph().height() + boundsGraph().top());
		};

		// horizontals
		int cntSegY = 5;
		for (int i = 0; i <= cntSegY; i++)
		{
			int pixelY = boundsGraph().height() / cntSegY * i + boundsGraph().top();
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
		GraphPoint *pt = new GraphPoint(this, collectionPts.size());
		pt->setGeometry(cursorPos.x() - 10, cursorPos.y() - 10, 20, 20);
		pt->show();

		this->installEventFilter(pt);
		this->connect(pt, SIGNAL(actDelete(int)), this, SLOT(onDeletePoint(int)));

		collectionPts.insert(collectionPts.size(), pt);

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