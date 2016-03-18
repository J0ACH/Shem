#include "Rect.h"

namespace Jui
{
	Rect::Rect(QWidget *target)
	{
		parent = target;
	}

	Rect::~Rect() {}

	QRect Rect::offset(int left, int top, int right, int bottom)
	{
		//QRect rect (parent.geometry());
		QRect rect(177, 150, 500, 500);

		parent->geometry();
		return rect;
	};

}
