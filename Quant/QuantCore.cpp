#include "QuantCore.h"

namespace QuantIDE
{
  QuantCore::QuantCore(CanvanNEW *canvan) :
    QObject(canvan),
    mCanvan(canvan),
    mBridge(new ScBridge(this, "_normal")),
    mNetwork(new UDPServer(this))
  {
    qDebug("Core init...");

    isCoreRunning = false;
    isNetworkRunning = false;

    connect(this, SIGNAL(actCoreInitPrepared()), this, SLOT(onCoreInit()));
    connect(mCanvan, SIGNAL(actClose()), this, SLOT(onCoreKill()));

    this->initControls();
  }
  QuantCore::QuantCore(CanvanNEW *canvan, QString appName, int sendPort, int listenPort) :
    QObject(canvan),
    mCanvan(canvan),
    mBridge(new ScBridge(this, "_" + appName)),
    mNetwork(new UDPServer(this, sendPort, listenPort))
  {
    userName = appName;

    isCoreRunning = false;
    isNetworkRunning = false;

    connect(this, SIGNAL(actCoreInitPrepared()), this, SLOT(onCoreInit()));
    connect(mCanvan, SIGNAL(actClose()), this, SLOT(onCoreKill()));

    colorMsgNormal = QColor(120, 120, 120);
    this->initControls();
  }

  void QuantCore::initControls()
  {
    networkPanel = new QuantPanel(mCanvan);
    mCanvan->addPanel(networkPanel, "NetworkPanel", Qt::DockWidgetArea::LeftDockWidgetArea);

    timePanel = new QuantPanel(mCanvan);
    timePanel->setVisible(false);
    mCanvan->addPanel(timePanel, "TimePanel", Qt::DockWidgetArea::LeftDockWidgetArea);

    //nodePanel = new QuantPanel(mCanvan);
    nodePanel = new NodePanelNEW(mCanvan);
    mCanvan->addPanel(nodePanel, "NodePanel", Qt::DockWidgetArea::TopDockWidgetArea);
    nodePanel->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    nodePanel->setMinimumHeight(300);

    lib_users = new Library(networkPanel, this);
    lib_proxy = new Library(timePanel, this);
    lib_nodes = new Library(nodePanel, this);

    networkPanel->setScrollWidget(lib_users);
    timePanel->setScrollWidget(lib_proxy);
    nodePanel->setScrollWidget(lib_nodes);

    textServerMeter = new Text(mCanvan->getStaustBar());
    textServerMeter->setText("NaN");
    textServerMeter->setToolTip("CPU");
    textServerMeter->setGeometry(mCanvan->getStaustBar()->width() - 270, 0, 40, mCanvan->getStaustBar()->height());
    textServerMeter->setAlign(Qt::AlignCenter);
    textServerMeter->show();

    textServerSynths = new Text(mCanvan->getStaustBar());
    textServerSynths->setText("0");
    textServerSynths->setToolTip("numSynths");
    textServerSynths->setGeometry(mCanvan->getStaustBar()->width() - 220, 0, 15, mCanvan->getStaustBar()->height());
    textServerSynths->setAlign(Qt::AlignCenter);
    textServerSynths->show();

    textServerGroups = new Text(mCanvan->getStaustBar());
    textServerGroups->setText("0");
    textServerGroups->setToolTip("numGroups");
    textServerGroups->setGeometry(mCanvan->getStaustBar()->width() - 200, 0, 15, mCanvan->getStaustBar()->height());
    textServerGroups->setAlign(Qt::AlignCenter);
    textServerGroups->show();
  }

