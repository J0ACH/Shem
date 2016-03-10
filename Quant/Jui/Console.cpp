#include "Console.h"

namespace Jui
{

	Console::Console(QWidget *parent)
		:QTextEdit(parent)
	{
		//text = new QTextEdit(this);
		setReadOnly(true);
		setOverwriteMode(false);
		setFont(QFont("Consolas", 8));

		append(tr("ahoj"));
		append(tr("zdur"));
		//update();

		setFrameStyle(QFrame::NoFrame);
	}

	void Console::addLine(QString newLine) { append(newLine); }

	/*
	void Console::paintEvent(QPaintEvent *event)
	{
	QPainter painter(this);

	QPen frame(QColor(20, 180, 240), 1);
	QBrush brush(QColor(125, 25, 25), Qt::SolidPattern);

	painter.setPen(Qt::NoPen);
	painter.setBrush(brush);
	painter.drawRect(QRect(0, 0, width(), height()));
	}
	*/


	Console::~Console()
	{

	}

}