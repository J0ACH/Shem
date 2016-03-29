#include "Edges.h"

namespace Jui
{

	Edges::Edges(QWidget *parent) : QObject(parent)
	{
		widgetParent = parent;
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
		EdgeControler *manipulator = new EdgeControler(widgetParent);
		manipulator->setParentObject(this);
		//manipulator->setGeometry(0, 0, 150, 150);
		manipulator->setDirection(direction);
		manipulator->setEdgeOffset(edgeOffset);
		collEdges->append(manipulator);
	}


	void Edges::setEdgeOffset(int offset) { edgeOffset = offset; }

	void Edges::edgeMoved(EdgePosition direction, int newValue)
	{
		QRectF originalGeometry = widgetParent->geometry();
		qDebug() << "Slot EdgeMoved " << originalGeometry;

		QPoint frameOriginGlobal = widgetParent->mapToGlobal(QPoint(0, 0));
		qDebug() << "Slot EdgeMoved " << frameOriginGlobal;

		switch (direction)
		{
		case EdgePosition::LEFT:
			qDebug("Slot EdgeMoved (%s) recived... [gMouse:%i]", "LEFT", newValue);
			break;
		case EdgePosition::TOP:
			break;
		case EdgePosition::RIGHT:
			qDebug("Slot EdgeMoved (%s) recived... [gMouse:%i]", "RIGHT", newValue);
			widgetParent->setFixedWidth(newValue - frameOriginGlobal.x());
			break;
		case EdgePosition::BOTTOM:
			break;
		case EdgePosition::ALL:
			break;
		default:
			qDebug("Slot EdgeMoved (%s) recived...", "SOME");
			break;
		}

		//widgetParent->update();
		//widgetParent->setGeometry(10, 10, 200, 200);

	}


	Edges::~Edges()
	{

	}
}