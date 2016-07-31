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
    textName->setGeometry(5, 5, 150, 20);
    
    textServerMeter = new Text(this);
    textServerMeter->setText("NaN");
    textServerMeter->setToolTip("CPU");
    textServerMeter->setGeometry(this->width() - 170, 5, 40, 20);
    textServerMeter->setAlign(Qt::AlignCenter);
    textServerMeter->show();

    textServerSynths = new Text(this);
    textServerSynths->setText("0");
    textServerSynths->setToolTip("numSynths");
    textServerSynths->setGeometry(this->width() - 120, 5, 20, 20);
    textServerSynths->setAlign(Qt::AlignCenter);
    textServerSynths->show();

    textServerGroups = new Text(this);
    textServerGroups->setText("0");
    textServerGroups->setToolTip("numGroups");
    textServerGroups->setGeometry(this->width() - 90, 5, 20, 20);
    textServerGroups->setAlign(Qt::AlignCenter);
    textServerGroups->show();

  }

  void QuantUser::setName(QString name)
  {
    userData.setValue(DataUser::Key::NAME, name);
    textName->setText(name);
  }
  void QuantUser::setServerMeter(QString value)
  {
    userData.setValue(DataUser::Key::SERVER_METER, value);
    textServerMeter->setText(value);
    textServerMeter->update();
  }
  void QuantUser::setServerSynth(QString value)
  {
    userData.setValue(DataUser::Key::SERVER_CNTSYNTHS, value);
    textServerSynths->setText(value);
    textServerSynths->update();
  }
  void QuantUser::setServerGroup(QString value)
  {
    userData.setValue(DataUser::Key::SERVER_CNTGROUPS, value);
    textServerGroups->setText(value);
    textServerGroups->update();
  }

  QString QuantUser::getName()  { return userData.getValue_string(DataUser::Key::NAME); }
  QString QuantUser::getServerMeter() { return userData.getValue_string(DataUser::Key::SERVER_METER); }
  QString QuantUser::getServerSynth() { return userData.getValue_string(DataUser::Key::SERVER_CNTSYNTHS); }
  QString QuantUser::getServerGroup() { return userData.getValue_string(DataUser::Key::SERVER_CNTGROUPS); }

  void QuantUser::sendData(TargetMethod targetMethod)
  {
    QString target = tr("onNet_%1").arg(metaEnum_targetMethods.valueToKey(targetMethod));
    userData.setTargetObject(this->getName());
    userData.setTargetMethod(target);

    emit actDataChanged(userData);
  }

  void QuantUser::onNet_UserJoin(DataUser data)
  {
    emit actPrint("User \"" + data.getValue_string(DataUser::NAME) + "\" has been connected to session", MessageType::STATUS);
    this->setServerMeter(data.getValue_string(DataUser::Key::SERVER_METER));
    this->setServerSynth(data.getValue_string(DataUser::Key::SERVER_CNTSYNTHS));
    this->setServerGroup(data.getValue_string(DataUser::Key::SERVER_CNTGROUPS));
  }
  void QuantUser::onNet_UserExist(DataUser data)
  {
    emit actPrint("User \"" + data.getValue_string(DataUser::NAME) + "\" is already connected", MessageType::STATUS);
    this->setServerMeter(data.getValue_string(DataUser::Key::SERVER_METER));
    this->setServerSynth(data.getValue_string(DataUser::Key::SERVER_CNTSYNTHS));
    this->setServerGroup(data.getValue_string(DataUser::Key::SERVER_CNTGROUPS));
  }
  void QuantUser::onNet_UserLeave(DataUser data)
  {
    emit actPrint("User \"" + data.getValue_string(DataUser::NAME) + "\" leave session", MessageType::STATUS);
    //disconnect(this, SIGNAL(actDataChanged(DataNEW)), mCore, SLOT(onObjectDataChanged(DataNEW)));
  }
  void QuantUser::onNet_UserServerStatus(DataUser data)
  {
    this->setServerMeter(data.getValue_string(DataUser::Key::SERVER_METER));
    this->setServerSynth(data.getValue_string(DataUser::Key::SERVER_CNTSYNTHS));
    this->setServerGroup(data.getValue_string(DataUser::Key::SERVER_CNTGROUPS));
  }

  void QuantUser::onServerStatus(QStringList data)
  {
    float peakCPU = data[0].toFloat();
    QString peakTXT = tr("%1 %").arg(QString::number(peakCPU, 'f', 2));

    this->setServerMeter(peakTXT);
    this->setServerSynth(data[1]);
    this->setServerGroup(data[2]);

    this->sendData(TargetMethod::UserServerStatus);
  }

  void QuantUser::paintEvent(QPaintEvent *event)
  {
    QuantObject::paintEvent(event);

    QPainter painter(this);

    painter.setPen(QColor(120, 30, 30));
    //painter.drawText(5, 15, data.getValue_string(DataUser::Key::NAME));
  }
  void QuantUser::resizeEvent(QResizeEvent *event)
  {
    QuantObject::resizeEvent(event);

    textServerMeter->setGeometry(this->width() - 120, 5, 40, 20);
    textServerSynths->setGeometry(this->width() - 70, 5, 20, 20);
      textServerGroups->setGeometry(this->width() - 40, 5, 20, 20);
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


