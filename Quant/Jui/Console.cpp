#include "Console.h"

namespace Jui
{

	Console::Console(QWidget *parent) : Panel(parent)
	{
		setObjectName("Console");

		text = new QTextEdit(this);
		text->setReadOnly(true);
		text->setOverwriteMode(false);
		text->setFont(QFont("Consolas", 8));
		text->setFrameStyle(QFrame::NoFrame);
		//text->setStyleSheet("background-color: QColor(Qt::transparent);");
		//text->setStyleSheet("QTextEdit{background-color: QColor(30,30,30)};}");
		/*
		text->setStyleSheet(
			"QTextEdit{background-color: QColor(30,30,30)};}"
			"QScrollBar:vertical{ border: 2px solid grey; background: #32CC99; width: 15px;	margin: 22px 0 22px 0; }"
			"QScrollBar::handle:vertical{background: white;min-height: 20px;}"
			"QScrollBar::add-line:vertical{border: 2px solid grey; background: #32CC99; height: 20px; subcontrol-position: bottom; subcontrol-origin: margin;}"
			"QScrollBar::sub-line:vertical{border: 2px solid grey; background: #32CC99; height: 20px; subcontrol-position: top; subcontrol-origin: margin;}"
			"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow : vertical{ border: 2px solid grey; width: 3px; height: 3px; background: white;}"
			);
		*/

		//QString textStyle = tr("QTextEdit{background-color: QColor(30,30,30); color: QColor(230,30,30);}");


		text->append(tr("Console init..."));

		connect(this, SIGNAL(resizeAct()), this, SLOT(fitTextSize()));
	}

	void Console::addLine(QString newLine) { text->append(newLine); }

	void Console::fitTextSize()
	{
		text->setGeometry(10, 35, this->width() - 20, this->height() - 45);
	}

	Console::~Console()
	{

	}

}