  void QuantCore::onCustomize(DataCustomize data)
  {
#if (!_DEBUG)
    userName = data.getValue_string(DataCustomize::Key::USERNAME);
#endif
    qDebug() << "QuantCore::onCustomize userName:" << userName;

    initNetworkOnStart = data.getValue_bool(DataCustomize::Key::BOOL_BOOT_NETWORK);
    initInterpretOnStart = data.getValue_bool(DataCustomize::Key::BOOL_BOOT_INTERPRETR);
    initServerOnStart = data.getValue_bool(DataCustomize::Key::BOOL_BOOT_SERVER);

    colorMsgNormal = data.getValue_color(DataCustomize::Key::COLOR_MSG_NORMAL);
    colorMsgStatus = data.getValue_color(DataCustomize::Key::COLOR_MSG_STATUS);
    colorMsgEvaluate = data.getValue_color(DataCustomize::Key::COLOR_MSG_EVALUATE);
    colorMsgAnswer = data.getValue_color(DataCustomize::Key::COLOR_MSG_ANSWER);
    colorMsgError = data.getValue_color(DataCustomize::Key::COLOR_MSG_ERROR);
    colorMsgWarning = data.getValue_color(DataCustomize::Key::COLOR_MSG_WARNINIG);
    colorMsgBundle = data.getValue_color(DataCustomize::Key::COLOR_MSG_BUNDLE);


    emit actCoreInitPrepared(); // ceka na initInterpretOnStart a initServerOnStart
  }
  void QuantCore::onChangeUserName(DataCustomize data)
  {
    if (mNetwork->isConnected()) { this->onNetChangeState(); }
    this->onCustomize(data);
  }

  // CORE /////////////////////////////////////////

  void QuantCore::onCoreInit()
  {
    // qDebug("QuantCore::onInitCore");
    if (!isCoreRunning)
    {
      isCoreRunning = true;

      if (initNetworkOnStart) { this->onNetChangeState(); }
      if (initInterpretOnStart) { this->onInterpretChangeState(); }
    }
  }
  void QuantCore::onCoreKill()
  {
    //qDebug("QuantCore::onCoreKill");
    if (mNetwork->isConnected()) { this->onNetChangeState(); }
    mBridge->killBridge();
    mCanvan->onCanvanClose();
  }

  // NETWORK /////////////////////////////////////////

  void QuantCore::onNetChangeState()
  {
    qDebug("QuantCore::onNetChangeState");
    if (!mNetwork->isConnected())
    {
      connect(mNetwork, SIGNAL(actInitDone()), this, SLOT(onNetInitDone()));
      connect(mNetwork, SIGNAL(actKillDone()), this, SLOT(onNetKillDone()));
      connect(mNetwork, SIGNAL(actNetDataRecived(QByteArray)), this, SLOT(onNetDataRecived(QByteArray)));
      connect(this, SIGNAL(actObjectDataSend(QByteArray)), mNetwork, SLOT(onSendData(QByteArray)));
      connect(mNetwork, SIGNAL(actPrint(QString, MessageType)), this, SLOT(onPrint(QString, MessageType)));

      mNetwork->initNetwork();
    }
    else
    {
      lib_users->getUser(userName)->sendData(QuantUser::TargetMethod::UserLeave);
      mNetwork->killNetwork();
    }
  }
  void QuantCore::onNetInitDone()
  {
    qDebug("QuantCore::onNetworkBootDone");
    this->onAddUser(userName, true, false);

    if (mBridge->isServerRunning())
    {
      connect(mBridge, SIGNAL(actServerStatus(QStringList)), lib_users->getUser(userName), SLOT(onServerStatus(QStringList)));

      lib_proxy->getProxy()->setBPM(lib_proxy->getProxy()->getBPM()); // PROBLEM - JE TU JEN PRO ZPOMALENI PRED DOTAZEM NA EXISTENCI PROXY, PROC????
      lib_proxy->getProxy()->sendData(QuantProxy::TargetMethod::ProxyExist);
    }

    mCanvan->getButtonBar("Bridge")->getButton("Network")->setState(Button::State::ON);
  }
  void QuantCore::onNetKillDone()
  {
    //qDebug("QuantCore::onNetKillDone");

    disconnect(mNetwork, SIGNAL(actInitDone()), this, SLOT(onNetInitDone()));
    disconnect(mNetwork, SIGNAL(actKillDone()), this, SLOT(onNetKillDone()));
    disconnect(mNetwork, SIGNAL(actNetDataRecived(QByteArray)), this, SLOT(onNetDataRecived(QByteArray)));
    disconnect(this, SIGNAL(actObjectDataSend(QByteArray)), mNetwork, SLOT(onSendData(QByteArray)));
    disconnect(mNetwork, SIGNAL(actPrint(QString, MessageType)), this, SLOT(onPrint(QString, MessageType)));

    mCanvan->getButtonBar("Bridge")->getButton("Network")->setState(Button::State::OFF);

    this->onKillUser(userName, true);
  }

  // INTERPRET /////////////////////////////////////////

