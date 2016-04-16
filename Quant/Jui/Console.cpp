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
		text->setTabStopWidth(30);
		//text->setFont(QFont("Consolas", 8));
		//text->setFont(QFont("Univers Condensed", 10, QFont::Normal));
		text->setFont(QFont("Univers 57 Condensed", 9, QFont::Normal));
		text->setFrameStyle(QFrame::NoFrame);

		text->append(tr("Console init..."));

		connect(this, SIGNAL(resizeAct()), this, SLOT(fitGeometry()));
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
			//QString old = text->toPlainText();
			//text->setText(old.operator+=(newText.toLatin1()));
			//	text->setPlainText(old.operator+=(newText));
			//text->append(newText);
			text->insertPlainText(newText); // problem s udrzeni pohledu na konci konzole
		}

		format.setForeground(QBrush(QColor(230, 230, 230), Qt::SolidPattern));
		text->setCurrentCharFormat(format);
		
	}

	void Console::fitGeometry() { text->setGeometry(10, 35, this->width() - 20, this->height() - 45); }

	Console::~Console()	{ }

}