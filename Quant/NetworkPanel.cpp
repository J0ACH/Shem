#include "NetworkPanel.h"

namespace QuantIDE
{
  NetworkPanel::NetworkPanel(QWidget *parent) : PanelNEW(parent)
  {
    this->setMinimumWidth(400);
    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);
  }

  void NetworkPanel::setScrollWidget(QWidget *widget)  { scrollArea->setWidget(widget); }
  QWidget* NetworkPanel::getScrollWidget()  { return  scrollArea->widget(); }

  void NetworkPanel::resizeEvent(QResizeEvent *event)
  {
    scrollArea->setGeometry(10, 30, width() - 20, height() - 40);
    scrollArea->widget()->setFixedWidth(scrollArea->width() - 10);

    PanelNEW::resizeEvent(event);
  }

  NetworkPanel::~NetworkPanel() { }
}