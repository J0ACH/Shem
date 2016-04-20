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
	// GRAPH 

	Graph::Graph(QWidget *parent) : QWidget(parent)
	{
		this->setMouseTracking(true);
		cursorPos = QPoint();
		collectionPts = QMap<int, GraphPoint*>();
	}

	QRect Graph::bounds() { return QRect(1, 1, width() - 2, height() - 2); }

	void Graph::onDeletePoint(int ID)
	{
		qDebug() << "Graph::onDeletePoint: "<< QString::number(ID);
		collectionPts.remove(ID);
		update();
	}

	void Graph::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		painter.fillRect(bounds(), QColor(120, 30, 30));

		painter.setPen(QPen(Qt::white, 1));

		QString pos = tr("%1 ; %2").arg(
			QString::number(cursorPos.x()),
			QString::number(cursorPos.y())
			);

		QTextOption option;
		option.setAlignment(Qt::AlignCenter);
		painter.drawText(QRect(cursorPos.x(), cursorPos.y(), 50, 30), pos, option);
		painter.drawRect(bounds());

		painter.drawText(QRect(10, 10, 50, 30), tr("numPts: %1").arg(collectionPts.size()), option);

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