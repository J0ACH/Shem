#include "Edges.h"

namespace Jui
{
	
	Edges::Edges(QWidget *parent) : QWidget(parent)
	{
		setGeometry(0, 0, parent->width(), parent->height());
		setEdgeOffset(10);

		//setAttribute(Qt::WA_TransparentForMouseEvents, true);
		//setAttribute(Qt::WA_NoMousePropagation);

		//testEdge = new EdgeControler(this);
		testEdge = new EdgeControler(this);
		testEdge->setGeometry(0, 0, 150, 150);
		testEdge->setDirection(EdgePosition::LEFT);
		//	testEdge->setDirection(EdgePosition::RIGHT);
		testEdge->setEdgeOffset(edgeOffset);



		//std::list<EdgeControler> controlers;
		//std::list<QWidget> controlers;

		//QList<EdgeControler*> test;
		//QList<EdgeControler> test;

		//this->setGeometry(parent->geometry());
		//EdgePosition position(EdgePosition::RIGHT);

		//QRect frame = parent->geometry();
		//EdgeControler *edge = new EdgeControler(this);
		//ahoj = new EdgeControler(this);


		//controlers.emplace_back(edge);
		//controlers.push_back(edge);
		//test.append(edge);


	}

	QRect Edges::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void Edges::setEdgeOffset(int offset) { edgeOffset = offset; }

	void Edges::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		//painter.setClipRegion(r3);

		painter.fillRect(bounds(), QColor(255, 30, 30, 100));

		painter.setPen(QPen(Qt::red, 1));
		//painter.drawRect(bounds().adjusted(2, 2, -2, -2));
		painter.drawRect(bounds());

		//	qDebug("Edges::paintEvent");
	}

	void Edges::resizeEvent(QResizeEvent *resizeEvent)
	{
		QRegion r1(bounds());
		QRegion r2(bounds().adjusted(edgeOffset, edgeOffset, -edgeOffset, -edgeOffset));
		QRegion r3 = r1.subtracted(r2);
		this->setMask(r3);

//		this->parentWidget()->resize(this->size());

		qDebug("Edges::resizeEvent");
	}

	Edges::~Edges()
	{

	}

}