  void QuantCore::onInterpretChangeState()
  {
    //qDebug("QuantCore::onInterpretChangeState");

    if (!mBridge->isInterpretRunning())
    {
      connect(mBridge, SIGNAL(actInterpretInit()), this, SLOT(onInterpretInit()));
      connect(mBridge, SIGNAL(actInterpretInitDone()), this, SLOT(onInterpretInitDone()));
      connect(mBridge, SIGNAL(actInterpretKill()), this, SLOT(onInterpretKill()));
      connect(mBridge, SIGNAL(actInterpretKillDone()), this, SLOT(onInterpretKillDone()));

      connect(mBridge, SIGNAL(actPrint(QString, MessageType)), this, SLOT(onPrint(QString, MessageType)));
    }

    mBridge->changeInterpretState();
  }
  void QuantCore::onInterpretInit()
  {
    //qDebug("QuantCore::onInterpretInit");
    this->onPrint("Interpretr init...", MessageType::STATUS);
  }
  void QuantCore::onInterpretInitDone()
  {
    // qDebug("QuantCore::onInterpretBootDone");
    mCanvan->getButtonBar("Bridge")->getButton("Interpretr")->setState(Button::State::ON);
    mCanvan->getButtonBar("Bridge")->getButton("Server")->setState(Button::State::OFF);
    this->onPrint("Interpretr init done...\n", MessageType::STATUS);
    if (initServerOnStart) { this->onServerChangeState(); }
  }
  void QuantCore::onInterpretKill()
  {
    //  qDebug("QuantCore::onInterpretKill");
    this->onPrint("Interpretr kill...", MessageType::STATUS);

  }
  void QuantCore::onInterpretKillDone()
  {
    //  qDebug("QuantCore::onInterpretKillDone");
    mCanvan->getButtonBar("Bridge")->getButton("Interpretr")->setState(Button::State::OFF);
    mCanvan->getButtonBar("Bridge")->getButton("Server")->setState(Button::State::FROZEN);

    disconnect(mBridge, SIGNAL(actInterpretInit()), this, SLOT(onInterpretInit()));
    disconnect(mBridge, SIGNAL(actInterpretInitDone()), this, SLOT(onInterpretInitDone()));
    disconnect(mBridge, SIGNAL(actInterpretKill()), this, SLOT(onInterpretKill()));
    disconnect(mBridge, SIGNAL(actInterpretKillDone()), this, SLOT(onInterpretKillDone()));

    disconnect(mBridge, SIGNAL(actPrint(QString, MessageType)), this, SLOT(onPrint(QString, MessageType)));

    this->onPrint("Interpretr kill done...\n", MessageType::STATUS);
  }

  // SERVER /////////////////////////////////////////

