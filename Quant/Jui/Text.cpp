#include "Text.h"

namespace Jui
{

  Text::Text(QWidget *parent) : QWidget(parent)
  {
    colorText = QColor(240, 30, 30);
    align = Qt::AlignVCenter | Qt::AlignLeft; // Qt::AlignCenter;
  }

  void Text::setText(QString txt) { text = txt; }

  void Text::setAlign(Qt::Alignment alignment)  { align = alignment; }
  
  void Text::setColorText(QColor color) { colorText = color; }
  QColor Text::getColorText() { return colorText; }

  void Text::setFont(QFont f) { font = f; }
  QFont Text::getFont() { return font; }

  void Text::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);

    QRectF rect = QRectF(0, 0, this->width() - 1, this->height() - 1);

    QTextOption option;
    option.setAlignment(align);

    painter.setFont(font);
    painter.setPen(colorText);
    painter.drawText(rect, text, option);

#ifdef _DEBUG   // nevim, jestli to pobezi i mimo VS
    painter.setPen(Qt::red);
    painter.drawRect(rect);   
#endif
  }

  Text::~Text()	{ }

}