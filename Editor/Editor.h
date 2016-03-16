
#ifndef EDITOR_H
#define EDITOR_H

#include "Console.h"
#include "ScBridge.h"
#include <QObject>
#include <QPlainTextEdit>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;

using namespace SupercolliderBridge;

class CodeEditor : public QPlainTextEdit {
  Q_OBJECT

public:
  CodeEditor(QWidget *parent = 0);

  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();

public slots:
  void sendCode();
  void consoleAddMsg(QString);

signals:
  void sendCode(QString);

protected:
  void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
  void updateLineNumberAreaWidth(int newBlockCount);
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect &, int);

private:
  QWidget *lineNumberArea;
  ScBridge *bridge;
  QTextEdit *console;
};

class LineNumberArea : public QWidget {
public:
  LineNumberArea(CodeEditor *editor) : QWidget(editor) { codeEditor = editor; }

  QSize sizeHint() const Q_DECL_OVERRIDE {
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
  }

protected:
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
    codeEditor->lineNumberAreaPaintEvent(event);
  }

private:
  CodeEditor *codeEditor;
};

#endif

/*
#ifndef DEMO_H
#define DEMO_H


#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

#include "ScBridge.h"

using namespace SupercolliderBridge;

namespace DemoIDE {


        class Demo : public QWidget
        {
                Q_OBJECT

        public:
                Demo();
                ~Demo();

        public slots:
                void beep();
                void sendCode();
                void consoleAddMsg(QString);

        signals:
                void sendCode(QString);

        protected:
                void closeEvent(QCloseEvent *event);

        private:
                ScBridge *bridge;

                void initControlers();

                QPushButton *buttStartLang;
                QPushButton *buttStartServer;
                QPushButton *buttKillLang;
                QPushButton *buttKillServer;
                QPushButton *buttBeep;
                QPushButton *buttCode;

                QLabel *runningLang;
                QLabel *runningServer;

                QTextEdit *console;
                QTextEdit *editor;


        };

}

#endif // DEMO
*/
