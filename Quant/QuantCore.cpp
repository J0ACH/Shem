#include "QuantCore.h"

namespace QuantIDE
{
  QuantCore::QuantCore(QObject *parent, ScBridge *bridge, UDPServer *network, Customize *customize) :
    QObject(parent),
    mBridge(bridge),
    mNetwork(network),
    mCustomize(customize)
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

  QString QuantCore::printLibrary()
  {
    QString printTxt;
    foreach(QString oneValue, library.values())
    {
      printTxt += tr("\t-> %1\n").arg(oneValue);
    }
    return printTxt;
  }

  void QuantCore::addNode(QString name)
  {

    QuantNode testNode(this);
    testNode.setMap(name, "QuantNode");

    // testNode->setLibrary("aaa", "reverseTest");

  }

  QuantCore::~QuantCore() { }
}


