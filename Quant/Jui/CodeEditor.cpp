#include "CodeEditor.h"

namespace Jui
{

  CodeEditor::CodeEditor(QWidget *parent) : QTextEdit(parent)
  {
    setObjectName("CodeEditor");

    this->installEventFilter(this);
    setFocusPolicy(Qt::StrongFocus);
    setFrameStyle(QFrame::NoFrame);

    normalColor = QColor(120, 120, 120);
    overColor = QColor(230, 230, 230);
    activeColor = QColor(50, 65, 95);

    append(tr("CodeEditor init..."));

    backgroundAlpha = 0;
    fadeTimeOut = 800;
    fps = 25;
    timer = new QTimer(this);

    testSecondCursor = new QTextCursor(this->document());
    testSecondCursorRect = QRect(0, 0, 10, 10);

    connect(this, SIGNAL(textChanged()), this, SLOT(fitTextFormat()));
    connect(timer, SIGNAL(timeout()), this, SLOT(alphaUpdate()));
  }

  void CodeEditor::setFontCode(QFont font) { this->setFont(font); update(); }

  void CodeEditor::fitTextFormat()
  {
    QTextCharFormat formatCommonText;
    formatCommonText.setForeground(QBrush(QColor(120, 120, 120)));
    this->highlightText(0, this->toPlainText().size(), formatCommonText);

    QList<QList<QVariant>*> classes = regexpText(HighLights::CLASS);
    QList<QList<QVariant>*> digits = regexpText(HighLights::DIGIT);
    QList<QList<QVariant>*> controls = regexpText(HighLights::CONTROL);
    //QList<QList<QVariant>*> nodes = regexpText(HighLights::NODE);
  }

  QList<QList<QVariant>*> CodeEditor::regexpText(HighLights typeHighLight)
  {
    QTextCharFormat format;
    QString regexpRule;
    switch (typeHighLight)
    {
    case HighLights::CLASS:
      regexpRule = "\\b[A-Z]\\w*";
      format.setFontWeight(QFont::Normal);
      format.setForeground(QBrush(QColor(180, 180, 180)));
      break;
    case HighLights::CONTROL:
      regexpRule = "\\\\\\w*";
      format.setFontWeight(QFont::DemiBold);
      format.setForeground(QBrush(QColor(20, 180, 240)));
      break;
    case HighLights::NODE:
      regexpRule = "~\\w+";
      format.setFontWeight(QFont::Normal);
      format.setForeground(QBrush(Qt::green));
      break;
    case HighLights::DIGIT:
      format.setFontWeight(QFont::Normal);
      regexpRule = "\\b((\\d+(\\.\\d+)?([eE][-+]?\\d+)?(pi)?)|pi)\\b";
      format.setForeground(QBrush(QColor(20, 110, 180)));
      break;
    }

    QString text = this->toPlainText();
    QList<QList<QVariant>*> answ;
    QRegExp rule(regexpRule);
    int count = 0;
    int position = 0;
    while ((position = rule.indexIn(text, position)) != -1)
    {
      QStringRef subString(&text, position, rule.matchedLength());
      QList<QVariant> *oneReg = new QList<QVariant>();
      oneReg->append(subString.toString());
      oneReg->append(position);
      oneReg->append(rule.matchedLength());
      answ.append(oneReg);

      this->highlightText(position, rule.matchedLength(), format);

      ++count;
      position += rule.matchedLength();
    }

    return answ;
  }

  void CodeEditor::highlightText(int position, int lenght, QTextCharFormat format)
  {
    disconnect(this, SIGNAL(textChanged()), this, SLOT(fitTextFormat()));

    QTextCursor cursor = this->textCursor();
    int cursorPositionBackup = cursor.position();

    cursor.setPosition(position, QTextCursor::MoveMode::MoveAnchor);
    cursor.setPosition(position + lenght, QTextCursor::MoveMode::KeepAnchor);
    cursor.setCharFormat(format);

    cursor.setPosition(cursorPositionBackup, QTextCursor::MoveMode::MoveAnchor);

    connect(this, SIGNAL(textChanged()), this, SLOT(fitTextFormat()));
  }

