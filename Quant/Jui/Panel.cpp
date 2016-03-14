#include "Panel.h"

namespace Jui
{

	Panel::Panel(QWidget *parent) : QWidget(parent)
	{
		this->setParent(parent);
		//this->setGeometry(600, 100, 200, 200);
		//this->setProperty()
		connect(this->parent(), SIGNAL(actDraw()), this, SLOT(draw()));
	}
	/*
	void Panel::paintEvent(QPaintEvent *)
	{
		
		//draw();
	}
	*/

	
	void Panel::draw()
	{
		//this->parent->msgConsole(QString("Panel draw..."));

		QPainter painter(this);

		painter.setPen(QPen(Qt::red, 5));
		painter.setBrush(QBrush(QColor(120, 120, 120), Qt::SolidPattern));
		painter.drawRect(this->geometry());
	}
	

}