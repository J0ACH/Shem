#include "NetworkPanel.h"

namespace QuantIDE
{
  NetworkPanel::NetworkPanel(QWidget *parent) : PanelNEW(parent)
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

    buttonConnect = new Button(this);
    buttonConnect->setText("Connect");
    connect(buttonConnect, SIGNAL(actPressed()), this, SLOT(onConnectPressed()));
  }


  void NetworkPanel::addProfile(QuantUser *yourProfile)
  {
    profiles.insert(yourProfile->getName(), yourProfile);
    this->updateProfilesPosition();
  }
  void NetworkPanel::removeProfile(QString name)
  {
    profiles.remove(name);
    this->updateProfilesPosition();
  }


  void NetworkPanel::onConnectPressed()
  {
    qDebug("NetworkPanel::onConnectPressed()");
  }


  void NetworkPanel::resizeEvent(QResizeEvent *event)
  {
    scrollArea->setGeometry(10, 130, width() - 20, height() - 170);
    scrollWidget->setFixedWidth(scrollArea->width() - 10);

    if (height() < 60) { buttonConnect->hide(); }
    else { buttonConnect->show(); }
    buttonConnect->setGeometry(width() - 100, height() - 30, 80, 20);

    this->updateProfilesPosition();

    scrollWidget->setFixedHeight(310); // end of scroll

    PanelNEW::resizeEvent(event);
  }

  void NetworkPanel::updateProfilesPosition()
  {
    int noLoop = 0;
    foreach(QuantUser *oneUser, profiles)
    {
      oneUser->setGeometry(10, noLoop * 40 + 30, width() - 20, oneUser->height());
      noLoop++;
    }
  }


  NetworkPanel::~NetworkPanel() { }
}