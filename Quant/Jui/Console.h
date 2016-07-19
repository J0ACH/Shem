#ifndef CONSOLE_H
#define CONSOLE_H

#include "Panel.h"
#include <QWidget>
#include <QTextEdit>
#include <QScrollBar>
  

namespace Jui
{

    class Console : public PanelNEW
  {
    Q_OBJECT

  public:
    Console(QWidget *parent = 0);
    ~Console();

    void setFont(QFont);
    void setColorText(QColor);
    void setColorBackground(QColor);
    
    public slots:
    void addText(QString text, QColor color, bool newLine);
  
  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    QTextEdit *text;

    
  };

}
#endif // CONSOLE_H
