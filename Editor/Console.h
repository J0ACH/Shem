#ifndef CONSOLE_H
#define CONSOLE_H

//#include <QtWidgets/QMainWindow>
//#include <QtWidgets/QDockWidget>
#include <QTextEdit>
#include <QtGui>

class Console : public QTextEdit {
  Q_OBJECT

public:
  Console(QWidget *parent);

  // void addLine(QString &text);

  ~Console();

public slots:
  void addLine(QString newLine);

protected:
  // void paintEvent(QPaintEvent *event);

private:
  QTextEdit *text;
};

#endif // CONSOLE_H
