#include "Panel.h"

namespace Jui
{

	Panel::Panel(QWidget *parent) : QWidget(parent)
	{
		this->setGeometry(200, 100, 200, 100);

	}

	void Panel::paintEvent(QPaintEvent *)
	{
		
		QPainter painter(this);
		
		painter.setPen(Qt::NoPen);
		painter.setBrush(QBrush(QColor(120, 120, 20), Qt::SolidPattern));
		painter.drawRect(this->geometry());
	}

}