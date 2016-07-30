#include "QuantCore.h"

namespace QuantIDE
{
  QuantCore::QuantCore(CanvanNEW *canvan) :
    QObject(canvan),
    mCanvan(canvan),
    mBridge(new ScBridge(this)),
    mNetwork(new UDPServer(this))
  {
    qDebug("Core init...");

    isCoreRunning = false;
    isNetworkRunning = false;

    lib_users = new QMap<QString, QuantUser*>();

    networkPanel = new NetworkPanel(mCanvan, lib_users);
    mCanvan->addPanel(networkPanel, "NetworkPanel", Qt::DockWidgetArea::LeftDockWidgetArea);

    //networkObjects.insert("core", this);

    connect(this, SIGNAL(actCoreInitPrepared()), this, SLOT(onCoreInit()));
    connect(mCanvan, SIGNAL(actClose()), this, SLOT(onCoreKill()));


    this->initControls();
  }

  void QuantCore::initControls()
  {
    proxy = new QuantProxy(mCanvan->centralWidget(), this);
    //networkObjects.insert("proxy", proxy);
    proxy->setGeometry(50, 25, 300, 150);
    proxy->show();
    connect(proxy, SIGNAL(actDataSend(DataNEW)), this, SLOT(onSendData(DataNEW)));
    /*

    proxy2 = new QuantProxy(mCanvan->centralWidget(), this);
    proxy2->setGeometry(400, 25, 300, 150);
    proxy2->show();
    networkObjects.insert("proxy2", proxy2);
    connect(proxy2, SIGNAL(actDataSend(DataNEW)), this, SLOT(onSendData(DataNEW)));
    */
  }

  void QuantCore::onCustomize(Data data)
  {
    userName = data.getValue_string(DataKey::USERNAME);
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

    QuantUser *me = new QuantUser(networkPanel, this);
    me->setName(userName);
    me->show();

    lib_users->insert(userName, me);
    networkPanel->updateProfilesPosition();
    me->sendData(QuantUser::TargetMethod::UserJoin);

    mCanvan->getButtonBar("Bridge")->getButton("Network")->setState(Button::State::ON);
  }
  void QuantCore::onNetKillDone()
  {
    //qDebug("QuantCore::onNetKillDone");

    disconnect(mNetwork, SIGNAL(actInitDone()), this, SLOT(onNetInitDone()));
    disconnect(mNetwork, SIGNAL(actKillDone()), this, SLOT(onNetKillDone()));
    disconnect(mNetwork, SIGNAL(actNetDataRecived(QByteArray)), this, SLOT(onNetDataRecived(QByteArray)));
    disconnect(mNetwork, SIGNAL(actPrint(QString, MessageType)), this, SLOT(onPrint(QString, MessageType)));
    disconnect(this, SIGNAL(actObjectDataSend(QByteArray)), mNetwork, SLOT(onSendData(QByteArray)));

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
    //  qDebug("QuantCore::onServerInitDone");
    mCanvan->getButtonBar("Bridge")->getButton("Server")->setState(Button::State::ON);
    this->onPrint("Server init done...\n", MessageType::STATUS);
  }
  void QuantCore::onServerKill()
  {
    // qDebug("QuantCore::onServerKill");
    this->onPrint("Server kill...", MessageType::STATUS);

  }
  void QuantCore::onServerKillDone()
  {
    // qDebug("QuantCore::onServerKillDone");
    mCanvan->getButtonBar("Bridge")->getButton("Server")->setState(Button::State::OFF);

    disconnect(mBridge, SIGNAL(actServerInit()), this, SLOT(onServerInit()));
    disconnect(mBridge, SIGNAL(actServerInitDone()), this, SLOT(onServerInitDone()));
    disconnect(mBridge, SIGNAL(actServerKill()), this, SLOT(onServerKill()));
    disconnect(mBridge, SIGNAL(actServerKillDone()), this, SLOT(onServerKillDone()));

    this->onPrint("Server kill done...\n", MessageType::STATUS);
  }

  // OBJECTS /////////////////////////////////////////

  void QuantCore::onObjectDataChanged(DataNEW data)
  {
    data.setOwener(userName);
    //data.setTime(proxy time); // pridat cas odeslani

    emit actObjectDataSend(data.wrap());
  }

  void QuantCore::onNetDataRecived(QByteArray msg)
  {
    if (DataNEW::isFromOtherOwener(msg, userName))
    {
      QString targetObject = DataNEW::getTarget(msg);
      QString targetMethod = DataNEW::getMethod(msg);
      std::string targetMethod_str = targetMethod.toLatin1().constData();

      qDebug() << "QuantCore::onNetworkDataRecived targetObject: " << targetObject;
      qDebug() << "QuantCore::onNetworkDataRecived targetMethod: " << targetMethod;

      this->onPrint("QuantCore::onNetworkDataRecived targetObject:" + targetObject, MessageType::NORMAL);
      this->onPrint("QuantCore::onNetworkDataRecived targetMethod:" + targetMethod, MessageType::NORMAL);

      switch (DataNEW::getType(msg))
      {
      case DataNEW::DataType::USER:
        if (targetMethod == "onNet_UserJoin") { this->addUser(DataUser(msg)); lib_users->value(userName)->sendData(QuantUser::TargetMethod::UserExist); }
        if (targetMethod == "onNet_UserExist") { this->addUser(DataUser(msg)); }
        QMetaObject::invokeMethod(lib_users->value(targetObject), targetMethod_str.c_str(), Q_ARG(DataUser, DataUser(msg)));
        if (targetMethod == "onNet_UserLeave") { this->removeUser(DataUser(msg)); }
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

  void QuantCore::onEvaluate(QString code)  { mBridge->evaluate(code); }

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


  void QuantCore::addUser(DataUser data)
  {
    QString name = data.getValue_string(DataUser::Key::NAME);

    if (!lib_users->contains(name))
    {
      QuantUser *newUser = new QuantUser(networkPanel, this);
      newUser->setName(name);
      newUser->show();

      lib_users->insert(name, newUser);
    }
    networkPanel->updateProfilesPosition();
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


