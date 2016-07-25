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

    buttonConnect = new Button(this);
    buttonConnect->setText("Connect");
    buttonConnect->setStateKeeping(Button::StateKeeping::HOLD);
    buttonConnect->setState(Button::State::ON);
    connect(buttonConnect, SIGNAL(actPressed()), this, SLOT(onConnectPressed()));
  }

  void NetworkPanel::onConnectPressed()
  {
    qDebug("NetworkPanel::onConnectPressed()");
    if (buttonConnect->getState() == Button::State::ON) { emit actNetworkConnect(); }
    else { emit actNetworkDisconnect(); }
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
    foreach(QString oneName, mProfiles->keys())
    {
      mProfiles->value(oneName)->setGeometry(10, noLoop * 40 + 30, width() - 20, 30);
      noLoop++;
    }
  }


  NetworkPanel::~NetworkPanel() { }
}