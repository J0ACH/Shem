#ifndef CONTROLBOX_H
#define CONTROLBOX_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPainter>

namespace Jui
{
  class ControlBox : public QWidget
  {
    Q_OBJECT

  public:
    ControlBox(QWidget *parent);
    ~ControlBox();

    QRect bounds();

    void setFont(QFont);
    void setColorText(QColor);
    void setColorBackground(QColor);

    public slots:
   // void addText(QString text, QColor color, bool newLine);
    
  protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

  private:
    QLabel *txt;
    QLineEdit *val;

    QColor colorText, colorBackground;

    void updateStyleSheet();
  };

}
#endif // CONTROLBOX_H
