#include "NetworkPanel.h"

namespace QuantIDE
{
  NetworkPanel::NetworkPanel(QWidget *parent, QMap<QString, QuantUser*> *lib) :
    PanelNEW(parent),
    mProfiles(lib)
  {
    this->setMinimumWidth(400);
    this->initControl();
  }

  void NetworkPanel::initControl()
  {
    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);

    scrollWidget = new QWidget(this);
    scrollWidget->setStyleSheet("background-color: rgba(0,0,0,0)");
    scrollWidget->setGeometry(0, 0, 300, 1000);

    scrollArea->setWidget(scrollWidget);
    }
 

  void NetworkPanel::resizeEvent(QResizeEvent *event)
  {
    scrollArea->setGeometry(10, 130, width() - 20, height() - 170);
    scrollWidget->setFixedWidth(scrollArea->width() - 10);

    this->updateProfilesPosition();

    scrollWidget->setFixedHeight(310); // end of scroll

    PanelNEW::resizeEvent(event);
  }

  void NetworkPanel::updateProfilesPosition()
  {
    int noLoop = 0;
    foreach(QString oneName, mProfiles->keys())
    {
      mProfiles->value(oneName)->setGeometry(10, noLoop * 40 + 30, width() - 20, 30);
      noLoop++;
    }
  }


  NetworkPanel::~NetworkPanel() { }
}