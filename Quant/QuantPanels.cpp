#include "QuantPanels.h"

namespace QuantIDE
{
  QuantPanel::QuantPanel(QWidget *parent) : PanelNEW(parent)
  {
    this->setMinimumWidth(400);
    this->setMinimumHeight(150);
    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);
  }

  void QuantPanel::setScrollWidget(QWidget *widget)  { scrollArea->setWidget(widget); }
  QWidget* QuantPanel::getScrollWidget()  { return  scrollArea->widget(); }

  void QuantPanel::resizeEvent(QResizeEvent *event)
  {
    scrollArea->setGeometry(10, 30, width() - 20, height() - 20);
    scrollArea->widget()->setFixedWidth(scrollArea->width() - 10);

    PanelNEW::resizeEvent(event);
  }

  QuantPanel::~QuantPanel() { }
}