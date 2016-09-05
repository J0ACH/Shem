#ifndef CONSOLE_H
#define CONSOLE_H

#include "Panel.h"
#include <QWidget>
#include <QTextEdit>
#include <QScrollBar>  

namespace Jui
{

    class Console : public Panel
  {
    Q_OBJECT
      Q_PROPERTY(QFont fontConsole READ getFontConsole WRITE setFontConsole)

  public:
    Console(QWidget *parent = 0);
    ~Console();

    void setFontConsole(QFont);
    QFont getFontConsole();

    void setColorText(QColor);
    void setColorBackground(QColor);
    
    public slots:
    void addText(QString text, QColor color, bool newLine);
  
  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    QTextEdit *text;  
    QFont fontConsole;
  };

}
#endif // CONSOLE_H
