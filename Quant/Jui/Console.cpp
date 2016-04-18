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

		text->setFrameStyle(QFrame::NoFrame);

		text->append(tr("Console init..."));

		QTextCharFormat format;
		format.setForeground(QBrush(QColor(230, 30, 30)));
		text->setCurrentCharFormat(format);

		connect(this, SIGNAL(resizeAct()), this, SLOT(fitGeometry()));
	}

	void Console::setFont(QFont font)
	{
		text->setFont(font);
		text->verticalScrollBar()->setValue(text->verticalScrollBar()->maximum());
		update();
	}

	void Console::setColorText(QColor color)
	{
		QTextCharFormat format;
		format.setForeground(QBrush(color));
		text->setCurrentCharFormat(format);
	}

	void Console::addText(QString newText, QColor color = QColor(70, 70, 70), bool newLine = true)
	{
		QTextCharFormat format;
		format.setForeground(QBrush(color, Qt::SolidPattern));
		text->setCurrentCharFormat(format);

		if (newLine) {
			text->append(newText);
		}
		else
		{
			text->insertPlainText(newText);
			text->verticalScrollBar()->setValue(text->verticalScrollBar()->maximum());
		}

		format.setForeground(QBrush(QColor(230, 230, 230), Qt::SolidPattern));
		text->setCurrentCharFormat(format);

	}

	void Console::fitGeometry() { text->setGeometry(10, 35, this->width() - 20, this->height() - 45); }

	Console::~Console()	{ }

}