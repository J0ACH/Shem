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
    this->setLibrary("user", "testName");
  }

  void QuantCore::setLibrary(QString key, QString value)
  {
    library.insert(key, value);
    emit actObjectChanged();
  }

  QString QuantCore::getLibrary(QString key)
  {
    return library.value(key);
  }

  void QuantCore::onMapChanged(QMap <QString, QVariant> map)
  {
    foreach(QString oneKey, map.keys())
    {
      qDebug() << "Core::onMapChanged [key: " << oneKey
        << " || value: " << map.value(oneKey).toString() << "]";
    }
  }


  void QuantCore::addNode(QString name)
  {
    QuantNode *testNode = new QuantNode(mCanvan, this);
    testNode->setGeometry(50, 50, 100, 100);
    testNode->show();
    testNode->setName(name);
  }

  QuantCore::~QuantCore() { }
}


