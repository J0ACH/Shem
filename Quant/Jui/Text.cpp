#include "Text.h"

namespace Jui
{

  Text::Text(QWidget *parent) : QLabel(parent)  {  }

  void Text::setColorText(QColor color)  { this->setStyleSheet(QString("QLabel { color : %1; }").arg(color.name())); }

  Text::~Text()	{ }

}