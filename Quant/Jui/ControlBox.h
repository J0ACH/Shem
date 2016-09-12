#ifndef CONTROLBOX_H
#define CONTROLBOX_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QDebug>

#include "Text.h"

namespace Jui
{
  class ControlBox : public QWidget
  {
    Q_OBJECT
      Q_PROPERTY(QColor colorNormal READ getColorNormal WRITE setColorNormal)
      Q_PROPERTY(QColor colorOver READ getColorOver WRITE setColorOver)
      Q_PROPERTY(QColor colorActive READ getColorActive WRITE setColorActive)
      Q_PROPERTY(QColor colorText READ getColorText WRITE setColorText)
      Q_PROPERTY(QFont font READ getFont WRITE setFont)

  public:
    ControlBox(QWidget *parent);
    ~ControlBox();

    QRect bounds();
    QPoint origin();
    void setLabel(QString);

    void enterFocus();
    void exitFocus();

    QString getValue_string();
    int getValue_int();
    double getValue_double();
    bool getValue_bool();

    void setColorNormal(QColor);
    void setColorOver(QColor);
    void setColorActive(QColor);
    void setColorText(QColor);
    void setFont(QFont);

    QColor getColorNormal();
    QColor getColorOver();
    QColor getColorActive();
    QColor getColorText();
    QFont getFont();

    void setLabelSize(int);

    public slots:
    void setValue(QString text);

  signals:
    void actValueChanged(QString);
    void actValueEvaluate(QString);
    void actPreviousFocused(QWidget*);
    void actNextFocused(QWidget*);
    void actParentFocused(QWidget*);

  protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    virtual bool eventFilter(QObject * watched, QEvent * event);
    void focusInEvent(QFocusEvent * event);

  private:
    Text *label;
    QLineEdit *value;
    QString oldValue;

    int labelSizeX;
    QColor colorNormal, colorOver, colorActive, colorText;
    QFont font;

    int fadeTimeOut;
    int fps;
    int backgroundAlpha;
    QTimer *timer;

    private slots:
    void alphaUpdate();
  };

}
#endif // CONTROLBOX_H

