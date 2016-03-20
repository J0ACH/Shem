#include "Edges.h"

namespace Jui
{
	// EdgeControler musi byt nacten pred Edge
	EdgeControler::EdgeControler(QWidget *parent) : QWidget(parent)
	{
		//parent = edges;
	}

	QRect EdgeControler::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void EdgeControler::setDirection(EdgePosition direction)
	{
		side = direction;
		update();
	}

	void EdgeControler::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		painter.setPen(QPen(Qt::yellow, 3));
		painter.drawRect(bounds());
		//qDebug("EdgesControler::paintEvent");
	}

	void EdgeControler::resizeEvent(QResizeEvent *resizeEvent)
	{
		switch (side)
		{
		case Jui::LEFT:
			this->setGeometry(0, 0, 15, this->parentWidget()->height());
			break;
		case Jui::TOP:
			break;
		case Jui::RIGHT:
			this->setGeometry(this->parentWidget()->width() - 15, 0, 15, this->parentWidget()->height());
			break;
		case Jui::BOTTOM:
			break;
		case Jui::ALL:
			break;
		default:
			break;
		}

		qDebug("EdgeControler::resizeEvent");
	}

	EdgeControler::~EdgeControler()	{	}


	Edges::Edges(QWidget *parent) : QWidget(parent)
	{
		setGeometry(0, 0, parent->width(), parent->height());

		setAttribute(Qt::WA_TransparentForMouseEvents);

		//testEdge = new EdgeControler(this);
		testEdge = new EdgeControler(this);
		testEdge->setGeometry(0, 0, 150, 150);
		testEdge->setDirection(EdgePosition::LEFT);
		//testEdge->setDirection(EdgePosition::RIGHT);

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

	void Edges::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		painter.setPen(QPen(Qt::red, 1));
		//painter.drawRect(bounds().adjusted(2, 2, -2, -2));
		painter.drawRect(bounds());

		//	qDebug("Edges::paintEvent");
	}

	void Edges::resizeEvent(QResizeEvent *resizeEvent)
	{
		//this->setGeometry(0, 0, parent->width(), parent->height());
		//testEdge->setGeometry(0, 0, 150, 150);

		qDebug("Edges::resizeEvent");
	}

	Edges::~Edges()
	{

	}






}