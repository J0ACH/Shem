#ifndef TEXT_H
#define TEXT_H

#include <QLabel>
#include <QTextFormat>
#include <QColor>
#include <QDebug>

namespace Jui
{
  class Text : public QLabel
  {
    Q_OBJECT

  public:
    Text(QWidget *parent=0);
    ~Text();

    void setColorText(QColor);
   
  };

}
#endif // TEXT_H
