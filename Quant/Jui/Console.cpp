#include "Console.h"

namespace Jui
{

	Console::Console(QWidget *parent) : Panel(parent)
	{
		setObjectName("Console");

		setEdgeControler(EdgeControler::Direction::LEFT, true);

		QFont font = QFont("Univers 57 Condensed", 9, QFont::Normal);
		font.setStretch(QFont::Unstretched);

		text = new QTextEdit(this);
		text->setReadOnly(true);
		text->setOverwriteMode(false);
		text->setTabStopWidth(30);
		
		text->setFrameStyle(QFrame::NoFrame);

		text->append(tr("Console init..."));

		connect(this, SIGNAL(resizeAct()), this, SLOT(fitGeometry()));
	}

	void Console::setFont(QFont font) 
	{ 
		text->setFont(font);
		text->verticalScrollBar()->setValue(text->verticalScrollBar()->maximum());

		update();
	}

	void Console::addText(QString newText, QColor color, bool newLine)
	{		
		QTextCharFormat format;
		format.setForeground(QBrush(color, Qt::SolidPattern));
		text->setCurrentCharFormat(format);

		if (newLine) {
			text->append(newText);
		}
		else
		{
			text->insertPlainText(newText); // problem s udrzeni pohledu na konci konzole
			text->verticalScrollBar()->setValue(text->verticalScrollBar()->maximum());
		}

		format.setForeground(QBrush(QColor(230, 230, 230), Qt::SolidPattern));
		text->setCurrentCharFormat(format);
		
	}

	void Console::fitGeometry() { text->setGeometry(10, 35, this->width() - 20, this->height() - 45); }

	Console::~Console()	{ }

}