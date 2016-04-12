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

		prEdit = false;

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
		QStringList classes = regexpText(this->toPlainText(), "\\b[A-Z]\\w*");
		QStringList symbols = regexpText(this->toPlainText(), "\\\\\\w*");
		QStringList nodes = regexpText(this->toPlainText(), "~\\w+");
		QStringList floats = regexpText(this->toPlainText(), "\\b((\\d+(\\.\\d+)?([eE][-+]?\\d+)?(pi)?)|pi)\\b");

		qDebug() << "found class: " << classes;
		qDebug() << "found symbols: " << symbols;
		qDebug() << "found nodes: " << nodes;
		qDebug() << "found floats: " << floats;

		emit classesAct(regexpText2(HighLights::CLASS));
		emit symbolsAct(regexpText2(HighLights::CONTROL));
		emit floatsAct(regexpText2(HighLights::DIGIT));

		highlightText(this->toPlainText(), Qt::red);
	}

	QStringList CodeEditor::regexpText2(HighLights typeHighLights)
	{
		QString regexpRule;
		switch (typeHighLights)
		{
		case Jui::HighLights::CLASS:
			regexpRule = "\\b[A-Z]\\w*";
			break;
		case Jui::HighLights::CONTROL:
			regexpRule = "\\\\\\w*";
			break;
		case Jui::HighLights::NODE:
			regexpRule = "~\\w+";
			break;
		case Jui::HighLights::DIGIT:
			regexpRule = "\\b((\\d+(\\.\\d+)?([eE][-+]?\\d+)?(pi)?)|pi)\\b";
			break;
		}

		QString text = this->toPlainText();
		QStringList answ;
		QRegExp rule(regexpRule);
		int count = 0;
		int pos = 0;
		while ((pos = rule.indexIn(text, pos)) != -1) {
			qDebug() << count << ") class pos [" << pos << " size " << rule.matchedLength() << "]";
			QStringRef subString(&text, pos, rule.matchedLength());
			answ.append(subString.toString());
			++count;
			pos += rule.matchedLength();
		}
		return answ;
	}

	QStringList CodeEditor::regexpText(const QString &text, QString regexpRule)
	{
		QStringList answ;
		QRegExp rule(regexpRule);
		int count = 0;
		int pos = 0;
		while ((pos = rule.indexIn(text, pos)) != -1) {
			//qDebug() << count << ") class pos [" << pos << " size " << rClass.matchedLength() << "]";
			QStringRef subString(&text, pos, rule.matchedLength());
			answ.append(subString.toString());
			++count;
			pos += rule.matchedLength();
		}
		return answ;
	}

	void CodeEditor::highlightText(QString text, QColor color)
	{
		if (!prEdit)
		{
			//QString txt = this->toPlainText();
			prEdit = true;
			QTextCursor cursor = this->textCursor();
			int pos = cursor.position();

			qDebug() << cursor.position();

			QTextCharFormat format;
			format.setFontWeight(QFont::Normal);
			format.setForeground(QBrush(QColor("white")));

			cursor.setPosition(0, QTextCursor::MoveMode::MoveAnchor);
			cursor.setPosition(text.size(), QTextCursor::MoveMode::KeepAnchor);
			cursor.setCharFormat(format);

			format.setFontWeight(QFont::DemiBold);
			format.setForeground(QBrush(QColor("red")));

			cursor.setPosition(pos, QTextCursor::MoveMode::MoveAnchor);
			cursor.setPosition(pos + 2, QTextCursor::MoveMode::KeepAnchor);
			cursor.setCharFormat(format);

			cursor.setPosition(pos, QTextCursor::MoveMode::MoveAnchor);
			prEdit = false;
		}
	}

	void CodeEditor::setBackground(const QColor & color)
	{
		this->setStyleSheet(tr("QTextEdit { background-color: %1; }").arg(color.name()));
	}

	bool CodeEditor::eventFilter(QObject* target, QEvent* event)
	{

		if (event->type() == QEvent::KeyPress)
		{
			prEdit = false;

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

		if (event->type() == QEvent::KeyRelease)
		{
			//if (!prEdit)
			//{
				highlightText(this->toPlainText(), Qt::red);
			//}
		}

		return QTextEdit::eventFilter(target, event);
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