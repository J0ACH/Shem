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

    nodePanel = new QuantPanel(mCanvan);
    mCanvan->addPanel(nodePanel, "NodePanel", Qt::DockWidgetArea::LeftDockWidgetArea);

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
    qDebug() << "QuantCore::onCustomize2 userName:" << userName;

    initNetworkOnStart = true;
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

    QuantUser *me = new QuantUser(0, this);
    me->setName(userName);
    me->setServerMeter(textServerMeter->getText());
    me->setServerSynth(textServerSynths->getText());
    me->setServerGroup(textServerGroups->getText());
    lib_users->addObject(me);

    me->sendData(QuantUser::TargetMethod::UserJoin);

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

    foreach(QString oneName, lib_users->keys())
    {
      lib_users->removeObject(oneName);
    }
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
    if (initServerOnStart) { mBridge->changeServerState(); }
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

    QuantNode *testNode = new QuantNode(0, this);
    testNode->setGeometry(10, 30, 30, 100);
    lib_nodes->addObject(testNode);

    this->onPrint("Server init done...\n", MessageType::STATUS);
  }
  void QuantCore::onServerKill()
  {
    qDebug("QuantCore::onServerKill");
    this->onPrint("Server kill...", MessageType::STATUS);

    lib_proxy->removeObject("proxy");

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

    /*
    if (Data::getType(data.wrap()) != Data::DataType::USER)
    {
    this->onPrint("QuantCore::onObjectDataChanged print" + data.print(), MessageType::STATUS);
    }
    */


    emit actObjectDataSend(data.wrap());
  }
  void QuantCore::onNetDataRecived(QByteArray msg)
  {
    // QString sender = Data::(msg);
    //  qDebug() << "QuantCore::onNetworkDataRecived msgOwner: " << ;

    if (Data::isFromOtherOwener(msg, userName))
    {
      QString targetObject = Data::getTarget(msg);
      QString targetMethod = Data::getMethod(msg);
      std::string targetMethod_str = targetMethod.toLatin1().constData();

      // qDebug() << "QuantCore::onNetworkDataRecived targetObject: " << targetObject;
      //  qDebug() << "QuantCore::onNetworkDataRecived targetMethod: " << targetMethod;

      //if (Data::getType(msg) != Data::DataType::USER)
      //  this->onPrint("QuantCore::onNetDataRecived", MessageType::STATUS);

      switch (Data::getType(msg))
      {
      case Data::DataType::USER:
        if (targetMethod == "onNet_UserJoin")
        {
          lib_users->addObject(msg);
          lib_users->getUser(userName)->sendData(QuantUser::TargetMethod::UserExist);
        }
        if (targetMethod == "onNet_UserExist")
        {
          lib_users->addObject(msg);
          lib_users->getUser(DataUser(msg));
        }
        QMetaObject::invokeMethod(lib_users->getUser(targetObject), targetMethod_str.c_str(), Q_ARG(DataUser, DataUser(msg)));
        if (targetMethod == "onNet_UserLeave") { lib_users->removeObject(DataUser(msg)); }
        break;
      case Data::DataType::PROXY:
        QMetaObject::invokeMethod(lib_proxy->getProxy(), targetMethod_str.c_str(), Q_ARG(DataProxy, DataProxy(msg)));
        break;
      case Data::DataType::NODE:
        qDebug() << "QuantCore::onNetworkDataRecived targetObject: " << targetObject;
        qDebug() << "QuantCore::onNetworkDataRecived targetMethod: " << targetMethod;
        QMetaObject::invokeMethod(lib_nodes->getNode(targetObject), targetMethod_str.c_str(), Q_ARG(DataNode, DataNode(msg)));
        break;
      default:
        break;
      }
    }
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


