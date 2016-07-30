#include "QuantObjects.h"

namespace QuantIDE
{
  QuantObject::QuantObject(QWidget *parent, QObject *core) :
    QWidget(parent),
    mCanvan(parent),
    mCore(core)
  {
    qDebug("QuantObject init...");
      connect(this, SIGNAL(actDataChanged(DataNEW)), core, SLOT(onObjectDataChanged(DataNEW)));
    connect(this, SIGNAL(actEvaluate(QString)), core, SLOT(onEvaluate(QString)));
    connect(this, SIGNAL(actPrint(QString, MessageType)), core, SLOT(onPrint(QString, MessageType)));
  }


  void QuantObject::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setPen(QColor(120, 30, 30));
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

    // painter.drawText(5, 15, this->getMap_string("QuantObjectType"));
  }

  QuantObject::~QuantObject() { }

  // QUANT USER ////////////////////////////////////////////////////////////////  

  QuantUser::QuantUser(QWidget *parent, QObject *core) : QuantObject(parent, core)

  {
    qDebug("QuantUser init...");

    const QMetaObject &mo = QuantUser::staticMetaObject;
    metaEnum_targetMethods = mo.enumerator(mo.indexOfEnumerator("TargetMethod"));

    textName = new Text(this);
    textName->setGeometry(5, 5, 50, 20);

    testBox = new ControlBox(this);
    testBox->setGeometry(100, 5, 250, 20);
    testBox->setLabel("test");
    connect(testBox, SIGNAL(actValueChanged(QString)), this, SLOT(onTestChanged(QString)));
  }

  void QuantUser::setName(QString name)
  {
    qDebug("QuantUser::setName");
    data.setValue(DataUser::Key::NAME, name);
    textName->setText(name);
  }
  QString QuantUser::getName()  { return data.getValue_string(DataUser::Key::NAME); }

  void QuantUser::sendData(TargetMethod targetMethod)
  {
    QString target = tr("onNet_%1").arg(metaEnum_targetMethods.valueToKey(targetMethod));
    data.setTargetObject(this->getName());
    data.setTargetMethod(target);

    emit actDataChanged(data);
  }

  void QuantUser::onTestChanged(QString txt)
  {
    qDebug("QuantUser::onTestChanged");
    data.setValue(DataUser::Key::VERSION, txt);
    this->sendData(TargetMethod::UserTest);
  }

  void QuantUser::onNet_UserJoin(DataUser data)
  {
    emit actPrint("User \"" + data.getValue_string(DataUser::NAME) + "\" has been connected to session", MessageType::STATUS);
  }
  void QuantUser::onNet_UserExist(DataUser data)
  {
    testBox->setValue(data.getValue_string(DataUser::Key::VERSION));
    emit actPrint("User \"" + data.getValue_string(DataUser::NAME) + "\" is already connected", MessageType::STATUS);
  }
  void QuantUser::onNet_UserLeave(DataUser data)
  {
    emit actPrint("User \"" + data.getValue_string(DataUser::NAME) + "\" leave session", MessageType::STATUS);
    //disconnect(this, SIGNAL(actDataChanged(DataNEW)), mCore, SLOT(onObjectDataChanged(DataNEW)));
  }
  void QuantUser::onNet_UserTest(DataUser data)
  {
    testBox->setValue(data.getValue_string(DataUser::Key::VERSION));
    //emit actPrint("User \"" + data.getValue_string(DataUser::NAME) + "\" leave session", MessageType::STATUS);
  }


  void QuantUser::paintEvent(QPaintEvent *event)
  {
    QuantObject::paintEvent(event);

    QPainter painter(this);

    painter.setPen(QColor(120, 30, 30));
    //painter.drawText(5, 15, data.getValue_string(DataUser::Key::NAME));
  }

  QuantUser::~QuantUser() { }

  // QUANT PROXYSPACE ////////////////////////////////////////////////////////////////  

  QuantProxy::QuantProxy(QWidget *parent, QObject *core) : QuantObject(parent, core)
  {
    data.setValue(DataProxy::TEMPO, 127);

    qDebug("QuantProxy init...");

    testButton = new Button(this);
    testButton->setGeometry(5, 50, this->width() - 10, 20);
    testButton->setText("beep");
    testButton->setStateKeeping(Button::StateKeeping::TOUCH);
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
    emit actEvaluate("().play");
  }

  void QuantProxy::onControlTEST(QString txt)
  {
    qDebug("QuantProxy::onControlTEST");
    data.setValue(DataProxy::TEMPO, txt);
    //    emit actDataSend(data);
  }

  void QuantProxy::onNet_Recived(DataProxy data)
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


