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

    void setColorMsgNormal(QColor);
    void setColorMsgStatus(QColor);
    void setColorMsgEvaluate(QColor);
    void setColorMsgAnswer(QColor);
    void setColorMsgError(QColor);
    void setColorMsgWarning(QColor);
    void setColorMsgBundle(QColor);

    public slots:
    void addText(QString text, QColor color, bool newLine);

    void onMsgNormal(QString);
    void onMsgStatus(QString);
    void onMsgEvaluate(QString);
    void onMsgAnswer(QString);
    void onMsgError(QString);
    void onMsgWarning(QString);
    void onMsgBundle(QString);

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    QTextEdit *text;

    QColor colorMsgNormal, colorMsgStatus, colorMsgEvaluate, colorMsgAnswer, colorMsgError, colorMsgWarning, colorMsgBundle;
  };

}
#endif // CONSOLE_H
