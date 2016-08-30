#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "Panel.h"
#include <QWidget>
#include <QTextEdit>
#include <QList>
#include <QVariant>
#include <QDebug>

namespace Jui
{

  class CodeEditor : public QTextEdit
  {
    Q_OBJECT

  public:
    CodeEditor(QWidget *parent);
    ~CodeEditor();

    void setBackground(const QColor &);
    void setFontCode(QFont);

    public slots:
    //void setCode(QString text);
    void onChangeExtraCursor(QString name, int position);
    void onEvaluate(QColor);

    void fitTextFormat();
    void alphaUpdate();

  signals:
    void actValueChanged(QString);
    void actValueEvaluate(QString);
    void actValueChanged(QString objectName, QString code);
    void actValueEvaluate(QString objectName, QString code);
    void actCursorMoved(int);

    // bude odstraneno
    void evaluateAct();
    void sendText(QString);

  protected:
    virtual bool eventFilter(QObject * watched, QEvent * event);
    void paintEvent(QPaintEvent *paintEvent);

  private:
    enum HighLights { CLASS, CONTROL, NODE, DIGIT };

    int cursorPosition;
    QTextCursor *testSecondCursor;
    QRect testSecondCursorRect;

    int fadeTimeOut;
    int fps;
    int backgroundAlpha;
    QTimer *timer;

    QColor normalColor, overColor, activeColor;

    QList<QList<QVariant>*> regexpText(HighLights type);
    void highlightText(int, int, QTextCharFormat);

    QMap<QString, QTextCursor*> extraCursors;
  };

}
#endif // CONSOLE_H

