#include "Edges.h"

namespace Jui
{

	Edges::Edges(QWidget *parent) : QObject(parent)
	{
		collParent = parent;
		qDebug("Edges init...");

		setObjectName("Edges");

		int newPrecision = 1;
		qDebug("Class(%s) -> parent(%s)",
			qPrintable(objectName()),
			qPrintable(this->parent()->objectName())
			);

		this->parent()->installEventFilter(this);

		setEdgeOffset(10);

		collEdges = new QVector<EdgeControler*>();
		//collRect = new QVector<QRect>();

		this->addManipulator(EdgePosition::LEFT);
		this->addManipulator(EdgePosition::RIGHT);

	}

	void Edges::addManipulator(EdgePosition direction)
	{

		EdgeControler *manipulator = new EdgeControler(collParent);
		manipulator->setParentObject(this);
		//manipulator->setGeometry(0, 0, 150, 150);
		manipulator->setDirection(direction);
		manipulator->setEdgeOffset(edgeOffset);
		collEdges->append(manipulator);


	}


	void Edges::setEdgeOffset(int offset) { edgeOffset = offset; }

	void Edges::edgeMoved(EdgePosition direction)
	{

		switch (direction)
		{
		case EdgePosition::LEFT:
			qDebug("Slot EdgeMoved (%s) recived...", "LEFT");
			break;
		case EdgePosition::TOP:
			break;
		case EdgePosition::RIGHT:
			qDebug("Slot EdgeMoved (%s) recived...", "RIGHT");
			break;
		case EdgePosition::BOTTOM:
			break;
		case EdgePosition::ALL:
			break;
		default:
			qDebug("Slot EdgeMoved (%s) recived...", "SOME");
			break;
		}

	}


	/*
	bool Edges::eventFilter(QObject *obj, QEvent *event)
	{

	if (event->type() == QEvent::Paint) {
	//QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
	qDebug("%s paint event", qPrintable(objectName()));
	//	return true;
	QPaintEvent *pevent = static_cast<QPaintEvent*>(event);
	//QWidget* widget = static_cast<QWidget*>(obj);
	QPainter painter(testWidget);

	//QPainter painter(paintEvent);
	painter.fillRect(QRect(0, 0, 30, 30), QColor(255, 30, 30));

	return QObject::eventFilter(obj, event);
	}
	else {
	// standard event processing
	return QObject::eventFilter(obj, event);
	}

	qDebug("%s event filter", qPrintable(objectName()));
	return QObject::eventFilter(obj, event);
	}
	*/


	Edges::~Edges()
	{

	}
}