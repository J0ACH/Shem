#include "QuantCore.h"

namespace QuantIDE
{
  QuantCore::QuantCore(CanvanNEW *canvan) :
    QObject(canvan),
    mCanvan(canvan),
    mBridge(new ScBridge(this, "_normal")),
    mNetwork(new UDPServer(this)),
    lib_usersNEW(0, this)
  {
    qDebug("Core init...");

    isCoreRunning = false;
    isNetworkRunning = false;

    lib_users = new QMap<QString, QuantUser*>();
    networkPanel = new NetworkPanel(mCanvan, lib_users);
    mCanvan->addPanel(networkPanel, "NetworkPanel", Qt::DockWidgetArea::LeftDockWidgetArea);

    proxy = NULL;
    timePanel = new TimePanel(mCanvan);
    mCanvan->addPanel(timePanel, "TimePanel", Qt::DockWidgetArea::LeftDockWidgetArea);

    connect(this, SIGNAL(actCoreInitPrepared()), this, SLOT(onCoreInit()));
    connect(mCanvan, SIGNAL(actClose()), this, SLOT(onCoreKill()));


    this->initControls();
  }

  QuantCore::QuantCore(CanvanNEW *canvan, QString appName, int sendPort, int listenPort) :
    QObject(canvan),
    mCanvan(canvan),
    mBridge(new ScBridge(this, "_" + appName)),
    mNetwork(new UDPServer(this, sendPort, listenPort)),
    lib_usersNEW(0, this)
  {
    userName = appName;

    isCoreRunning = false;
    isNetworkRunning = false;

    lib_users = new QMap<QString, QuantUser*>();
    networkPanel = new NetworkPanel(mCanvan, lib_users);
    mCanvan->addPanel(networkPanel, "NetworkPanel", Qt::DockWidgetArea::LeftDockWidgetArea);

    proxy = NULL;
    timePanel = new TimePanel(mCanvan);
    timePanel->setVisible(false);
    mCanvan->addPanel(timePanel, "TimePanel", Qt::DockWidgetArea::LeftDockWidgetArea);

    connect(this, SIGNAL(actCoreInitPrepared()), this, SLOT(onCoreInit()));
    connect(mCanvan, SIGNAL(actClose()), this, SLOT(onCoreKill()));

    colorMsgNormal = QColor(120, 120, 120);
    this->initControls();
  }

  void QuantCore::initControls()
  {
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

    // lib_usersNEW = Library(0, this);
    lib_usersNEW.display();
  }

