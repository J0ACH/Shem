#include "QuantPanels.h"

namespace QuantIDE
{
  // NETWORK PANEL //////////////////////////////////////////////////////////////////////////////////////

  NetworkPanel::NetworkPanel(QWidget *parent) : PanelNEW(parent)
  {
    this->setMinimumWidth(400);
    this->setMinimumHeight(150);
    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);
  }

  void NetworkPanel::setScrollWidget(QWidget *widget)  { scrollArea->setWidget(widget); }
  QWidget* NetworkPanel::getScrollWidget()  { return  scrollArea->widget(); }

  void NetworkPanel::resizeEvent(QResizeEvent *event)
  {
    scrollArea->setGeometry(10, 30, width() - 20, height() - 20);
    scrollArea->widget()->setFixedWidth(scrollArea->width() - 10);

    PanelNEW::resizeEvent(event);
  }

  NetworkPanel::~NetworkPanel() { }

  // TIME PANEL //////////////////////////////////////////////////////////////////////////////////////

  TimePanel::TimePanel(QWidget *parent) :    PanelNEW(parent)
   // mProxy(NULL)
  {
    this->setMinimumWidth(400);
    this->setMinimumHeight(150);
    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);
  }

  /*
  void TimePanel::initControl()
  {
    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);

    scrollWidget = new QWidget(this);
    scrollWidget->setStyleSheet("background-color: rgba(0,0,0,0)");
    scrollWidget->setGeometry(0, 0, 300, 1000);

    scrollArea->setWidget(scrollWidget);
  }
  */

  //void TimePanel::insertProxy(QuantProxy* proxy)  { mProxy = proxy; }

  void TimePanel::setScrollWidget(QWidget *widget)  { scrollArea->setWidget(widget); }

  void TimePanel::resizeEvent(QResizeEvent *event)
  {
    scrollArea->setGeometry(10, 30, width() - 20, height() - 20);
    scrollArea->widget()->setFixedWidth(scrollArea->width() - 10);

    /*
    if (mProxy != NULL)
    {
      mProxy->setGeometry(10, 30, width() - 20, 250);
    }
    */


    //scrollWidget->setFixedHeight(210); // end of scroll

    PanelNEW::resizeEvent(event);
  }

  TimePanel::~TimePanel() { }

}