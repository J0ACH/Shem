#include "Edges.h"

namespace Jui
{
	Edges::Edges(QWidget *window) : QWidget(window)
	{
		//this->setParent(parent);
		//this->setGeometry(QRect(0, 0, parent->width(), parent->height()));
		parent = window;
		setGeometry(0, 0, parent->width(), parent->height());

		//parent = widget;
		//std::list<EdgeControler> controlers;
		//std::list<QWidget> controlers;

		//QList<EdgeControler*> test;
		//QList<EdgeControler> test;

		//this->setGeometry(parent->geometry());
		//EdgePosition position(EdgePosition::RIGHT);

		//QRect frame = parent->geometry();
		//EdgeControler *edge = new EdgeControler(this);
		//ahoj = new EdgeControler(this);



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
		QPainter painter;

		painter.setPen(QPen(Qt::red, 1));
		painter.drawRect(bounds().adjusted(2, 2, -2, -2));

		qDebug("Edges::paintEvent");
	}

	void Edges::resizeEvent(QResizeEvent *resizeEvent)
	{		
		this->setGeometry(0, 0, parent->width(), parent->height());
		qDebug("Edges::resizeEvent");
	}
	/*
	bool Edges::eventFilter(QObject* target, QEvent* event)
	{
		if (target == this && event->type() == QEvent::Paint) {
			QPainter painter;
			painter.begin(this);
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.setPen(QPen(Qt::black, 12, Qt::DashDotLine, Qt::RoundCap));
			painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
			painter.drawEllipse(50, 50, 100, 100);
			painter.end();
			return true; // return true if you do not want to have the child widget paint on its own afterwards, otherwise, return false.
		}
		return false;
	}
	*/

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