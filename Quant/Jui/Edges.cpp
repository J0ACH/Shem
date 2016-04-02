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

	void Edges::edgePressed()
	{
		mEdgePressActGrandParentOriginGlobal = parent->parentWidget()->mapToGlobal(QPoint(0, 0));
		qDebug() << "EdgePressed grandParent originGlobal " << mEdgePressActGrandParentOriginGlobal;
	}

	void Edges::edgeMoved(EdgeControler::Direction direction, int newValue)
	{
		QRect originalGeometry = parent->geometry();

		QPoint frameOriginGlobal = parent->mapToGlobal(QPoint(0, 0));

		QPoint parentPoint;
		QRect newRect;


		switch (direction)
		{
		case EdgeControler::LEFT:
			originalGeometry.setLeft(newValue - mEdgePressActGrandParentOriginGlobal.x());
			qDebug() << "NewValue X: " << newValue - mEdgePressActGrandParentOriginGlobal.x();
			parent->setGeometry(originalGeometry);
			break;

		case EdgeControler::TOP:
			originalGeometry.setTop(newValue - mEdgePressActGrandParentOriginGlobal.y());
			qDebug() << "NewValue Y: " << newValue - mEdgePressActGrandParentOriginGlobal.y();
			parent->setGeometry(originalGeometry);
			break;

		case EdgeControler::RIGHT:
			originalGeometry.setRight(newValue - mEdgePressActGrandParentOriginGlobal.x());
			qDebug() << "NewValue X: " << newValue - mEdgePressActGrandParentOriginGlobal.x();
			parent->setGeometry(originalGeometry);

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
			originalGeometry.setBottom(newValue - mEdgePressActGrandParentOriginGlobal.y());
			qDebug() << "NewValue Y: " << newValue - mEdgePressActGrandParentOriginGlobal.y();
			parent->setGeometry(originalGeometry);

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

		qDebug() << "Rect" << parent->geometry();

	}

	Edges::~Edges()
	{

	}
}