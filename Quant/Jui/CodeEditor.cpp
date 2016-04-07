#include "CodeEditor.h"

namespace Jui
{

	CodeEditor::CodeEditor(QWidget *parent) : QTextEdit(parent)
	{
		setObjectName("CodeEditor");
		installEventFilter(this);

		setFont(QFont("Consolas", 8));
		setFrameStyle(QFrame::NoFrame);

		append(tr("CodeEditor init..."));
		
		connect(this, SIGNAL(textChanged()), this, SLOT(fitFormat()));
	}

	void CodeEditor::fitTextFormat()
	{
		highlightText(this->toPlainText());
	}

	bool CodeEditor::eventFilter(QObject* _o, QEvent* _e)
	{
		if (_e->type() == QEvent::KeyPress)
		{
			QKeyEvent* eventKey = static_cast<QKeyEvent*>(_e);
			if (eventKey->key() == Qt::Key_Return)
			{
				emit evaluate(this->toPlainText());
				qDebug() << "event: ENTER PRESSED";
				return true;
			}
		}
		return QTextEdit::eventFilter(_o, _e);
	}

	void CodeEditor::highlightText(const QString &text)
	{
		for (int i = 0; i < text.length(); ++i) {
			if (!text.at(i).isLetterOrNumber())
				//setFormat(i, 1, Qt::green);
				qDebug() << "highlight: " << text.at(i);
		}
	}	

	CodeEditor::~CodeEditor()	{ }

}