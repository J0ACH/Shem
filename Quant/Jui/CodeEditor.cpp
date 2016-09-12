#include "CodeEditor.h"

namespace Jui
{

    CodeEditor::CodeEditor(QWidget *parent) : QWidget(parent)
  {
    code = new QTextEdit(this);
    code->setObjectName("code");
    code->installEventFilter(this);
    code->setFrameStyle(QFrame::NoFrame);
    code->setTabStopWidth(30);
    code->append(tr("CodeEditor init..."));

    this->installEventFilter(this);
    code->installEventFilter(this);

    QFontMetrics fm(this->font());
    linePixelHeight = fm.height();
    this->resizeByLines();

    normalColor = QColor(120, 120, 120);
    overColor = QColor(230, 230, 230);
    activeColor = QColor(50, 65, 95);

    backgroundAlpha = 0;
    fadeTimeOut = 800;
    fps = 25;
    timer = new QTimer(this);

    connect(code, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect(code, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));
    connect(timer, SIGNAL(timeout()), this, SLOT(alphaUpdate()));
  }

  void CodeEditor::setFontCode(QFont font)
  {
    this->setFont(font);

    QFontMetrics fm(this->font());
    linePixelHeight = fm.height();

    update();
  }

  int CodeEditor::getLinePixelHeight()  { return linePixelHeight; }

  void CodeEditor::setBackground(const QColor & color)
  {
    this->setStyleSheet("QTextEdit { background-color: rgba(0,0,0,0); }");
  }

  void CodeEditor::setText(QString txt)  { code->setText(txt); }

  void CodeEditor::onTextChanged()
  {
    // cursorPosition = this->textCursor().position();
    qDebug() << "CodeEditor::onTextChanged";
    this->codeSnippet();

    QTextCharFormat formatCommonText;
    formatCommonText.setForeground(QBrush(QColor(120, 120, 120)));
    if (code->toPlainText().size() > 0)
    {
      this->highlightText(0, code->toPlainText().size(), formatCommonText);
    }
    QList<QList<QVariant>*> classes = regexpText(HighLights::CLASS);
    QList<QList<QVariant>*> digits = regexpText(HighLights::DIGIT);
    QList<QList<QVariant>*> controls = regexpText(HighLights::CONTROL);

    this->resizeByLines();
  }

  void CodeEditor::codeSnippet()
  {
    QString text = code->toPlainText();

    QMap<QString, QString> snippedLib;
    snippedLib.insert("sin", "SinOsc.ar(\\freq.kr(90)!2)");
    snippedLib.insert("setfreq", "\\xset -> Pbind(\n\t\\freq, Pseq([90,98,98,105], inf),\n\t\\dur, 0.5\n)");

    foreach(QString oneSnipped, snippedLib.keys())
    {
      if (text.contains(oneSnipped))
      {
        QTextCursor cursor = code->textCursor();
        int cursorPositionBackup = cursor.position();

        QString codeTxt = snippedLib.value(oneSnipped);

        QString foundText = text.replace(oneSnipped, codeTxt, Qt::CaseSensitivity::CaseSensitive);
        code->setPlainText(foundText);

        cursor.setPosition(cursorPositionBackup - oneSnipped.size() + codeTxt.size(), QTextCursor::MoveMode::MoveAnchor);
        code->setTextCursor(cursor);
      }
    }
  }

