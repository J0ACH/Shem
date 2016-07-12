#include "QuantCore.h"

namespace QuantIDE
{
  QuantCore::QuantCore(CanvanNEW *canvan, Customize *customize) :
    QObject(canvan),
    mCanvan(canvan),
    mCustomize(customize),
    mBridge(new ScBridge(canvan)),
    mNetwork(new UDPServer(canvan, mBridge))
  {
    qDebug("Core init...");

    connect(mBridge, SIGNAL(interpretBootDoneAct()), this, SLOT(onInterpretBootDone()));
    connect(mBridge, SIGNAL(serverBootDoneAct()), this, SLOT(onServerBootDone()));
    mBridge->changeInterpretState();
  }

  void QuantCore::onInterpretBootDone()
  {
    qDebug("QuantCore::onInterpretBooted");
    mBridge->changeServerState();
  }
  void QuantCore::onServerBootDone()
  {
    qDebug("QuantCore::onServerBootDone");
  }


  void QuantCore::onMyMapSet(QMap <QString, QVariant> map)
  {
    foreach(QString oneKey, map.keys())
    {
      qDebug() << "Core::onMapChanged [key: " << oneKey
        << " || value: " << map.value(oneKey).toString() << "]";
    }
  }
  void QuantCore::onEvaluate(QString code)  { mBridge->evaluate(code); }

  void QuantCore::addProxySpace()
  {
    qDebug() << "Core::addProxySpace mCanvan->getScreen: " << mCanvan->centralWidget()->geometry();
    QuantProxy *proxy = new QuantProxy(mCanvan->centralWidget(), this);
    proxy->setGeometry(50, 25, 100, 100);
    proxy->show();

    //mCanvan->update();

    //proxy->setName("testProxy");
  }

  void QuantCore::addNode(QString name)
  {

    QuantNode *testNode = new QuantNode(mCanvan->centralWidget(), this);
    testNode->setGeometry(50, 150, 100, 100);
    testNode->show();
    testNode->setName(name);


  }

  QuantCore::~QuantCore()
  {
    qDebug("Core closing...");
    mBridge->killBridge();
    qDebug("Bridge killed...");
  }
}


