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

    void setColorNormal(QColor);
    void setColorStatus(QColor);
    void setColorEvaluate(QColor);
    void setColorResult(QColor);
    void setColorError(QColor);
    void setColorWarning(QColor);
    void setColorBundle(QColor);

    public slots:
    void addText(QString text, QColor color, bool newLine);

    void onMsgNormal(QString);
    void onMsgStatus(QString);
    void onMsgEvaluate(QString);
    void onMsgResult(QString);
    void onMsgError(QString);
    void onMsgWarning(QString);
    void onMsgBundle(QString);

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    QTextEdit *text;

    QColor colorMsgNormal, colorMsgStatus, colorMsgEvaluate, colorMsgResult, colorMsgError, colorMsgWarning, colorMsgBundle;
  };

}
#endif // CONSOLE_H
