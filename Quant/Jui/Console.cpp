#include "Console.h"

namespace Jui
{

	Console::Console(QWidget *parent) : Panel(parent)
	{
		setObjectName("Console");

		//setGeometry(this->width() - 310, headerSize + 5, 300, this->height() - headerSize - tailSize - 10);

		text = new QTextEdit(this);
		//text->setGeometry(10, 45, this->width() - 20, this->height() - 55);
		text->setReadOnly(true);
		text->setOverwriteMode(false);
		text->setFont(QFont("Consolas", 8));

		text->append(tr("ahoj"));
		text->append(tr("zdur"));
		//update();

		text->setFrameStyle(QFrame::NoFrame);
		//text->setStyleSheet("QTextEdit{background-color: Qt::transparent;}");
		text->setStyleSheet("background-color: QColor(Qt::transparent);");

		connect(this, SIGNAL(resizeAct()), this, SLOT(fitTextSize()));
	}

	void Console::addLine(QString newLine) { text->append(newLine); }

	void Console::fitTextSize()
	{
//		qDebug() << "Console::fitTextSize";
		text->setGeometry(10, 45, this->width() - 20, this->height() - 55);
	}

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