  void QuantCore::onServerChangeState()
  {
    if (mBridge->isInterpretRunning())
    {
      //  qDebug("QuantCore::onServerChangeState");

      if (!mBridge->isServerRunning())
      {
        connect(mBridge, SIGNAL(actServerInit()), this, SLOT(onServerInit()));
        connect(mBridge, SIGNAL(actServerInitDone()), this, SLOT(onServerInitDone()));
        connect(mBridge, SIGNAL(actServerKill()), this, SLOT(onServerKill()));
        connect(mBridge, SIGNAL(actServerKillDone()), this, SLOT(onServerKillDone()));
        connect(mBridge, SIGNAL(actServerStatus(QStringList)), this, SLOT(onServerStatus(QStringList)));
        if (mNetwork->isConnected())
        {
          connect(mBridge, SIGNAL(actServerStatus(QStringList)), lib_users->getUser(userName), SLOT(onServerStatus(QStringList)));
        }
      }
      mBridge->changeServerState();
    }
  }
  void QuantCore::onServerInit()
  {
    //qDebug("QuantCore::onServerInit");
    this->onPrint("Server init...", MessageType::STATUS);
  }
  void QuantCore::onServerInitDone()
  {
    qDebug("QuantCore::onServerInitDone");
    mCanvan->getButtonBar("Bridge")->getButton("Server")->setState(Button::State::ON);

    mBridge->initOSC();

    QuantProxy *proxy = new QuantProxy(0, this);
    proxy->setGeometry(10, 30, 30, 150);
    lib_proxy->addObject(proxy);
    lib_proxy->getProxy()->sendData(QuantProxy::TargetMethod::ProxyExist);

    connect(nodePanel, SIGNAL(actAddNode(QString, bool)), this, SLOT(onAddNode(QString, bool)));

    this->onPrint("Server init done...\n", MessageType::STATUS);
  }
  void QuantCore::onServerKill()
  {
    qDebug("QuantCore::onServerKill");
    this->onPrint("Server kill...", MessageType::STATUS);

    lib_proxy->removeObject("proxy");

    disconnect(nodePanel, SIGNAL(actAddNode()), this, SLOT(onAddNode()));

    //proxy->deleteLater();
    //proxy = NULL;
  }
  void QuantCore::onServerKillDone()
  {
    // qDebug("QuantCore::onServerKillDone");
    mCanvan->getButtonBar("Bridge")->getButton("Server")->setState(Button::State::OFF);

    disconnect(mBridge, SIGNAL(actServerInit()), this, SLOT(onServerInit()));
    disconnect(mBridge, SIGNAL(actServerInitDone()), this, SLOT(onServerInitDone()));
    disconnect(mBridge, SIGNAL(actServerKill()), this, SLOT(onServerKill()));
    disconnect(mBridge, SIGNAL(actServerKillDone()), this, SLOT(onServerKillDone()));
    disconnect(mBridge, SIGNAL(actServerStatus(QStringList)), this, SLOT(onServerStatus(QStringList)));
    disconnect(mBridge, SIGNAL(actServerStatus(QStringList)), lib_users->getUser(userName), SLOT(onServerStatus(QStringList)));

    textServerMeter->setText("NaN");
    textServerSynths->setText("0");
    textServerGroups->setText("0");

    lib_users->getUser(userName)->setServerMeter("NaN");
    lib_users->getUser(userName)->setServerSynth("0");
    lib_users->getUser(userName)->setServerGroup("0");

    lib_users->getUser(userName)->sendData(QuantUser::TargetMethod::UserServerStatus);

    this->onPrint("Server kill done...\n", MessageType::STATUS);
  }
  void QuantCore::onServerStatus(QStringList data)
  {
    //qDebug() << "QuantCore::onServerStatus: " << data;
    float peakCPU = data[0].toFloat();

    textServerMeter->setText(tr("%1 %").arg(QString::number(peakCPU, 'f', 2)));
    textServerSynths->setText(data[1]);
    textServerGroups->setText(data[2]);

    textServerMeter->update();
    textServerSynths->update();
    textServerGroups->update();
  }

  // OBJECTS /////////////////////////////////////////

  void QuantCore::onObjectDataChanged(Data data)
  {
    data.setSender(userName);
    //data.setTime(proxy time); // pridat cas odeslani

    // QString sender = data.getSender();
    QString targetObject = data.getTarget();
    QString targetMethod = data.getMethod();

    if (targetMethod != "onNet_UserServerStatus")
    {
      // qDebug() << "QuantCore::onObjectDataChanged sender: " << sender;
      qDebug() << "QuantCore::onObjectDataChanged targetObject: " << targetObject;
      qDebug() << "QuantCore::onObjectDataChanged targetMethod: " << targetMethod;

      // this->onPrint("Core [" + userName + "] send targetObject: " + targetObject + " targetMethod: " + targetMethod, MessageType::BUNDLE);
    }

    emit actObjectDataSend(data.wrap());
  }
  void QuantCore::onNetDataRecived(QByteArray msg)
  {

    if (Data::isFromOtherOwener(msg, userName))
    {
      QString sender = Data::getSender(msg);
      QString targetObject = Data::getTarget(msg);
      QString targetMethod = Data::getMethod(msg);
      std::string targetMethod_str = targetMethod.toLatin1().constData();

      if (targetMethod != "onNet_UserServerStatus")
      {
        qDebug() << "QuantCore::onNetworkDataRecived sender: " << sender;
        qDebug() << "QuantCore::onNetworkDataRecived targetObject: " << targetObject;
        qDebug() << "QuantCore::onNetworkDataRecived targetMethod: " << targetMethod;

        //  this->onPrint("Core [" + sender + "->" + userName + "] recived targetObject: " + targetObject + " targetMethod: " + targetMethod, MessageType::WARNING);
      }

      switch (Data::getType(msg))
      {
      case Data::DataType::USER:
        if (targetMethod == "onNet_UserJoin") { this->onAddUser(sender, false, true); }
        if (targetMethod == "onNet_UserExist") { this->onAddUser(sender, false, false); }
        QMetaObject::invokeMethod(lib_users->getUser(targetObject), targetMethod_str.c_str(), Q_ARG(DataUser, DataUser(msg)));
        if (targetMethod == "onNet_UserLeave") { this->onKillUser(sender, false); }
        break;

      case Data::DataType::PROXY:
        QMetaObject::invokeMethod(lib_proxy->getProxy(), targetMethod_str.c_str(), Q_ARG(DataProxy, DataProxy(msg)));
        break;

      case Data::DataType::NODE:
        if (targetMethod == "onNet_NodeCreated") { this->onAddNode(targetObject, false); }
        QMetaObject::invokeMethod(lib_nodes->getNode(targetObject), targetMethod_str.c_str(), Q_ARG(DataNode, DataNode(msg)));
        if (targetMethod == "onNet_NodeKilled") { this->onKillNode(targetObject, false); }
        break;
      default:
        break;
      }
    }
  }

