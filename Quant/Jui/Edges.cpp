#include "Edges.h"

namespace Jui
{

	Edges::Edges(QWidget *parent) : QWidget(parent)
	{
		setGeometry(0, 0, parent->width(), parent->height());
		setEdgeOffset(10);
		drawRegion = true;
		//QRegion maskRegion(bounds());
		//maskRegion.operator+(bounds());

		collEdges = new QVector<EdgeControler*>();
		collRect = new QVector<QRect>();

		//collRect->append(QRect(0, 0, 30, 30));
		//collRect->append(QRect(200, 0, 30, 30));
		//collRect->append(QRect(0, 200, 30, 30));


		this->addManipulator(EdgePosition::LEFT);
		//this->addManipulator(EdgePosition::RIGHT);

		//setAttribute(Qt::WA_TransparentForMouseEvents, true);
		//setAttribute(Qt::WA_NoMousePropagation);

		/*
		testEdge = new EdgeControler(this);
		testEdge->setGeometry(0, 0, 150, 150);
		testEdge->setDirection(EdgePosition::LEFT);
		testEdge->setEdgeOffset(edgeOffset);

		collEdges.append(testEdge);

		testEdge = new EdgeControler(this);
		testEdge->setGeometry(0, 0, 150, 150);
		testEdge->setDirection(EdgePosition::RIGHT);
		testEdge->setEdgeOffset(edgeOffset);

		collEdges.append(testEdge);
		*/
	}

	QRect Edges::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void Edges::addManipulator(EdgePosition direction)
	{
		EdgeControler *manipulator = new EdgeControler(this);
		manipulator->setGeometry(0, 0, 150, 150);
		manipulator->setDirection(direction);
		collEdges->append(manipulator);

		QRect manipulatorRect = manipulator->bounds();
		collRect->append(manipulatorRect);

		qDebug() << tr("manipulatorRect : %1, %2, %3, %4").arg(
			QString::number(manipulatorRect.left()),
			QString::number(manipulatorRect.top()),
			QString::number(manipulatorRect.right()),
			QString::number(manipulatorRect.bottom())
			);

		qDebug() << tr("rectCount : %1").arg(QString::number(maskRegion.rectCount()));

		if (maskRegion.rectCount() == 0){
			QRegion maskRegion(manipulatorRect);
			qDebug() << tr("NewMask : %1").arg(QString::number(maskRegion.rectCount()));
		}
		else
		{
			maskRegion = maskRegion.operator+=(manipulatorRect);
			qDebug() << tr("AddMask : %1").arg(QString::number(maskRegion.rectCount()));
		};
		qDebug() << tr("rectCount2 : %1").arg(QString::number(maskRegion.rectCount()));
		//this->setMask(maskRegion);

	}

	void Edges::setEdgeOffset(int offset) { edgeOffset = offset; }

	void Edges::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		if (drawRegion)
		{
			painter.fillRect(bounds(), QColor(255, 30, 30, 100));
			/*
			for (int i = 0; i < collRect->size(); ++i) {
				painter.fillRect(collRect->at(i), QColor(255, 30, 30, 100));
				//	cout << "Found Alfonso at position " << i << endl;
			}
			*/

			foreach(QRect oneRect, *collRect)
			{
				maskRegion = maskRegion.operator+=(oneRect);
			}

			foreach(QRect oneRect, *collRect)
			{
			//	painter.fillRect(oneRect, QColor(255, 30, 30, 100));
			}
		}

		painter.setPen(QPen(Qt::red, 1));
		painter.drawRect(bounds());
			qDebug("Edges::paintEvent");
	}

	void Edges::moveEvent(QMoveEvent * moveEvent)
	{

	}

	void Edges::resizeEvent(QResizeEvent *resizeEvent)
	{
		setGeometry(0, 0, this->parentWidget()->width(), this->parentWidget()->height());

		qDebug() << tr("rectCountResize : %1").arg(QString::number(maskRegion.rectCount()));
		//this->addManipulator(EdgePosition::LEFT);
		//QRegion r1(bounds());
		//QRegion r2(bounds().adjusted(edgeOffset, edgeOffset, -edgeOffset, -edgeOffset));
		//maskRegion = r1.subtracted(r2);
		

		foreach(EdgeControler *oneCtrl, *collEdges)
		{
			oneCtrl->fitGeometry();
		}


		this->setMask(maskRegion);

		//		this->parentWidget()->resize(this->size());



		qDebug("Edges::resizeEvent");
	}

	Edges::~Edges()
	{

	}
}