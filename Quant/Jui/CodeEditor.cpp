#include "CodeEditor.h"

namespace Jui
{

	CodeEditor::CodeEditor(QWidget *parent) : QTextEdit(parent)
	{
		setObjectName("CodeEditor");

		

		//text = new QTextEdit(this);
		//text->setReadOnly(true);
		//text->setOverwriteMode(false);
		setFont(QFont("Consolas", 8));
		setFrameStyle(QFrame::NoFrame);

		append(tr("CodeEditor init..."));

		//connect(this, SIGNAL(resizeAct()), this, SLOT(fitGeometry()));
	}
	/*
	void CodeEditor::addText(QString newText, bool newLine)
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

	void CodeEditor::fitGeometry() { text->setGeometry(10, 35, this->width() - 20, this->height() - 45); }
	*/

	CodeEditor::~CodeEditor()	{ }

}