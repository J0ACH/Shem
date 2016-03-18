#include "Edges.h"

namespace Jui
{
	Edges::Edges(QWidget *parent)
	{
		this->setParent(parent);
		this->setGeometry(QRect(0, 0, parent->width(), parent->height()));
		//parent = widget;
		//std::list<EdgeControler> controlers;
		//std::list<QWidget> controlers;

		QList<EdgeControler*> test;
		//QList<EdgeControler> test;

		//this->setGeometry(parent->geometry());
		EdgePosition position(EdgePosition::RIGHT);

		QRect frame = parent->geometry();
		EdgeControler *edge = new EdgeControler(this);
		//ahoj = new EdgeControler(this);



		switch (position)
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


		//controlers.emplace_back(edge);
		//controlers.push_back(edge);
		test.append(edge);
	}

	//EdgeControler Edges::getAhoj(void) { return ahoj;  }
	
	void Edges::paintEvent(QPaintEvent *event)
	{
		QPainter painter;

		painter.setPen(QPen(Qt::white, 3));
		//painter.setBrush(QBrush(QColor(20, 20, 20), Qt::SolidPattern));
		//painter.drawRect(QRect(0, 0, canvan->screen->width(), canvan->screen->height()));
		painter.drawRect(QRect(0, 0, 20, 20));
		painter.drawRect(this->geometry());
	}
	

	Edges::~Edges()
	{

	}


	EdgeControler::EdgeControler(QWidget *parent)
	{
		//parent = edges;
	}


	void EdgeControler::paintEvent(QPaintEvent *event)
	{
		QPainter painter;

		painter.setPen(QPen(Qt::white, 1));
		//painter.setBrush(QBrush(QColor(20, 20, 20), Qt::SolidPattern));
		//painter.drawRect(QRect(0, 0, canvan->screen->width(), canvan->screen->height()));
		painter.drawRect(this->geometry());
	}


	EdgeControler::~EdgeControler()	{	}

}