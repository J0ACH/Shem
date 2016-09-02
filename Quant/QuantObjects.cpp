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

    isPlaying = false;

    nodeData.setValue(DataNode::Key::NAME, "node");
    nodeData.setValue(DataNode::Key::VOLUME, "0.5");
    nodeData.setValue(DataNode::Key::FTIME, "0");

    this->initControl();
      }

  void QuantNode::initControl()
  {
    closeButton = new Button(this);
    closeButton->setIcon(QImage(":/smallClose16.png"), 0);
    closeButton->setText("X");

    playButton = new Button(this);
    playButton->setText("play");
    playButton->setStateKeeping(Jui::Button::StateKeeping::HOLD);

    volumeBox = new ControlBox(this);
    volumeBox->setLabel("vol");
    volumeBox->setLabelSize(15);
    volumeBox->setValue(nodeData.getValue_string(DataNode::Key::VOLUME));
    
    fadeTimeBox = new ControlBox(this);
    fadeTimeBox->setLabel("fTime");
    fadeTimeBox->setLabelSize(30);
    fadeTimeBox->setValue(nodeData.getValue_string(DataNode::Key::FTIME));    

    this->addCodeEditor(0); // sourceCode
    textName = new Text(this);

    connect(closeButton, SIGNAL(actPressed()), this, SLOT(onClose()));
    connect(playButton, SIGNAL(actPressed()), this, SLOT(onPlayingChanged()));
    connect(volumeBox, SIGNAL(actValueEvaluate(QString)), this, SLOT(onVolumeChanged(QString)));
    connect(fadeTimeBox, SIGNAL(actValueEvaluate(QString)), this, SLOT(onFadeTimeChanged(QString)));
  }

  void QuantNode::setName(QString name)
  {
    nodeData.setValue(DataNode::Key::NAME, name);
    textName->setText(name);
  }
  QString QuantNode::getName()  { return nodeData.getValue_string(DataNode::Key::NAME); }

  void QuantNode::sendData(TargetMethod targetMethod)
  {
    QString target = tr("onNet_%1").arg(metaEnum_targetMethods.valueToKey(targetMethod));
    nodeData.setTargetObject(this->getName());
    nodeData.setTargetMethod(target);

    emit actDataChanged(nodeData);
  }

  void QuantNode::onNet_NodeCreated(DataNode data)
  {
    //qDebug("QuantNode::onNet_NodeCreated");
    emit actPrint("Node \"" + this->getName() + "\" created by user " + data.getSender() + " ...", MessageType::STATUS);
    nodeData = data;
    textName->setText(this->getName());
  }
  void QuantNode::onNet_NodeKilled(DataNode data)
  {
    //qDebug("QuantNode::onNet_NodeKilled");    
    emit actEvaluate(tr("~%1.free").arg(this->getName()), true);
    emit actPrint("Node \"" + this->getName() + "\" deleted by user " + data.getSender() + " ...", MessageType::STATUS);
  }
  void QuantNode::onNet_NodePlayingChanged(DataNode data)
  {
    QString nodeName = this->getName();
    QString volume = volumeBox->getValue_string();
    QString fTime = fadeTimeBox->getValue_string();

    if (!isPlaying)
    {
      isPlaying = true;
      this->playButton->setState(Button::ON);
      emit actPrint("Node \"" + this->getName() + "\" cmd PLAY by user " + data.getSender() + " ...", MessageType::STATUS);
      emit actEvaluate(tr("~%1.play(vol: %2, fadeTime: %3)").arg(
        nodeName,
        volume,
        fTime
        ), true);
    }
    else
    {
      isPlaying = false;
      this->playButton->setState(Button::OFF);
      emit actPrint("Node \"" + this->getName() + "\" cmd STOP by user " + data.getSender() + " ...", MessageType::STATUS);
      emit actEvaluate(tr("~%1.stop(%2)").arg(
        nodeName,
        fTime
        ), true);
    }
  }

  void QuantNode::onVolumeChanged(QString txtValue)
  {
    // qDebug() << "QuantNode::onVolumeChanged value:" << txtValue;
    nodeData.setValue(DataNode::Key::VOLUME, txtValue);
    this->sendData(QuantNode::TargetMethod::VolumeChanged);

    if (isPlaying)
    {
      emit actEvaluate(tr("~%1.play(vol: %2, fadeTime: %3)").arg(this->getName(), volumeBox->getValue_string(), fadeTimeBox->getValue_string()));
    }
  }
  void QuantNode::onNet_VolumeChanged(DataNode data)
  {
    // qDebug() << "QuantNode::onNet_VolumeChanged";
    QString volume = data.getValue_string(DataNode::Key::VOLUME);
    nodeData.setValue(DataNode::Key::VOLUME, volume);
    volumeBox->setValue(volume);
    if (isPlaying)
    {
      emit actPrint("Node \"" + this->getName() + "\" cmd VOLUME: " + volume + " set by user " + data.getSender() + " ...", MessageType::STATUS);
      emit actEvaluate(tr("~%1.play(vol: %2, fadeTime: %3)").arg(this->getName(), volumeBox->getValue_string(), fadeTimeBox->getValue_string()));
    }
  }

  void QuantNode::onFadeTimeChanged(QString txtValue)
  {
    // qDebug() << "QuantNode::onFadeTimeChanged value:" << txtValue;
    nodeData.setValue(DataNode::Key::FTIME, txtValue);
    this->sendData(QuantNode::TargetMethod::FadeTimeChanged);
    emit actEvaluate(tr("~%1.fadeTime_(%2);").arg(this->getName(), fadeTimeBox->getValue_string()));
  }
  void QuantNode::onNet_FadeTimeChanged(DataNode data)
  {
    // qDebug() << "QuantNode::onNet_FadeTimeChanged";
    QString ftime = data.getValue_string(DataNode::Key::FTIME);
    nodeData.setValue(DataNode::Key::FTIME, ftime);
    fadeTimeBox->setValue(ftime);
    emit actPrint("Node \"" + this->getName() + "\" cmd FadeTIME: " + ftime + " set by user " + data.getSender() + " ...", MessageType::STATUS);
    emit actEvaluate(tr("~%1.fadeTime_(%2);").arg(this->getName(), fadeTimeBox->getValue_string()));
  }

  void QuantNode::onAddCodeEditor(QString insertButtonName)
  {
    int insertingIndex = insertButtonName.toInt();
    // qDebug() << "QuantNode::onAddCodeEditor button name:" << insertingIndex;

    this->addCodeEditor(insertingIndex);
    nodeData.setValue(DataNode::Key::INDEX_CHANGE, insertingIndex);

    this->sendData(QuantNode::TargetMethod::AddCodeEditor);

  }
  void QuantNode::onNet_AddCodeEditor(DataNode data)
  {
    emit actPrint(data.print("QuantNode::onNet_AddCodeEditor"), MessageType::NORMAL);
    int insertingIndex = data.getValue_int(DataNode::Key::INDEX_CHANGE);
    // qDebug() << "QuantNode::onNet_AddCodeEditor button name:" << insertingIndex;
    this->addCodeEditor(insertingIndex);
  }
  void QuantNode::addCodeEditor(int index)
  {
    QStringList tempList;
    for (int i = 0; i < codeEditors.size(); i++)
    {
      tempList.append(nodeData.getValue_string(DataNode::Key::CODES, i));
      //   qDebug() << "QuantNode::addCodeEditor TEMPLIST A:" << i << " -> " << tempList[i];
    }

    CodeEditor *newCodeEditor = new CodeEditor(this);
    newCodeEditor->setText("nil");
    newCodeEditor->setFixedHeight(40);
    newCodeEditor->show();
    codeEditors.insert(index + 1, newCodeEditor);

    Text *newCodeIndex = new Text(this);
    newCodeIndex->show();
    codeIndexs.append(newCodeIndex);

    Button *insertButton = new Button(this);
    insertButton->setText("--- insert code ---");
    insertButton->show();
    insertCodeButtons.append(insertButton);

    Button *removeButton = new Button(this);
    removeButton->setIcon(QImage(":/smallClose16.png"), 0);
    removeButton->show();
    removeCodeButtons.append(removeButton);

    connect(newCodeEditor, SIGNAL(actValueChanged(QString, QString)), this, SLOT(onCodeChanged(QString, QString)));
    connect(newCodeEditor, SIGNAL(actValueEvaluate(QString, QString)), this, SLOT(onCodeEvaluate(QString, QString)));
    connect(newCodeEditor, SIGNAL(actCursorMoved(int)), this, SLOT(onSourceCursorMoved(int)));
    connect(insertButton, SIGNAL(actPressed(QString)), this, SLOT(onAddCodeEditor(QString)));
    connect(removeButton, SIGNAL(actPressed(QString)), this, SLOT(onRemoveCodeEditor(QString)));

    this->fitEditorsPosition();

    tempList.insert(index + 1, "nil");

    for (int i = 0; i < tempList.size(); i++)
    {
      nodeData.setValue(DataNode::Key::CODES, i, tempList[i]);
      //  qDebug() << "QuantNode::addCodeEditor TEMPLIST B:" << i << " -> " << tempList[i];
    }
    nodeData.setValue(DataNode::Key::INDEX_SIZE, codeEditors.size());
  }

  void QuantNode::onRemoveCodeEditor(QString removeButtonName)
  {
    int removeingIndex = removeButtonName.toInt();
    //qDebug() << "QuantNode::onRemoveCodeEditor button name:" << removeingIndex;

    this->removeCodeEditor(removeingIndex);


    nodeData.setValue(DataNode::Key::INDEX_CHANGE, removeingIndex);
    this->sendData(QuantNode::TargetMethod::RemoveCodeEditor);

    emit actEvaluate(tr("~%1[%2] = nil").arg(this->getName(), QString::number(removeingIndex)));
  }
  void QuantNode::onNet_RemoveCodeEditor(DataNode data)
  {
    emit actPrint(data.print("QuantNode::onNet_RemoveCodeEditor"), MessageType::NORMAL);
    int removeingIndex = data.getValue_int(DataNode::Key::INDEX_CHANGE);
    //qDebug() << "QuantNode::onNet_RemoveCodeEditor button name:" << removeingIndex;

    this->removeCodeEditor(removeingIndex);

    emit actEvaluate(tr("~%1[%2] = nil").arg(this->getName(), QString::number(removeingIndex)));
  }
  void QuantNode::removeCodeEditor(int index)
  {
    QStringList tempList;
    for (int i = 0; i < codeEditors.size(); i++)
    {
      tempList.append(nodeData.getValue_string(DataNode::Key::CODES, i));
      // qDebug() << "QuantNode::removeCodeEditor TEMPLIST A:" << i << " -> " << tempList[i];
    }

    CodeEditor *codeEditor = codeEditors.takeAt(index);
    codeEditor->close();

    Button *insertButton = insertCodeButtons.takeAt(index);
    insertButton->close();

    Button *removeButton = removeCodeButtons.takeAt(index);
    removeButton->close();

    Text *codeIndex = codeIndexs.takeAt(index);
    codeIndex->close();

    nodeData.setValue(DataNode::Key::INDEX_SIZE, codeEditors.size());

    tempList.removeAt(index);

    for (int i = 0; i < tempList.size(); i++)
    {
      nodeData.setValue(DataNode::Key::CODES, i, tempList[i]);
      // qDebug() << "QuantNode::removeCodeEditor TEMPLIST B:" << i << " -> " << tempList[i];
    }

    nodeData.deleteValue(DataNode::Key::CODES, codeEditors.size());

    this->fitEditorsPosition();
  }

  void QuantNode::onCodeChanged(QString indexCodeName, QString code)
  {
    int codeIndex = indexCodeName.toInt();
    qDebug() << "QuantNode::onCodeChanged [name:" << codeIndex << "|| code:" << code << "]";

    nodeData.setValue(DataNode::Key::INDEX_CHANGE, codeIndex);
    nodeData.setValue(DataNode::Key::CODES, codeIndex, code);
    // emit actPrint(nodeData.print("QuantNode::onCodeChanged"), MessageType::EVAULATE);
    this->sendData(QuantNode::TargetMethod::CodeChanged);
  }
  void QuantNode::onNet_CodeChanged(DataNode data)
  {
    //qDebug() << "QuantNode::onNet_CodesChanged";
    int dataIndexCnt = data.getValue_int(DataNode::Key::INDEX_SIZE);
    int dataIndexChanged = data.getValue_int(DataNode::Key::INDEX_CHANGE);
    //qDebug() << "QuantNode::onNet_NodeCodes indexSize:" << dataIndexCnt;
    qDebug() << "QuantNode::onNet_CodesChanged indexChanged:" << dataIndexChanged;

    QString oneCode = data.getValue_string(DataNode::Key::CODES, dataIndexChanged);
    nodeData.setValue(DataNode::Key::CODES, dataIndexChanged, oneCode);
    codeEditors[dataIndexChanged]->setText(oneCode);

    emit actPrint(nodeData.print("QuantNode::onNet_CodesChanged"), MessageType::EVAULATE);

    // nodeData.setValue(DataNode::Key::SOURCE_CURSOR, data.getValue_int(DataNode::Key::SOURCE_CURSOR));
    // codeSource->onChangeExtraCursor(data.getSender(), data.getValue_int(DataNode::Key::SOURCE_CURSOR));
    this->update();
  }

  void QuantNode::onCodeEvaluate(QString indexCodeName, QString code)
  {
    int codeIndex = indexCodeName.toInt();
    qDebug() << "QuantNode::onCodeEvaluate [name:" << codeIndex << "|| code:" << code << "]";

    nodeData.setValue(DataNode::Key::INDEX_CHANGE, codeIndex);
    this->sendData(QuantNode::TargetMethod::CodeEvaluate);

    if (codeIndex == 0) { emit actEvaluate(tr("(~%1[%2] = { %3 })").arg(this->getName(), QString::number(codeIndex), code)); }
    else { emit actEvaluate(tr("(~%1[%2] = %3)").arg(this->getName(), QString::number(codeIndex), code)); }
  }
  void QuantNode::onNet_CodeEvaluate(DataNode data)
  {
    int dataIndexEvaluated = data.getValue_int(DataNode::Key::INDEX_CHANGE);
    qDebug() << "QuantNode::onNet_CodeEvaluate indexChanged:" << dataIndexEvaluated;

    QString code = data.getValue_string(DataNode::Key::CODES, dataIndexEvaluated);
    nodeData.setValue(DataNode::Key::CODES, dataIndexEvaluated, code);
    codeEditors[dataIndexEvaluated]->setText(code);
    codeEditors[dataIndexEvaluated]->onEvaluate(QColor(120, 30, 30));

    if (dataIndexEvaluated == 0) { emit actEvaluate(tr("(~%1[%2] = { %3 })").arg(this->getName(), QString::number(dataIndexEvaluated), code)); }
    else { emit actEvaluate(tr("(~%1[%2] =  %3 )").arg(this->getName(), QString::number(dataIndexEvaluated), code)); }

    this->update();
  }

  void QuantNode::onNet_NodeDisplay(DataNode data)
  {
    nodeData.setValue(DataNode::Key::SOURCE_CURSOR, data.getValue_int(DataNode::Key::SOURCE_CURSOR));

    //qDebug() << "QuantNode::onNet_NodeDisplay SOURCE_CURSOR:" << data.getValue_int(DataNode::Key::SOURCE_CURSOR);

    this->update();
  }

  void QuantNode::onClose()
  {
    emit actEvaluate(tr("~%1.free").arg(this->getName()), true);
    emit actKilled(this->getName(), true);
  }
  void QuantNode::onPlayingChanged()
  {
    QString nodeName = this->getName();
    double volume = 0.5;
    double fTime = 0;

    if (!isPlaying)
    {
      isPlaying = true;
      this->sendData(QuantNode::TargetMethod::NodePlayingChanged);

      emit actEvaluate(tr("~%1.play(vol: %2, fadeTime: %3)").arg(
        nodeName,
        QString::number(volume),
        QString::number(fTime)
        ), true);
    }
    else
    {
      isPlaying = false;
      this->sendData(QuantNode::TargetMethod::NodePlayingChanged);
      emit actEvaluate(tr("~%1.stop(%2)").arg(
        nodeName,
        QString::number(fTime)
        ), true);
    }
  }

  void QuantNode::onSourceCursorMoved(int position)
  {
    //qDebug("QuantNode::onSourceCursorMoved");
    nodeData.setValue(DataNode::SOURCE_CURSOR, position);
    this->sendData(QuantNode::TargetMethod::NodeDisplay);
  }

  void QuantNode::resizeEvent(QResizeEvent *event)
  {
    QuantObject::resizeEvent(event);
    closeButton->setGeometry(this->width() - 30, 10, 16, 16);
    textName->setGeometry(5, 5, 70, 20);
    playButton->setGeometry(80, 5, 40, 20);
    volumeBox->setGeometry(130, 5, 50, 20);
    fadeTimeBox->setGeometry(190, 5, 50, 20);

    this->fitEditorsPosition();
  }

  void QuantNode::fitEditorsPosition()
  {
    int gapSize = 10;
    int lastObjOriginY = gapSize + 30;

    for (int i = 0; i < codeEditors.size(); i++)
    {
      codeEditors[i]->setGeometry(
        30,
        lastObjOriginY,
        width() - 60,
        codeEditors[i]->height()
        );
      codeEditors[i]->setObjectName(QString::number(i));

      insertCodeButtons[i]->setGeometry(
        30,
        lastObjOriginY + codeEditors[i]->height() + gapSize / 2,
        width() - 60,
        20
        );
      insertCodeButtons[i]->setObjectName(QString::number(i));

      removeCodeButtons[i]->setGeometry(width() - 25, lastObjOriginY, 20, 20);
      removeCodeButtons[i]->setObjectName(QString::number(i));
      if (i == 0) { removeCodeButtons[i]->hide(); }

      codeIndexs[i]->setGeometry(5, lastObjOriginY, 30, 20);
      codeIndexs[i]->setObjectName(QString::number(i));
      codeIndexs[i]->setText("[" + QString::number(i) + "]");

      lastObjOriginY += codeEditors[i]->height() + insertCodeButtons[i]->height() + gapSize;
    }

    this->setFixedHeight(lastObjOriginY + 30);

    this->update();
    emit actSizeChanged();
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


