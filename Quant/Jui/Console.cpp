#include "Console.h"

namespace Jui
{

	Console::Console(QWidget *parent) : Panel(parent)
	{
		setObjectName("Console");

		setEdgeControler(EdgeControler::Direction::LEFT, true);

		text = new QTextEdit(this);
		text->setReadOnly(true);
		text->setOverwriteMode(false);
		text->setFont(QFont("Consolas", 8));
		text->setFrameStyle(QFrame::NoFrame);
	
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