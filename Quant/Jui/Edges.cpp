#include "Edges.h"

namespace Jui
{

	Edges::Edges(QWidget *parent) : QObject(parent)
	{
		widgetParent = parent;
		//qDebug("Edges init...");

		setObjectName("Edges");

		qDebug("Class(%s) -> parent(%s)",
			qPrintable(objectName()),
			qPrintable(this->parent()->objectName())
			);

		this->parent()->installEventFilter(this);

		if (this->parent()->objectName() == "Canvan")
		{
			parentType = WIN;
			qDebug("Canvan class found");
		}
		else
		{
			parentType = WIDGET;
		};

		setEdgeOffset(10);

		//collEdges = new QVector<EdgeControler*>();
		//collRect = new QVector<QRect>();

		this->addManipulator(EdgeControler::LEFT);
		this->addManipulator(EdgeControler::TOP);
		this->addManipulator(EdgeControler::RIGHT);
		this->addManipulator(EdgeControler::BOTTOM);

		connect(widgetParent, SIGNAL(resizeAct()), this, SLOT(parentResized()));
	}

	void Edges::addManipulator(EdgeControler::Direction direction)
	{
		EdgeControler *manipulator = new EdgeControler(widgetParent);
		manipulator->setParentObject(this);
		//manipulator->setGeometry(0, 0, 150, 150);
		manipulator->setDirection(direction);
		manipulator->setEdgeOffset(edgeOffset);
		//collEdges->append(manipulator);

		connect(widgetParent, SIGNAL(resizeAct()), manipulator, SLOT(fitGeometry()));
	}


	void Edges::setEdgeOffset(int offset) { edgeOffset = offset; }

	void Edges::edgeMoved(EdgeControler::Direction direction, int newValue)
	{
		QRectF originalGeometry = widgetParent->geometry();
	//	qDebug() << "Slot EdgeMoved " << originalGeometry;

		QPoint frameOriginGlobal = widgetParent->mapToGlobal(QPoint(0, 0));
	//	qDebug() << "Slot EdgeMoved " << frameOriginGlobal;

		switch (direction)
		{
		case EdgeControler::LEFT:
		//	qDebug("Slot EdgeMoved (%s) recived... [gMouse:%i]", "LEFT", newValue);
			widgetParent->move(newValue - frameOriginGlobal.x(), originalGeometry.y());
			break;
		case EdgeControler::TOP:
			break;
		case EdgeControler::RIGHT:
			// OK
		//	qDebug("Slot EdgeMoved (%s) recived... [gMouse:%i]", "RIGHT", newValue);
			widgetParent->setFixedWidth(newValue - frameOriginGlobal.x());
			break;
		case EdgeControler::BOTTOM:
			// OK
			widgetParent->setFixedHeight(newValue - frameOriginGlobal.y());
			break;
		case EdgeControler::ALL:
			break;
		default:
		//	qDebug("Slot EdgeMoved (%s) recived...", "SOME");
			break;
		}


		//widgetParent->update();
	}

	void Edges::parentResized()
	{
		qDebug("Edges:parentResized slot....");
	}

	Edges::~Edges()
	{

	}
}