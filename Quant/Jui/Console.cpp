#include "Console.h"

namespace Jui
{

  Console::Console(QWidget *parent) : PanelNEW(parent)
  {
    //setEdgeControler(EdgeControler::Direction::LEFT, true);
    this->setTitle("Console");
    this->setMinimumWidth(400);

    text = new QTextEdit(this);
    text->setReadOnly(true);
    text->setOverwriteMode(false);
    text->setTabStopWidth(30);
    text->setFrameStyle(QFrame::NoFrame);

    this->setColorText(QColor(130, 130, 130));
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

  void Console::resizeEvent(QResizeEvent *event)
  {
    PanelNEW::resizeEvent(event); // send event to superclass
    text->setGeometry(10, 35, this->width() - 20, this->height() - 45);
  }

  Console::~Console()	{ }

}