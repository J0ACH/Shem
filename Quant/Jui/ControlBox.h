#ifndef CONTROLBOX_H
#define CONTROLBOX_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

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

    void setLabelSize(int);

    public slots:
    //void setValue(QString text);

  signals :
    void actValueChanged(QString);
    
  protected:
   // void mousePressEvent(QMouseEvent *event);
   // void focusInEvent(QFocusEvent*);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    virtual bool eventFilter(QObject * watched, QEvent * event);

  private:
    QLabel *label;
    QLineEdit *value;
    QString oldValue;

    int labelSizeX;
    QColor colorText, colorBackground;

    void updateStyleSheet();
  };

}
#endif // CONTROLBOX_H
