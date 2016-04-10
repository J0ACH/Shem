#include "CodeEditor.h"

namespace Jui
{

	CodeEditor::CodeEditor(QWidget *parent) : QTextEdit(parent)
	{
		setObjectName("CodeEditor");
		installEventFilter(this);

		setFont(QFont("Consolas", 8));
		setFrameStyle(QFrame::NoFrame);
		

		normalColor = QColor(120, 120, 120);
		overColor = QColor(230, 230, 230);
		activeColor = QColor(50, 65, 95);

		

		append(tr("CodeEditor init..."));

		backgroundAlpha = 0;
		fadeTimeOut = 800;
		fps = 25;
		timer = new QTimer(this);

		connect(this, SIGNAL(textChanged()), this, SLOT(fitTextFormat()));
		connect(timer, SIGNAL(timeout()), this, SLOT(alphaUpdate()));
	}

	void CodeEditor::fitTextFormat()
	{
		highlightText(this->toPlainText());
	}

	void CodeEditor::setBackground(const QColor & color)
	{
		this->setStyleSheet(tr("QTextEdit { background-color: %1; }").arg(color.name()));
	}

	bool CodeEditor::eventFilter(QObject* target, QEvent* event)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent* eventKey = static_cast<QKeyEvent*>(event);

			quint32 modifers = eventKey->nativeModifiers();

			switch (eventKey->key())
			{
			case Qt::Key::Key_Return:

				qDebug() << "event: ENTER PRESSED";

				switch (eventKey->modifiers())
				{
				case Qt::KeyboardModifier::ControlModifier:
					backgroundAlpha = 255;
					timer->stop();
					timer->setInterval(fadeTimeOut / fps);
					timer->start();

					emit sendText(this->toPlainText());

					qDebug() << "KeyEvent: Ctrl+ENTER PRESSED (modifer" << eventKey->modifiers() << ")";
					return true;
					break;
				}
				break;
			case Qt::Key::Key_Escape:
				qDebug() << "KeyEvent: Escape PRESSED";
				return true;
				break;
			}
		}

		return QTextEdit::eventFilter(target, event);
	}

	void CodeEditor::highlightText(const QString &text)
	{
		for (int i = 0; i < text.length(); ++i) {
			if (!text.at(i).isLetterOrNumber())
				//setFormat(i, 1, Qt::green);
				qDebug() << "highlight: " << text.at(i);
		}
	}

	void CodeEditor::paintEvent(QPaintEvent *event)
	{
		QPainter painter(viewport());
		QRect bounds = QRect(0, 0, viewport()->width() - 1, viewport()->height() - 1);
		QColor fillColor = activeColor;
		fillColor.setAlpha(backgroundAlpha);
		painter.fillRect(bounds, fillColor);
		
		painter.setPen(QColor(230, 230, 230));
		painter.drawLine(0, 0, width(), 0);
		painter.drawLine(0, height() - 1, width(), height() - 1);

		QTextEdit::paintEvent(event);
	}

	void CodeEditor::alphaUpdate()
	{
		backgroundAlpha -= 255 / fps;
		if (backgroundAlpha <= 0)
		{
			timer->stop();
			backgroundAlpha = 0;
		}

		//qDebug() << tr("buttonAlpha %1").arg(QString::number(backgroundAlpha));
		update();
	}

	CodeEditor::~CodeEditor() { }

}