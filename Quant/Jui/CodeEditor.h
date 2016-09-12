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

  class CodeEditor : public QWidget
  {
    Q_OBJECT

  public:
    CodeEditor(QWidget *parent);
    ~CodeEditor();

    void setBackground(const QColor &);
    void setFontCode(QFont);

    int getLinePixelHeight();
    void codeSnippet();

    void setText(QString);

    public slots:
    void onTextChanged();
    void onCursorPositionChanged();
    void onChangeExtraCursor(QString name, int position);
    void onEvaluate(QColor);

    void alphaUpdate();

  signals:
    void actValueInsert(int, int, QString);
    void actValueChanged(QString);
    void actValueEvaluate(QString);
    void actValueChanged(QString objectName, QString code);
    void actValueEvaluate(QString objectName, QString code);
    void actCursorMoved(int);
    void actHeightChanged();
    void actPreviousFocused(QWidget*);
    void actNextFocused(QWidget*);
    void actParentFocused(QWidget*);

  protected:
    virtual bool eventFilter(QObject * watched, QEvent * event);
    void paintEvent(QPaintEvent *paintEvent);
    void resizeEvent(QResizeEvent *resizeEvent);
    
  private:
    QTextEdit *code;
    enum HighLights { CLASS, CONTROL, NODE, DIGIT };

    int linePixelHeight;

    int cursorPosition;
    // QTextCursor *testSecondCursor;
    // QRect testSecondCursorRect;

    int fadeTimeOut;
    int fps;
    int backgroundAlpha;
    QTimer *timer;

    QColor normalColor, overColor, activeColor;

    QList<QList<QVariant>*> regexpText(HighLights type);
    void highlightText(int, int, QTextCharFormat);

    void resizeByLines();

    // QMap<QString, QTextCursor*> extraCursors;
  };

}
#endif // CONSOLE_H

