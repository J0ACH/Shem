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
    scrollWidget->setGeometry(0, 0, scrollArea->width(), 1000);

    scrollArea->setWidget(scrollWidget);
  }

  QWidget* NetworkPanel::getScrollWidget()  { return scrollWidget; }

  void NetworkPanel::resizeEvent(QResizeEvent *event)
  {
    scrollArea->setGeometry(10, 30, width() - 20, height() - 40);
    scrollWidget->setFixedWidth(scrollArea->width() - 10);

    this->updateProfilesPosition();
    PanelNEW::resizeEvent(event);
  }

  void NetworkPanel::updateProfilesPosition()
  {
    int noLoop = 0;
    int endY = 0;
    foreach(QString oneName, mProfiles->keys())
    {
      int originY = noLoop * (mProfiles->value(oneName)->height() + 10) + 10;
      int heightY = mProfiles->value(oneName)->height();
      mProfiles->value(oneName)->setGeometry(
        10,
        originY,
        scrollArea->width() - 30,
        heightY
        );
      endY = originY + heightY;
      noLoop++;
    }
    scrollWidget->setFixedHeight(endY + 10);
  }

  NetworkPanel::~NetworkPanel() { }
}