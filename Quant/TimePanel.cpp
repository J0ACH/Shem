#include "TimePanel.h"

namespace QuantIDE
{
  TimePanel::TimePanel(QWidget *parent) :
    PanelNEW(parent),
    mProxy(NULL)
  {
    this->setMinimumWidth(400);
    this->setMinimumHeight(300);
    this->initControl();
  }

  void TimePanel::initControl()
  {
    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);

    scrollWidget = new QWidget(this);
    scrollWidget->setStyleSheet("background-color: rgba(0,0,0,0)");
    scrollWidget->setGeometry(0, 0, 300, 1000);

    scrollArea->setWidget(scrollWidget);
  }

  void TimePanel::insertProxy(QuantProxy* proxy)  { mProxy = proxy; }

  void TimePanel::resizeEvent(QResizeEvent *event)
  {
    scrollArea->setGeometry(10, 130, width() - 20, height() - 170);
    scrollWidget->setFixedWidth(scrollArea->width() - 10);

    if (mProxy != NULL)
    {
      mProxy->setGeometry(10, 30, width() - 20, 250);
    }


    scrollWidget->setFixedHeight(210); // end of scroll

    PanelNEW::resizeEvent(event);
  }

  TimePanel::~TimePanel() { }
}