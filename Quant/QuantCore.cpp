#include "QuantCore.h"

namespace QuantIDE
{
  QuantCore::QuantCore(QWidget *parent, Customize *customize) :
    QObject(parent),
    mCustomize(customize),
    mCanvan(parent),
    mBridge(new ScBridge(parent)),
    mNetwork(new UDPServer(parent, mBridge))
  {
    qDebug("Core init...");
  }

  void QuantCore::onMapChanged(QMap <QString, QVariant> map)
  {
    foreach(QString oneKey, map.keys())
    {
      qDebug() << "Core::onMapChanged [key: " << oneKey
        << " || value: " << map.value(oneKey).toString() << "]";
    }
  }

  void QuantCore::addProxySpace()
  {
    QuantProxy *proxy = new QuantProxy(mCanvan, this);
    proxy->setGeometry(50, 25, 100, 100);
    proxy->show();
    //proxy->setName("testProxy");
  }

  void QuantCore::addNode(QString name)
  {
    QuantNode *testNode = new QuantNode(mCanvan, this);
    testNode->setGeometry(50, 150, 100, 100);
    testNode->show();
    testNode->setName(name);
  }

  QuantCore::~QuantCore() { }
}


