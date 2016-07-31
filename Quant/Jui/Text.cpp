#include "Text.h"

#define DISPLAY_FRAMES 0

namespace Jui
{

  Text::Text(QWidget *parent) : QWidget(parent)
  {
    colorText = QColor(240, 30, 30);
    align = Qt::AlignVCenter | Qt::AlignLeft; 
  }

  void Text::setText(QString txt) { text = txt; }
  QString Text::getText() { return text; }

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

#if DISPLAY_FRAMES
    painter.setPen(Qt::red);
    painter.drawRect(rect);   
#endif
  }

  Text::~Text()	{ }

}