#include "Edges.h"

namespace Jui
{

	Edges::Edges(QWidget *parentObject) : QObject(parentObject)
	{
		parent = parentObject;
		//qDebug("Edges init...");

		setObjectName("Edges");

		qDebug("Class(%s) -> parent(%s)",
			qPrintable(objectName()),
			qPrintable(parent->objectName())
			);

		parent->installEventFilter(this);

		if (parent->objectName() == "Canvan")
		{
			this->parentType = WIN;
		}
		else
		{
			this->parentType = WIDGET;
		};

		this->setEdgeOffset(10);

		this->addManipulator(EdgeControler::LEFT);
		this->addManipulator(EdgeControler::TOP);
		this->addManipulator(EdgeControler::RIGHT);
		this->addManipulator(EdgeControler::BOTTOM);
	}

	void Edges::addManipulator(EdgeControler::Direction direction)
	{
		EdgeControler *manipulator = new EdgeControler(parent);
		manipulator->setParentObject(this);
		manipulator->setDirection(direction);
		manipulator->setEdgeOffset(edgeOffset);

		connect(parent, SIGNAL(resizeAct()), manipulator, SLOT(fitGeometry()));
	}

	void Edges::setEdgeOffset(int offset) { edgeOffset = offset; }

	void Edges::edgeMoved(EdgeControler::Direction direction, int newValue)
	{
		QRectF originalGeometry = parent->geometry();
		//	qDebug() << "Slot EdgeMoved " << originalGeometry;

		QPoint frameOriginGlobal = parent->mapToGlobal(QPoint(0, 0));
		//	qDebug() << "Slot EdgeMoved " << frameOriginGlobal;

		switch (direction)
		{
		case EdgeControler::LEFT:
			//	qDebug("Slot EdgeMoved (%s) recived... [gMouse:%i]", "LEFT", newValue);
			parent->move(newValue - frameOriginGlobal.x(), originalGeometry.y());

			break;
		case EdgeControler::TOP:
			break;
		case EdgeControler::RIGHT:
			// OK
			//	qDebug("Slot EdgeMoved (%s) recived... [gMouse:%i]", "RIGHT", newValue);
			parent->setFixedWidth(newValue - frameOriginGlobal.x());
			if (parentType == WIN)
			{
				QRect winRect = parent->parentWidget()->geometry();
				//	qDebug() << "Rect" << parent->parentWidget()->geometry();
				parent->parentWidget()->setGeometry(
					winRect.left(),
					winRect.top(),
					parent->width(),
					winRect.height()
					);
			};
			break;
		case EdgeControler::BOTTOM:
			// OK
			parent->setFixedHeight(newValue - frameOriginGlobal.y());
			if (parentType == WIN)
			{
				QRect winRect = parent->parentWidget()->geometry();
				parent->parentWidget()->setGeometry(
					winRect.left(),
					winRect.top(),
					winRect.width(),
					parent->height()
					);
			};
			break;
		case EdgeControler::ALL:
			break;
		default:
			//	qDebug("Slot EdgeMoved (%s) recived...", "SOME");
			break;
		}

	}

	Edges::~Edges()
	{

	}
}