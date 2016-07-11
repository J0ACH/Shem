#include "QuantObjects.h"

namespace QuantIDE
{
  QuantObject::QuantObject(QWidget *parent, QObject *core) : QWidget(parent)
  {
    qDebug("QuantObject init...");

    this->setMap("QuantObjectType", "QuantObject");

    connect(core, SIGNAL(actObjectChanged()), this, SLOT(onObjectChanged()));
    connect(
      this, SIGNAL(actMapChanged(QMap <QString, QVariant>)),
      core, SLOT(onMapChanged(QMap <QString, QVariant>))
      );
  }

  void QuantObject::setMap(QString key, QString value)
  {
    map.insert(key, QVariant(value));
    emit actMapChanged(map);
  }

  QString QuantObject::getMap_string(QString key)  { return map.value(key).toString(); }

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

  void QuantObject::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setPen(QColor(120, 30, 30));
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
    //painter.fillRect(QRect(0, 0, width() - 1, height() - 1), QColor(120,30,30));

    painter.drawText(5, 15, this->getMap_string("QuantObjectType"));
  }

  QuantObject::~QuantObject()
  {
  }


  // QUANT PROXYSPACE ////////////////////////////////////////////////////////////////  

  QuantProxy::QuantProxy(QWidget *parent, QObject *core) : QuantObject(parent, core)
  {
    qDebug("QuantProxy init...");
    this->setMap("QuantObjectType", "QuantProxy");
  }
  QuantProxy::~QuantProxy()
  {
  }

  // QUANT NODEPROXY ////////////////////////////////////////////////////////////////

  QuantNode::QuantNode(QWidget *parent, QObject *core) : QuantObject(parent, core)
  {
    qDebug("QuantNode init...");
    this->setMap("QuantObjectType", "QuantNode");

    nameBox = new ControlBox(this);
    nameBox->setGeometry(5, 50, 90, 20);
    nameBox->setLabel("name");
    nameBox->setValue(this->getMap_string("name"));
    nameBox->setColorText(QColor(120, 30, 30));
  }
  void QuantNode::setName(QString name)
  {
    this->setMap("name", name);
    nameBox->setValue(this->getMap_string("name"));
  }
  QuantNode::~QuantNode()
  {
  }

  // QUANT CONTROLS ////////////////////////////////////////////////////////////////

  QuantControl::QuantControl(QWidget *parent, QObject *core) : QuantObject(parent, core)
  {
    qDebug("QuantControl init...");
    this->setMap("QuantObjectType", "QuantControl");
  }
  QuantControl::~QuantControl()
  {
  }

  // QUANT BUS ////////////////////////////////////////////////////////////////

  QuantBus::QuantBus(QWidget *parent, QObject *core) : QuantObject(parent, core)
  {
    qDebug("QuantBus init...");
    this->setMap("QuantObjectType", "QuantBus");
  }
  QuantBus::~QuantBus()
  {
  }
}


