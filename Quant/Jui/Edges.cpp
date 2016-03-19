#include "Edges.h"

namespace Jui
{
	Edges::Edges(QWidget *parent) : QWidget(parent)
	{
		setGeometry(0, 0, parent->width(), parent->height());

		setAttribute(Qt::WA_TransparentForMouseEvents);

		//std::list<EdgeControler> controlers;
		//std::list<QWidget> controlers;

		//QList<EdgeControler*> test;
		//QList<EdgeControler> test;

		//this->setGeometry(parent->geometry());
		//EdgePosition position(EdgePosition::RIGHT);

		//QRect frame = parent->geometry();
		//EdgeControler *edge = new EdgeControler(this);
		//ahoj = new EdgeControler(this);
		/*
		switch (EdgePosition::RIGHT)
		{
		case Jui::LEFT:
			break;
		case Jui::TOP:
			break;

		case Jui::RIGHT:
			//edge->setGeometry(frame.width() - 5, 5, 5, frame.height() - 10);
		//	ahoj->setGeometry(frame.width() - 5, 5, 5, frame.height() - 10);
			break;

		case Jui::BOTTOM:
			break;
		case Jui::ALL:
			break;
		default:
			break;
		}
		*/


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

		painter.setPen(QPen(Qt::red, 1 ));
		//painter.drawRect(bounds().adjusted(2, 2, -2, -2));
		painter.drawRect(bounds());

	//	qDebug("Edges::paintEvent");
	}

	void Edges::resizeEvent(QResizeEvent *resizeEvent)
	{		
		//this->setGeometry(0, 0, parent->width(), parent->height());
		qDebug("Edges::resizeEvent");
	}
	
	Edges::~Edges()
	{

	}

	/*

	EdgeControler::EdgeControler(QWidget *parent)
	{
		//parent = edges;
	}


	void EdgeControler::paintEvent(QPaintEvent *event)
	{
		QPainter painter;

		painter.setPen(QPen(Qt::white, 1));
		//painter.drawRect(bounds().adjusted(2, 2, -2, -2));
	}


	EdgeControler::~EdgeControler()	{	}
	*/

}