  // USERS /////////////////////////////////////////

  void QuantCore::onAddUser(QString name, bool sendMsgJoin, bool sendMsgExist)
  {
    if (!lib_users->containObject(name))
    {
      QuantUser *newUser = new QuantUser(0, this);
      newUser->setName(name);
      newUser->setFixedHeight(40);
      lib_users->addObject(newUser);

      if (sendMsgJoin) { newUser->sendData(QuantUser::TargetMethod::UserJoin); }
      if (sendMsgExist) { newUser->sendData(QuantUser::TargetMethod::UserExist); }
    }
  }
  void QuantCore::onKillUser(QString name, bool sendData)
  {
    if (lib_users->containObject(name))
    {
      if (sendData)
      {
        lib_users->getUser(name)->sendData(QuantUser::TargetMethod::UserLeave);
        foreach(QString oneName, lib_users->keys()) { lib_users->removeObject(oneName); }
      }
      else { lib_users->removeObject(name); }
      lib_users->update();
    }
  }

  // NODES /////////////////////////////////////////

  void QuantCore::onAddNode(QString nodeName, bool sendData)
  {
    QString newNodeName = lib_nodes->getUniqueName(nodeName);

    QuantNode *newNode = new QuantNode(0, this);
    newNode->setName(newNodeName);
    newNode->setFixedHeight(120);
    lib_nodes->addObject(newNode);

    connect(newNode, SIGNAL(actKilled(QString, bool)), this, SLOT(onKillNode(QString, bool)));
    connect(newNode, SIGNAL(actSizeChanged()), lib_nodes, SLOT(onUpdateObjectPosition()));


    if (sendData) { newNode->sendData(QuantNode::TargetMethod::NodeCreated); }
  }
  void QuantCore::onKillNode(QString nodeName, bool sendData)
  {
    // qDebug() << "QuantCore::onKillNode()" << nodeName;
    if (sendData) { lib_nodes->getNode(nodeName)->sendData(QuantNode::TargetMethod::NodeKilled); }

    lib_nodes->removeObject(nodeName);
    lib_nodes->update();
  }

  // OTHER /////////////////////////////////////////

  void QuantCore::onEvaluate(QString code, bool print)  { mBridge->evaluate(code, print); }
  void QuantCore::onPrint(QString message, MessageType type)
  {
    switch (type)
    {
    default:
    case MessageType::NORMAL:
      emit actPrint(message, colorMsgNormal, true);
      break;
    case MessageType::STATUS:
      emit actPrint(message, colorMsgStatus, true);
      break;
    case MessageType::EVAULATE:
      emit actPrint(message, colorMsgEvaluate, true);
      break;
    case MessageType::ANSWER:
      emit actPrint(message, colorMsgAnswer, true);
      break;
    case MessageType::ERROR:
      emit actPrint(message, colorMsgError, true);
      break;
    case MessageType::WARNING:
      emit actPrint(message, colorMsgWarning, true);
      break;
    case MessageType::BUNDLE:
      emit actPrint(message, colorMsgBundle, true);
      break;
    }

  }

  QuantCore::~QuantCore()
  {
    qDebug("Core closing...");

    qDebug("Bridge killed...");
  }
}


