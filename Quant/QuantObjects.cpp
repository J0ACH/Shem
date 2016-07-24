#include "QuantObjects.h"

namespace QuantIDE
{
  QuantObject::QuantObject(QWidget *parent, QObject *core) : QWidget(parent)
  {
    qDebug("QuantObject init...");

    connect(this, SIGNAL(actEvaluate(QString)), core, SLOT(onEvaluate(QString)));
  }

  void QuantObject::onNetworkDataRecived(DataNEW data)
  {
    qDebug("QuantObject::onDataRecived");
  }

  void QuantObject::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setPen(QColor(120, 30, 30));
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

    // painter.drawText(5, 15, this->getMap_string("QuantObjectType"));
  }

  QuantObject::~QuantObject() { }


  // QUANT PROXYSPACE ////////////////////////////////////////////////////////////////  

  QuantProxy::QuantProxy(QWidget *parent, QObject *core) : QuantObject(parent, core)
  {
    data.setValue(DataProxy::TEMPO, 127);

    qDebug("QuantProxy init...");
  
    testButton = new Button(this);
    testButton->setGeometry(5, 50, this->width() - 10, 20);
    testButton->setText("beep");
    testButton->setStateKeeping(Button::StateKeeping::HOLD);
    connect(testButton, SIGNAL(pressAct()), this, SLOT(onBeep()));

    nameBox = new ControlBox(this);
    nameBox->setGeometry(5, 80, 200, 50);
    nameBox->setLabel("tempo");
    nameBox->setValue(data.getValue_string(DataProxy::TEMPO));
    connect(nameBox, SIGNAL(actValueChanged(QString)), this, SLOT(onControlTEST(QString)));
    connect(nameBox, SIGNAL(actValueEvaluate(QString)), this, SLOT(onControlTEST(QString)));

  }

  void QuantProxy::onBeep()
  {

    qDebug("QuantProxy::onBeep");
    // emit actEvaluate("().play");
  }

  void QuantProxy::onControlTEST(QString txt)
  {
    qDebug("QuantProxy::onControlTEST");
    data.setValue(DataProxy::TEMPO, txt);
    emit actDataSend(data);
  }

  void QuantProxy::onDataRecived(DataProxy data)
  {
    qDebug("QuantProxy::onDataRecived");
    data.print("QuantProxy::onDataRecived");

    nameBox->setValue(data.getValue_string(DataProxy::TEMPO));
  }

  QuantProxy::~QuantProxy() { }

  // QUANT NODEPROXY ////////////////////////////////////////////////////////////////

  QuantNode::QuantNode(QWidget *parent, QObject *core) : QuantObject(parent, core)
  {
    qDebug("QuantNode init...");

    nameBox = new ControlBox(this);
    nameBox->setGeometry(5, 50, 90, 20);
    nameBox->setLabel("name");
    // nameBox->setValue(this->getMap_string("name"));
  }
  void QuantNode::setName(QString name)
  {
    // this->setMap("name", name);
    // nameBox->setValue(this->getMap_string("name"));
  }
  QuantNode::~QuantNode() { }

  // QUANT CONTROLS ////////////////////////////////////////////////////////////////

  QuantControl::QuantControl(QWidget *parent, QObject *core) : QuantObject(parent, core)
  {
    qDebug("QuantControl init...");
    // this->setMap("QuantObjectType", "QuantControl");
  }
  QuantControl::~QuantControl() { }

  // QUANT BUS ////////////////////////////////////////////////////////////////

  QuantBus::QuantBus(QWidget *parent, QObject *core) : QuantObject(parent, core)
  {
    qDebug("QuantBus init...");
    // this->setMap("QuantObjectType", "QuantBus");
  }
  QuantBus::~QuantBus() { }

}