  void CodeEditor::highlightText(int position, int lenght, QTextCharFormat format)
  {
    disconnect(code, SIGNAL(textChanged()), this, SLOT(onTextChanged()));

    QTextCursor cursor = code->textCursor();
    int cursorPositionBackup = cursor.position();

    cursor.setPosition(position, QTextCursor::MoveMode::MoveAnchor);
    cursor.setPosition(position + lenght, QTextCursor::MoveMode::KeepAnchor);
    cursor.setCharFormat(format);

    cursor.setPosition(cursorPositionBackup, QTextCursor::MoveMode::MoveAnchor);

    connect(code, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
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
      format.setForeground(QBrush(QColor(220, 220, 220)));
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

    QString text = code->toPlainText();
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

  void CodeEditor::onCursorPositionChanged()
  {
    cursorPosition = code->textCursor().position();
    qDebug() << "CodeEditor::onCursorPositionChanged" << cursorPosition;

    emit actCursorMoved(cursorPosition);
  }

  void CodeEditor::onChangeExtraCursor(QString name, int position)
  {

    //cursor.movePosition(QTextCursor::MoveOperation::Left, QTextCursor::MoveMode::MoveAnchor, 5);

    /*
    qDebug() << "CodeEditor::onChangeExtraCursor name:" << name << "position:" << position;

    if (!extraCursors.contains(name))
    {
    QTextCursor *oneCursor = new QTextCursor(this->document());
    //oneCursor = QRect(0, 0, 10, 10);

    extraCursors.insert(name, oneCursor);
    }

    extraCursors.value(name)->setPosition(position);
    this->update();
    */
  }

  void CodeEditor::onEvaluate(QColor color)
  {
    backgroundAlpha = 255;
    timer->stop();
    timer->setInterval(fadeTimeOut / fps);
    timer->start();
  }

  bool CodeEditor::eventFilter(QObject *target, QEvent *event)
  {
    //qDebug() << "EventType " << event->type();
    // if (hasFocus())
    // {
    if (event->type() == QEvent::KeyPress)
    {
      QKeyEvent* eventKey = static_cast<QKeyEvent*>(event);
      quint32 modifers = eventKey->nativeModifiers();

      switch (eventKey->key())
      {
      case Qt::Key::Key_Return:

        //qDebug() << "event: ENTER PRESSED";

        switch (eventKey->modifiers())
        {
        case Qt::KeyboardModifier::ControlModifier:

          this->onEvaluate(activeColor);

          emit actValueEvaluate(code->toPlainText());
          emit actValueEvaluate(this->objectName(), code->toPlainText());

          // qDebug() << "CodeEditor::eventFilter Ctrl+ENTER PRESSED (modifer" << eventKey->modifiers() << ")";
          event->accept();
          return true;
          break;
        default:
          // qDebug() << "CodeEditor::eventFilter ENTER PRESSED";
          if (target->objectName() != "code")
          {
            code->setFocus();
            return true;
          }
          break;
        }
        break;
      case Qt::Key::Key_Escape:
        // qDebug() << "CodeEditor::eventFilter ESC PRESSED";
        if (target->objectName() == "code")
        {
          this->setFocus();
        }
        else
        {
          emit actParentFocused(this);
        }
        event->accept();
        return true;
        break;
      case Qt::Key::Key_Up:
        // qDebug() << "ControlBox::eventFilter pressed Key_UP";
        if (target->objectName() == "code")
        {
          // qDebug() << "ControlBox::eventFilter UP on CODE" << target->objectName();
          return true;
        }
        else
        {
          // qDebug() << "ControlBox::eventFilter emit actPreviousFocused(" << this->objectName() << ")";
          emit actPreviousFocused(this);
          return true;
        }
        break;
      case Qt::Key::Key_Down:
        // qDebug() << "ControlBox::eventFilter pressed Key_Down";
        if (target->objectName() == "code")
        {
          //  qDebug() << "ControlBox::eventFilter DOWN on CODE" << target->objectName();
          return true;
        }
        else
        {
          // qDebug() << "ControlBox::eventFilter emit actNextFocused(" << this->objectName() << ")";
          emit actNextFocused(this);
          return true;
        }
        break;
      }
      //  }
      if (event->type() == QEvent::KeyRelease)
      {
        QKeyEvent* eventKey = static_cast<QKeyEvent*>(event);

        cursorPosition = code->textCursor().position();
        // testSecondCursor->setPosition(cursorPosition - 5);
        // testSecondCursorRect = this->cursorRect(*testSecondCursor);
        // this->update();

        emit actValueChanged(code->toPlainText());
        emit actValueChanged(this->objectName(), code->toPlainText());

        /*
        qDebug();
        qDebug() << "KeyEvent: cursor pos:" << cursorPosition;
        qDebug() << "KeyEvent: size of change:" << eventKey->count();
        qDebug() << "KeyEvent: insert text:" << eventKey->text();
        qDebug() << "KeyEvent: insert key:" << eventKey->key();
        */


        /*
        qDebug() << "KeyEvent: cursor pos:" << cursorPosition;
        qDebug() << "KeyEvent: cursor RECT:" << this->cursorRect();

        qDebug() << "KeyEvent: cursor 2ND pos:" << testSecondCursor->position();
        qDebug() << "KeyEvent: cursor 2NP RECT:" << this->cursorRect(*testSecondCursor);
        */
        return true;
      }
    };

    event->ignore();
    return QObject::eventFilter(target, event);
  }

  void CodeEditor::resizeByLines()
  {
    QString text = code->toPlainText();
    QRegExp rule("\n");
    int count = 1;
    int position = 0;

    while ((position = rule.indexIn(text, position)) != -1)
    {
      QStringRef subString(&text, position, rule.matchedLength());
      ++count;
      position += rule.matchedLength();
    }

    this->setFixedHeight(count * linePixelHeight + 10);

    //qDebug() << "CodeEditor::resizeByLines count of lines" << count;
    emit actHeightChanged();

  }

  void CodeEditor::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    //QPainter painter(viewport());
    QRect bounds = QRect(0, 0, this->width() - 1, this->height() - 1);
    QColor fillColor = activeColor;
    fillColor.setAlpha(backgroundAlpha);
    painter.fillRect(bounds, fillColor);

    if (this->hasFocus()) { painter.setPen(QPen(QColor(220, 30, 30), 3)); }
    else { painter.setPen(QPen(QColor(120, 30, 30), 1)); }
    /*
    if (this->hasFocus()) { painter.setPen(activeColor); }
    else { painter.setPen(QColor(120, 120, 120)); }
    */

    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, height() - 1, width(), height() - 1);

    // second cursor test
    /*
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
    */

    // painter.drawText(100, 20, this->objectName());

    //QTextEdit::paintEvent(event);
    QWidget::paintEvent(event);
  }

  void CodeEditor::resizeEvent(QResizeEvent *resizeEvent)
  {
    code->setGeometry(0, 0, this->width(), this->height());
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
