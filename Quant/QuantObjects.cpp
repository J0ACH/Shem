#include "QuantObjects.h"

namespace QuantIDE
{
  QuantObject::QuantObject(QWidget *parent, QObject *core) :
    QWidget(parent),
    mCanvan(parent),
    mCore(core)
  {
    qDebug("QuantObject init...");
    connect(this, SIGNAL(actDataChanged(Data)), core, SLOT(onObjectDataChanged(Data)));
    connect(this, SIGNAL(actEvaluate(QString, bool)), core, SLOT(onEvaluate(QString, bool)));
    connect(this, SIGNAL(actPrint(QString, MessageType)), core, SLOT(onPrint(QString, MessageType)));
  }

  int QuantObject::getType() { return objectType; }

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

    objectType = QuantObject::ObjectType::USER;

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
    //disconnect(this, SIGNAL(actDataChanged(Data)), mCore, SLOT(onObjectDataChanged(Data)));
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
    objectType = QuantObject::ObjectType::PROXY;

    const QMetaObject &mo = QuantProxy::staticMetaObject;
    metaEnum_targetMethods = mo.enumerator(mo.indexOfEnumerator("TargetMethod"));

    proxyData.setValue(DataProxy::BPM, 60);

    qDebug("QuantProxy init...");

    testButton = new Button(this);
    //testButton->setGeometry(5, 10, this->width() - 10, 20);
    testButton->setText("beep");
    testButton->setStateKeeping(Button::StateKeeping::TOUCH);
    connect(testButton, SIGNAL(pressAct()), this, SLOT(onBeep()));

    tempoBox = new ControlBox(this);
    //tempoBox->setGeometry(5, 80, 200, 50);
    tempoBox->setLabel("BPM");
    tempoBox->setLabelSize(50);
    tempoBox->setValue(proxyData.getValue_string(DataProxy::BPM));
    //connect(tempoBox, SIGNAL(actValueChanged(QString)), this, SLOT(onTempoChanged(QString)));
    connect(tempoBox, SIGNAL(actValueEvaluate(QString)), this, SLOT(onTempoChanged(QString)));