  void CodeEditor::setBackground(const QColor & color)
  {
    this->setStyleSheet("QTextEdit { background-color: rgba(0,0,0,0); }");
  }

  void CodeEditor::onChangeExtraCursor(QString name, int position)
  {
    qDebug() << "CodeEditor::onChangeExtraCursor name:" << name << "position:" << position;

    if (!extraCursors.contains(name))
    {
      QTextCursor *oneCursor = new QTextCursor(this->document());
      //oneCursor = QRect(0, 0, 10, 10);

      extraCursors.insert(name, oneCursor);
    }

    extraCursors.value(name)->setPosition(position);
    this->update();
  }

  void CodeEditor::onEvaluate(QColor color)
  {
    backgroundAlpha = 255;
    timer->stop();
    timer->setInterval(fadeTimeOut / fps);
    timer->start();
  }

  bool CodeEditor::eventFilter(QObject* target, QEvent* event)
  {
    //qDebug() << "EventType " << event->type();
    if (hasFocus())
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
            /*
            backgroundAlpha = 255;
            timer->stop();
            timer->setInterval(fadeTimeOut / fps);
            timer->start();
            */
            this->onEvaluate(activeColor);

            //emit evaluateAct();
            emit sendText(this->toPlainText());

            emit actValueEvaluate(this->toPlainText());
            emit actValueEvaluate(this->objectName(), this->toPlainText());


            qDebug() << "KeyEvent: Ctrl+ENTER PRESSED (modifer" << eventKey->modifiers() << ")";
            event->accept();
            return true;
            break;
          }
          break;
        case Qt::Key::Key_Escape:
          qDebug() << "KeyEvent: Escape PRESSED";
          event->accept();
          return true;
          break;
        }
      }
      if (event->type() == QEvent::KeyRelease)
      {
        cursorPosition = this->textCursor().position();
        testSecondCursor->setPosition(cursorPosition - 5);
        testSecondCursorRect = this->cursorRect(*testSecondCursor);
        this->update();

        emit actCursorMoved(cursorPosition);
        emit actValueChanged(this->toPlainText());
        emit actValueChanged(this->objectName(), this->toPlainText());

        qDebug() << "KeyEvent: cursor pos:" << cursorPosition;
        qDebug() << "KeyEvent: cursor RECT:" << this->cursorRect();

        qDebug() << "KeyEvent: cursor 2ND pos:" << testSecondCursor->position();
        qDebug() << "KeyEvent: cursor 2NP RECT:" << this->cursorRect(*testSecondCursor);
      }
    };

    event->ignore();
    return QObject::eventFilter(target, event);
  }

  void CodeEditor::paintEvent(QPaintEvent *event)
  {
    QPainter painter(viewport());
    QRect bounds = QRect(0, 0, viewport()->width() - 1, viewport()->height() - 1);
    QColor fillColor = activeColor;
    fillColor.setAlpha(backgroundAlpha);
    painter.fillRect(bounds, fillColor);

    if (this->hasFocus()) { painter.setPen(activeColor); }
    else { painter.setPen(QColor(120, 120, 120)); }

    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, height() - 1, width(), height() - 1);

    // second cursor test
    painter.setPen(QPen(QColor(90, 40, 40), 3));
    foreach(QTextCursor *oneExtraCursor, extraCursors)
    {
      QRect cursorRect = this->cursorRect(*oneExtraCursor);
      painter.drawLine(
        cursorRect.topLeft().x(),
        cursorRect.topLeft().y(),
        cursorRect.bottomLeft().x(),
        cursorRect.bottomLeft().y()
        );
    }
    
    painter.drawText(100, 20, this->objectName());

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

  CodeEditor::~CodeEditor() {	}

}
