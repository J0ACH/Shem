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
		mEdgePressActParentOriginGlobal = parent->parentWidget()->mapToGlobal(QPoint(0, 0));
		qDebug() << "EdgePressed parent originGlobal " << mEdgePressActParentOriginGlobal;
	}

	void Edges::edgeMoved(EdgeControler::Direction direction, int newValue)
	{
		

		QRect originalGeometry = parent->geometry();
		
		//	qDebug() << "Slot EdgeMoved " << originalGeometry;

		QPoint frameOriginGlobal = parent->mapToGlobal(QPoint(0, 0));
		//QPoint frameOriginGlobal = parent->mapToGlobal(QPoint(0, 0));

		//	qDebug() << "Slot EdgeMoved " << frameOriginGlobal;

		QPoint parentPoint;
		QRect newRect;
		//int newX;

		switch (direction)
		{
		case EdgeControler::LEFT:
			qDebug() << "NewValue X: " << newValue;

			//	qDebug("Slot EdgeMoved (%s) recived... [gMouse:%i]", "LEFT", newValue);
			//parentPoint = parent->mapFrom(parent, QPoint(0, 0));
			//newX = newValue - originalGeometry.left();
			originalGeometry.setLeft(newValue - mEdgePressActParentOriginGlobal.x());
			qDebug() << "originalGeometry.left()" << originalGeometry.left();	
			qDebug() << "mPressActParentOrigin" << mEdgePressActParentOriginGlobal;
			parent->setGeometry(originalGeometry);

			//newRect = parent->geometry();
			//newRect = parent->geometry().adjusted(newValue, 0, 0, 0);
			//qDebug() << "newRect:" << newRect;
			//parent->setGeometry(parentPoint.x(), originalGeometry.top(), originalGeometry.width(), originalGeometry.height());
			//parent->setGeometry(newRect.adjusted(newValue, 0, 0, 0));
			//parent->setGeometry(newRect);
			//parent->move(newX, originalGeometry.y());
			//	parent->setFixedWidth(newValue - frameOriginGlobal.x());

			break;
		case EdgeControler::TOP:
			originalGeometry.setTop(newValue);
			qDebug() << "originalGeometry.top()" << originalGeometry.top();
			parent->setGeometry(originalGeometry);
			break;

		case EdgeControler::RIGHT:
			// OK
			//parent->setFixedWidth(newValue - frameOriginGlobal.x());
			originalGeometry.setRight(newValue);
			qDebug() << "originalGeometry.right()" << originalGeometry.right();
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
			//parent->setFixedHeight(newValue - frameOriginGlobal.y());
			originalGeometry.setBottom(newValue);
			qDebug() << "originalGeometry.bottom()" << originalGeometry.bottom();
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