  void QuantCore::onCustomize(Data data)
  {
#if (!_DEBUG)
    userName = data.getValue_string(DataKey::USERNAME);
#endif
    // qDebug() << "QuantCore::onCustomize userName:" << userName;
    initNetworkOnStart = true;
    initInterpretOnStart = data.getValue_bool(DataKey::BOOL_BOOT_INTERPRETR);
    initServerOnStart = data.getValue_bool(DataKey::BOOL_BOOT_SERVER);

    colorMsgNormal = data.getValue_color(DataKey::COLOR_MSG_NORMAL);
    colorMsgStatus = data.getValue_color(DataKey::COLOR_MSG_STATUS);
    colorMsgEvaluate = data.getValue_color(DataKey::COLOR_MSG_EVALUATE);
    colorMsgAnswer = data.getValue_color(DataKey::COLOR_MSG_ANSWER);
    colorMsgError = data.getValue_color(DataKey::COLOR_MSG_ERROR);
    colorMsgWarning = data.getValue_color(DataKey::COLOR_MSG_WARNINIG);
    colorMsgBundle = data.getValue_color(DataKey::COLOR_MSG_BUNDLE);


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
      lib_users->value(userName)->sendData(QuantUser::TargetMethod::UserLeave);
      mNetwork->killNetwork();
    }
  }
  void QuantCore::onNetInitDone()
  {
    qDebug("QuantCore::onNetworkBootDone");

    QuantUser *me = new QuantUser(networkPanel->getScrollWidget(), this);
    me->setName(userName);
    me->setServerMeter(textServerMeter->getText());
    me->setServerSynth(textServerSynths->getText());
    me->setServerGroup(textServerGroups->getText());
    me->show();

    lib_usersNEW.addObject(me);

    // lib_users->insert(userName, me);
    // networkPanel->updateProfilesPosition();
    me->sendData(QuantUser::TargetMethod::UserJoin);

    if (mBridge->isServerRunning())
    {
      connect(mBridge, SIGNAL(actServerStatus(QStringList)), lib_users->value(userName), SLOT(onServerStatus(QStringList)));

      proxy->setBPM(proxy->getBPM()); // PROBLEM - JE TU JEN PRO ZPOMALENI PRED DOTAZEM NA EXISTENCI PROXY, PROC????
      proxy->sendData(QuantProxy::TargetMethod::ProxyExist);
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
      lib_users->value(oneName)->close();
      lib_users->remove(oneName);
    }
    networkPanel->updateProfilesPosition();

    lib_users->clear();
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
          connect(mBridge, SIGNAL(actServerStatus(QStringList)), lib_users->value(userName), SLOT(onServerStatus(QStringList)));
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
    // POZOR - PROBEHNE 2X - CHYBA ZE SERVERU A VOLANI SERVER.DEFAULT A SERVER.BOOT
    qDebug("QuantCore::onServerInitDone");

    mCanvan->getButtonBar("Bridge")->getButton("Server")->setState(Button::State::ON);
    this->onPrint("Server init done...\n", MessageType::STATUS);

    qDebug() << "QuantCore::onServerInitDone ISNULL:" << (proxy == NULL);
    if (proxy == NULL)
    {
      proxy = new QuantProxy(mCanvan->getPanel("TimePanel"), this);
      proxy->setGeometry(10, 30, mCanvan->getPanel("TimePanel")->width() - 20, 150);
      proxy->show();
      timePanel->insertProxy(proxy);

      proxy->sendData(QuantProxy::TargetMethod::ProxyExist);
    }
    mBridge->initOSC();
  }
  void QuantCore::onServerKill()
  {
    qDebug("QuantCore::onServerKill");
    this->onPrint("Server kill...", MessageType::STATUS);

    proxy->deleteLater();
    proxy = NULL;
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
    disconnect(mBridge, SIGNAL(actServerStatus(QStringList)), lib_users->value(userName), SLOT(onServerStatus(QStringList)));

    textServerMeter->setText("NaN");
    textServerSynths->setText("0");
    textServerGroups->setText("0");

    lib_users->value(userName)->setServerMeter("NaN");
    lib_users->value(userName)->setServerSynth("0");
    lib_users->value(userName)->setServerGroup("0");

    lib_users->value(userName)->sendData(QuantUser::TargetMethod::UserServerStatus);

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

  void QuantCore::onObjectDataChanged(DataNEW data)
  {
    data.setSender(userName);
    //data.setTime(proxy time); // pridat cas odeslani

    /*
    if (DataNEW::getType(data.wrap()) != DataNEW::DataType::USER)
    {
    //this->onPrint("QuantCore::onObjectDataChanged print" + data.print(), MessageType::STATUS);
    }
    */

    emit actObjectDataSend(data.wrap());
  }
  void QuantCore::onNetDataRecived(QByteArray msg)
  {
    if (DataNEW::isFromOtherOwener(msg, userName))
    {
      QString targetObject = DataNEW::getTarget(msg);
      QString targetMethod = DataNEW::getMethod(msg);
      std::string targetMethod_str = targetMethod.toLatin1().constData();

      //  qDebug() << "QuantCore::onNetworkDataRecived targetObject: " << targetObject;
      //  qDebug() << "QuantCore::onNetworkDataRecived targetMethod: " << targetMethod;

      //if (DataNEW::getType(msg) != DataNEW::DataType::USER)
      //  this->onPrint("QuantCore::onNetDataRecived", MessageType::STATUS);

      switch (DataNEW::getType(msg))
      {
      case DataNEW::DataType::USER:
        // lib_usersNEW.addObject(DataUser(msg));

        if (targetMethod == "onNet_UserJoin") { lib_usersNEW.addObject(DataUser(msg)); lib_usersNEW.getUser(userName)->sendData(QuantUser::TargetMethod::UserExist); }
        if (targetMethod == "onNet_UserExist") { lib_usersNEW.addObject(DataUser(msg)); lib_usersNEW.getUser(DataUser(msg)); }
        QMetaObject::invokeMethod(lib_usersNEW.getUser(targetObject), targetMethod_str.c_str(), Q_ARG(DataUser, DataUser(msg)));
        if (targetMethod == "onNet_UserLeave") { lib_usersNEW.removeObject(DataUser(msg)); }
        /*
        if (targetMethod == "onNet_UserJoin") { this->addUser(DataUser(msg)); lib_users->value(userName)->sendData(QuantUser::TargetMethod::UserExist); }
        if (targetMethod == "onNet_UserExist") { this->addUser(DataUser(msg)); }
        QMetaObject::invokeMethod(lib_users->value(targetObject), targetMethod_str.c_str(), Q_ARG(DataUser, DataUser(msg)));
        if (targetMethod == "onNet_UserLeave") { this->removeUser(DataUser(msg)); }
        */
        break;
      case DataNEW::DataType::PROXY:
        QMetaObject::invokeMethod(proxy, targetMethod_str.c_str(), Q_ARG(DataProxy, DataProxy(msg)));
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

  /*
  void QuantCore::initProxy()
  {
  QString code;
  code =
  "p = ProxySpace.push(s).makeTempoClock;"
  "p.clock.tempo_(60 / 60);";

  this->onEvaluate(code);
  }
  */




  void QuantCore::addUser(DataUser data)
  {
    QString name = data.getValue_string(DataUser::Key::NAME);

    if (!lib_users->contains(name))
    {
      QuantUser *newUser = new QuantUser(networkPanel->getScrollWidget(), this);
      newUser->setName(name);
      newUser->show();

      //lib_users->insert(name, newUser);
      lib_usersNEW.addObject(newUser);
    }
    //networkPanel->updateProfilesPosition();
  }
  void QuantCore::removeUser(DataUser data)
  {
    QString name = data.getValue_string(DataUser::Key::NAME);

    if (lib_users->contains(name))
    {
      lib_users->value(name)->close();
      lib_users->remove(name);
    }
    networkPanel->updateProfilesPosition();
  }
  /*
  void QuantCore::setProxySpace(DataProxy)
  {

  }
  */

  /*
  void QuantCore::addProxySpace()
  {
  //qDebug() << "Core::addProxySpace mCanvan->getScreen: " << mCanvan->centralWidget()->geometry();
  QuantProxy *proxy = new QuantProxy(mCanvan->getPanel("Time"), this);
  proxy->setGeometry(50, 25, 100, 100);
  proxy->show();

  //mCanvan->update();

  //proxy->setName("testProxy");
  }

  void QuantCore::addNode(QString name)
  {

  QuantNode *testNode = new QuantNode(mCanvan->centralWidget(), this);
  testNode->setGeometry(50, 150, 100, 100);
  testNode->show();
  testNode->setName(name);


  }
  */



  QuantCore::~QuantCore()
  {
    qDebug("Core closing...");

    qDebug("Bridge killed...");
  }
}