    this->initProxy();
  }

  void QuantProxy::initProxy()
  {
    QString code;
    code =
      "p = ProxySpace.push(s).makeTempoClock;"
      "p.clock.tempo_(60 / 60);";

    emit actEvaluate(code);
  }

  void QuantProxy::setBPM(int bpm)
  {
    proxyData.setValue(DataProxy::BPM, bpm);
    tempoBox->setValue(QString::number(bpm));
    tempoBox->update();
    emit actEvaluate(tr("p.clock.tempo_(%1 / 60);").arg(QString::number(bpm)), true);
  }
  int QuantProxy::getBPM()
  {
    return proxyData.getValue_int(DataProxy::BPM);
  }
  double QuantProxy::getTempo()
  {
    return proxyData.getValue_int(DataProxy::BPM) / 60.0;
  }

  void QuantProxy::sendData(TargetMethod targetMethod)
  {
    QString target = tr("onNet_%1").arg(metaEnum_targetMethods.valueToKey(targetMethod));
    //proxyData.setTargetObject(this->getName());
    proxyData.setTargetMethod(target);

    // emit actPrint("QuantProxy::sendData to target: " + target, MessageType::WARNING);
    emit actDataChanged(proxyData);
  }

  void QuantProxy::onNet_ProxyExist(DataProxy data)
  {
    qDebug("QuantProxy::onNet_ProxyTempo");
    emit actPrint("User \"" + data.getSender() + "\" task for existing proxyspace", MessageType::STATUS);
    emit actPrint(data.print("QuantProxy::onNet_ProxyExist"), MessageType::NORMAL);
    this->sendData(QuantProxy::TargetMethod::ProxySet);
  }

  void QuantProxy::onNet_ProxySet(DataProxy data)
  {
    qDebug("QuantProxy::onNet_ProxyTempo");
    emit actPrint("Copying proxyspace from \"" + data.getSender() + "\"", MessageType::STATUS);
    emit actPrint(data.print("QuantProxy::onNet_ProxyTempo"), MessageType::NORMAL);
    this->setBPM(data.getValue_int(DataProxy::BPM));
  }

  void QuantProxy::onTempoChanged(QString bpmTxt)
  {
    this->setBPM(bpmTxt.toInt());
    this->sendData(QuantProxy::TargetMethod::ProxySet);
  }

  void QuantProxy::onBeep()
  {
    emit actEvaluate("().play;", true);
  }

  void QuantProxy::resizeEvent(QResizeEvent *event)
  {
    QuantObject::resizeEvent(event);

    testButton->setGeometry(5, 10, this->width() - 10, 20);
    tempoBox->setGeometry(5, 50, this->width() - 10, 30);
  }

  QuantProxy::~QuantProxy() { }

  // QUANT NODEPROXY ////////////////////////////////////////////////////////////////

  QuantNode::QuantNode(QWidget *parent, QObject *core) : QuantObject(parent, core)
  {
    objectType = QuantObject::ObjectType::NODE;

    const QMetaObject &mo = QuantNode::staticMetaObject;
    metaEnum_targetMethods = mo.enumerator(mo.indexOfEnumerator("TargetMethod"));

    qDebug("QuantNode init...");

    codeSource = new CodeEditor(this);
    codeSource->setGeometry(5, 50, 90, 20);
    //sourceBox->setLabel("name");
    // nameBox->setValue(this->getMap_string("name"));

    connect(codeSource, SIGNAL(actValueChanged(QString)), this, SLOT(onSourceChanged(QString)));
    connect(codeSource, SIGNAL(actValueEvaluate(QString)), this, SLOT(onSourceEvaluate(QString)));
    connect(codeSource, SIGNAL(actCursorMoved(int)), this, SLOT(onSourceCursorMoved(int)));
  }

  void QuantNode::setSource(QString code)
  {
    qDebug() << "QuantNode::setSource" << code;
    codeSource->setText(code);
    codeSource->update();

    nodeData.setValue(DataNode::SOURCE, code);
  }

  void QuantNode::sendData(TargetMethod targetMethod)
  {
    QString target = tr("onNet_%1").arg(metaEnum_targetMethods.valueToKey(targetMethod));
    nodeData.setTargetObject("testNode");
    nodeData.setTargetMethod(target);

    // emit actPrint("QuantProxy::sendData to target: " + target, MessageType::WARNING);
    emit actDataChanged(nodeData);
  }

  void QuantNode::onNet_NodeExist(DataNode data)
  {
    qDebug("QuantNode::onNet_NodeExist");
    //  emit actPrint("User \"" + data.getSender() + "\" task for existing node", MessageType::STATUS);
    // emit actPrint(data.print("QuantNode::onNet_NodeExist"), MessageType::NORMAL);
    this->sendData(QuantNode::TargetMethod::NodeSet);
  }

  void QuantNode::onNet_NodeSet(DataNode data)
  {
    qDebug() << "QuantNode::onNet_NodeSet key SOURCE:" << data.getValue_string(DataNode::Key::SOURCE);
    qDebug() << data.print("QuantNode::onNet_NodeSet");
    //emit actPrint("Copying node from \"" + data.getSender() + "\"", MessageType::STATUS);
    //emit actPrint(data.print("QuantNode::onNet_NodeSet"), MessageType::NORMAL);
    this->setSource(data.getValue_string(DataNode::Key::SOURCE));
  }

  void QuantNode::onNet_NodeEvaluate(DataNode data)
  {
    qDebug() << "QuantNode::onNet_NodeSet key SOURCE:" << data.getValue_string(DataNode::Key::SOURCE);
    qDebug() << data.print("QuantNode::onNet_NodeEvaluate");
    //emit actPrint("Copying node from \"" + data.getSender() + "\"", MessageType::STATUS);
    //emit actPrint(data.print("QuantNode::onNet_NodeEvaluate"), MessageType::NORMAL);
    this->setSource(data.getValue_string(DataNode::Key::SOURCE));

    emit actEvaluate(data.getValue_string(DataNode::Key::SOURCE), true);
  }

  void QuantNode::onNet_NodeDisplay(DataNode data)
  {
    nodeData.setValue(DataNode::Key::SOURCE_CURSOR, data.getValue_int(DataNode::Key::SOURCE_CURSOR));

    qDebug() << "QuantNode::onNet_NodeDisplay SOURCE_CURSOR:" << data.getValue_int(DataNode::Key::SOURCE_CURSOR);
    //codeSource->setTe
  }

  void QuantNode::onSourceChanged(QString sourceTxt)
  {
    nodeData.setValue(DataNode::SOURCE, sourceTxt);
    this->sendData(QuantNode::TargetMethod::NodeSet);
  }
  void QuantNode::onSourceEvaluate(QString sourceTxt)
  {
    nodeData.setValue(DataNode::SOURCE, sourceTxt);
    emit actEvaluate(sourceTxt);
    this->sendData(QuantNode::TargetMethod::NodeEvaluate);
  }
  void QuantNode::onSourceCursorMoved(int position)
  {
    qDebug("QuantNode::onSourceCursorMoved");
    nodeData.setValue(DataNode::SOURCE_CURSOR, position);
    this->sendData(QuantNode::TargetMethod::NodeDisplay);
  }
 
  void QuantNode::resizeEvent(QResizeEvent *event)
  {
    QuantObject::resizeEvent(event);
    codeSource->setGeometry(5, 5, this->width() - 10, 90);
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


