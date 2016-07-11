#include "QuantObjects.h"

namespace QuantIDE
{
  QuantObject::QuantObject(QObject *parent) : QObject(parent)
  {
    qDebug("QuantObject init...");

    objectType = BASE;

    connect(parent, SIGNAL(actObjectChanged()), this, SLOT(onObjectChanged()));
    connect(
      this, SIGNAL(actMapChanged(QMap <QString, QVariant>)),
      parent, SLOT(onMapChanged(QMap <QString, QVariant>))
      );
  }

  void QuantObject::setMap(QString key, QString value)
  {
    map.insert(key, QVariant(value));
    emit actMapChanged(map);
  }
  void QuantObject::setMap(QString key, ObjectType value)
  {
    map.insert(key, QVariant(value));
    emit actMapChanged(map);
  }

  void QuantObject::onObjectChanged()
  {
    qDebug() << "QuantObject::onObjectChanged()";
  }

  void QuantObject::printMap()
  {
    foreach(QString oneKey, map.keys())
    {
      qDebug() << "QuantObject::printMap [key: " << oneKey
        << " || value: " << map.value(oneKey).toString() << "]";
    }
  }

  QuantObject::~QuantObject()
  {
  }


  // QUANT PROXYSPACE ////////////////////////////////////////////////////////////////  

  QuantProxy::QuantProxy(QObject *parent) : QuantObject(parent)
  {
    qDebug("QuantProxy init...");
    objectType = PROXYSPACE;
    
  }
  QuantProxy::~QuantProxy()
  {
  }

  // QUANT NODEPROXY ////////////////////////////////////////////////////////////////

  QuantNode::QuantNode(QObject *parent) : QuantObject(parent)
  {
    qDebug("QuantNode init...");
    objectType = NODEPROXY;
    this->setMap("QuantObjectType", objectType);
  }
  QuantNode::~QuantNode()
  {
  }

  // QUANT CONTROLS ////////////////////////////////////////////////////////////////

  QuantControl::QuantControl(QObject *parent) : QuantObject(parent)
  {
    qDebug("QuantControl init...");
    objectType = ENVCONTROL;
  }
  QuantControl::~QuantControl()
  {
  }

  // QUANT BUS ////////////////////////////////////////////////////////////////

  QuantBus::QuantBus(QObject *parent) : QuantObject(parent)
  {
    qDebug("QuantBus init...");
    objectType = BUS;
  }
  QuantBus::~QuantBus()
  {
  }
}


