#include "Console.h"

namespace Jui
{

  Console::Console(QWidget *parent) : PanelNEW(parent)
  {
    //setEdgeControler(EdgeControler::Direction::LEFT, true);
    this->setTitle("Console");
    this->setMinimumWidth(500);

    text = new QTextEdit(this);
    text->setReadOnly(true);
    text->setOverwriteMode(false);
    text->setTabStopWidth(30);

    this->setColorText(QColor(130, 130, 130));

    text->setFrameStyle(QFrame::NoFrame);

    //    txt.append(tr("QTextEdit { selection-background-color: %1; }").arg(colorActive.name()));

    text->append(tr("Console init..."));

    QTextCharFormat format;
    format.setForeground(QBrush(QColor(230, 30, 30)));
    text->setCurrentCharFormat(format);

    //connect(this, SIGNAL(resizeAct()), this, SLOT(fitGeometry()));
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

  void Console::setColorBackground(QColor color)
  {
    PanelNEW::setColorBackground(color);
    QWidget *viewport = text->viewport();
    QPalette palete = viewport->palette();
    palete.setColor(viewport->backgroundRole(), color);
    viewport->setPalette(palete);
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

  void Console::setColorNormal(QColor color) { this->colorMsgNormal = color; }
  void Console::setColorStatus(QColor color) { this->colorMsgStatus = color; }
  void Console::setColorEvaluate(QColor color) { this->colorMsgEvaluate = color; }
  void Console::setColorResult(QColor color) { this->colorMsgResult = color; }
  void Console::setColorError(QColor color) { this->colorMsgError = color; }
  void Console::setColorWarning(QColor color) { this->colorMsgWarning = color; }
  void Console::setColorBundle(QColor color) { this->colorMsgBundle = color; }

  void Console::onMsgNormal(QString msg)  { this->addText(msg, colorMsgNormal); }
  void Console::onMsgStatus(QString msg)  { this->addText(msg, colorMsgStatus); }
  void Console::onMsgEvaluate(QString msg)  { this->addText(msg, colorMsgEvaluate); }
  void Console::onMsgResult(QString msg)  { this->addText(msg, colorMsgResult); }
  void Console::onMsgError(QString msg)  { this->addText(msg, colorMsgError); }
  void Console::onMsgWarning(QString msg)  { this->addText(msg, colorMsgWarning); }
  void Console::onMsgBundle(QString msg)  { this->addText(msg, colorMsgBundle); }
  
  void Console::resizeEvent(QResizeEvent *event)
  {
    PanelNEW::resizeEvent(event); // send event to superclass
    text->setGeometry(10, 35, this->width() - 20, this->height() - 45);
  }

  Console::~Console()	{ }

}