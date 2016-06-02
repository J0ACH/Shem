#ifndef CONTROLBOX_H
#define CONTROLBOX_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
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

    void setLabel(QString);
    double getValue();

    void setFont(QFont);
    void setColorText(QColor);
    void setColorBackground(QColor);
    void setLabelSize(int);

  public slots:
    void setValue(QString text);
    void alphaUpdate();

  signals:
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

    int fadeTimeOut;
    int fps;
    int backgroundAlpha;
    QTimer *timer;

    void updateStyleSheet();
  };

}
#endif // CONTROLBOX_H
