#ifndef TEXT_H
#define TEXT_H

#include <QDebug>
#include <QWidget>
#include <QPainter>

namespace Jui
{
  class Text : public QWidget
  {
    Q_OBJECT
      Q_PROPERTY(QColor colorText READ getColorText WRITE setColorText)
      Q_PROPERTY(QFont font READ getFont WRITE setFont)

  public:
    Text(QWidget *parent = 0);
    ~Text();

    void setText(QString);
    QString getText();
    void setAlign(Qt::Alignment alignment);

    void setColorText(QColor);
    QColor getColorText();

    void setFont(QFont);
    QFont getFont();

  protected:
    void paintEvent(QPaintEvent *event);

  private:
    QString text;
    QColor colorText;
    QFont font;
    Qt::Alignment align;
  };
}
#endif // TEXT_H

