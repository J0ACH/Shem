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

		connect(this, SIGNAL(resizeAct()), this, SLOT(fitGeometry()));
	}

	void Console::addText(QString newText, bool newLine)
	{
		if (newLine) { text->append(newText); }
		else
		{
			//QString old = text->toPlainText();
			//text->setText(old.operator+=(newText.toLatin1()));
			//	text->setPlainText(old.operator+=(newText));
			//text->append(newText);
			text->insertPlainText(newText); // problem s udrzeni pohledu na konci konzole
		}
	}

	void Console::fitGeometry() { text->setGeometry(10, 35, this->width() - 20, this->height() - 45); }

	Console::~Console()	{ }

}