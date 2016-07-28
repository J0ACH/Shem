#include "QuantCore.h"

namespace QuantIDE
{
  QuantCore::QuantCore(CanvanNEW *canvan) :
    QObject(canvan),
    mCanvan(canvan),
    mBridge(new ScBridge(this)),
    mNetwork(NULL)
  {
    qDebug("Core init...");

    isCoreRunning = false;
    //isInterpretRunning = false;
    //isServerRunnig = false;
    isNetworkRunning = false;

    lib_users = new QMap<QString, QuantUser*>();

    networkPanel = new NetworkPanel(mCanvan, lib_users);
    mCanvan->addPanel(networkPanel, "Network", Qt::DockWidgetArea::LeftDockWidgetArea);
    connect(networkPanel, SIGNAL(actNetworkConnect()), this, SLOT(onNetInit()));
    connect(networkPanel, SIGNAL(actNetworkDisconnect()), this, SLOT(onNetKill()));

    networkObjects.insert("core", this);

    connect(this, SIGNAL(actCoreInitPrepared()), this, SLOT(onCoreInit()));
    connect(mCanvan, SIGNAL(actClose()), this, SLOT(onCoreKill()));


    this->initControls();
  }

  void QuantCore::initControls()
  {
    proxy1 = new QuantProxy(mCanvan->centralWidget(), this);
    networkObjects.insert("proxy1", proxy1);
    proxy1->setGeometry(50, 25, 300, 150);
    proxy1->show();
    connect(proxy1, SIGNAL(actDataSend(DataNEW)), this, SLOT(onSendData(DataNEW)));

    proxy2 = new QuantProxy(mCanvan->centralWidget(), this);
    proxy2->setGeometry(400, 25, 300, 150);
    proxy2->show();
    networkObjects.insert("proxy2", proxy2);
    connect(proxy2, SIGNAL(actDataSend(DataNEW)), this, SLOT(onSendData(DataNEW)));
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

      if (initNetworkOnStart) { this->onNetInit(); }
      if (initInterpretOnStart) { this->onInterpretInit(); }
    }
  }
  void QuantCore::onCoreKill()
  {
    //qDebug("QuantCore::onCoreKill");
    mBridge->killBridge();
    mCanvan->onCanvanClose();
  }

  // NETWORK /////////////////////////////////////////

  void QuantCore::onNetInit()
  {
    // qDebug("QuantCore::onInitNetwork");
    mNetwork = new UDPServer(this);

    connect(mNetwork, SIGNAL(actInitDone()), this, SLOT(onNetInitDone()));
    connect(mNetwork, SIGNAL(actKillDone()), this, SLOT(onNetKillDone()));
    connect(mNetwork, SIGNAL(actPrint(QString, MessageType)), this, SLOT(onPrint(QString, MessageType)));
    connect(mNetwork, SIGNAL(actNetDataRecived(QByteArray)), this, SLOT(onNetDataRecived(QByteArray)));

    connect(this, SIGNAL(actDataSend(QByteArray)), mNetwork, SLOT(onSendData(QByteArray)));

    mNetwork->initNetwork();
  }
  void QuantCore::onNetInitDone()
  {
    // qDebug("QuantCore::onNetworkBootDone");

    DataUser data;
    data.setTargetObject("core");
    data.setTargetMethod("onNet_userJoined");

    data.setValue(DataUser::NAME, userName);
    data.setValue(DataUser::VERSION, "0.40"); // DODELAT
    data.setValue(DataUser::BOOL_INTERPRETR, mBridge->isInterpretRunning());
    data.setValue(DataUser::BOOL_SERVER, mBridge->isServerRunning());

    this->addUser(data);
    this->onSendData(data.wrap());
  }
  void QuantCore::onNetKill()
  {
    // qDebug("QuantCore::onNetKill");
    foreach(QString oneName, lib_users->keys())
    {
      qDebug() << "QuantCore::onNetKill oneName:" << oneName;

      QuantUser *newUser = lib_users->take(oneName);
      newUser->close();
      networkPanel->updateProfilesPosition();
    }

    DataUser data;
    data.setTargetObject("core");
    data.setTargetMethod("onNet_userLeaved");
    data.setValue(DataUser::NAME, userName);
    this->onSendData(data.wrap());

    mNetwork->killNetwork();
  }
  void QuantCore::onNetKillDone()
  {
    mNetwork->deleteLater();
  }

  // INTERPRET /////////////////////////////////////////

  void QuantCore::onInterpretInit()
  {
    qDebug("QuantCore::onInitInterpret");
        
    if (!mBridge->isInterpretRunning())
    {
      connect(mBridge, SIGNAL(actInterpretInitDone()), this, SLOT(onInterpretInitDone()));
      connect(mBridge, SIGNAL(actInterpretKill()), this, SLOT(onInterpretKill()));
      connect(mBridge, SIGNAL(actInterpretKillDone()), this, SLOT(onInterpretKillDone()));

      connect(mBridge, SIGNAL(actPrint(QString, MessageType)), this, SLOT(onPrint(QString, MessageType)));

      connect(mBridge, SIGNAL(actServerInit()), this, SLOT(onServerInit()));
      connect(mBridge, SIGNAL(actServerInitDone()), this, SLOT(onServerInitDone()));
      connect(mBridge, SIGNAL(actServerKill()), this, SLOT(onServerKill()));
      connect(mBridge, SIGNAL(actServerKillDone()), this, SLOT(onServerKillDone()));

      this->onPrint("Interpretr init...", MessageType::STATUS);
    }

    mBridge->changeInterpretState();
  }
  void QuantCore::onInterpretInitDone()
  {
    qDebug("QuantCore::onInterpretBootDone");
    mCanvan->getButtonBar("Bridge")->getButton("Interpretr")->setState(Button::State::ON);
    mCanvan->getButtonBar("Bridge")->getButton("Server")->setState(Button::State::OFF);
    this->onPrint("Interpretr init done...\n", MessageType::STATUS);
    if (initServerOnStart) { mBridge->changeServerState(); }
  }
  void QuantCore::onInterpretKill()
  {
    qDebug("QuantCore::onInterpretKill");
    this->onPrint("Interpretr kill...", MessageType::STATUS);

  }
  void QuantCore::onInterpretKillDone()
  {
    qDebug("QuantCore::onInterpretKillDone");
    mCanvan->getButtonBar("Bridge")->getButton("Interpretr")->setState(Button::State::OFF);
    mCanvan->getButtonBar("Bridge")->getButton("Server")->setState(Button::State::FROZEN);
    this->onPrint("Interpretr kill done...\n", MessageType::STATUS);

    disconnect(mBridge, SIGNAL(actInterpretInitDone()), this, SLOT(onInterpretInitDone()));
    disconnect(mBridge, SIGNAL(actInterpretKill()), this, SLOT(onInterpretKill()));
    disconnect(mBridge, SIGNAL(actInterpretKillDone()), this, SLOT(onInterpretKillDone()));

    disconnect(mBridge, SIGNAL(actPrint(QString, MessageType)), this, SLOT(onPrint(QString, MessageType)));

    disconnect(mBridge, SIGNAL(actServerInit()), this, SLOT(onServerInit()));
    disconnect(mBridge, SIGNAL(actServerInitDone()), this, SLOT(onServerInitDone()));
    disconnect(mBridge, SIGNAL(actServerKill()), this, SLOT(onServerKill()));
    disconnect(mBridge, SIGNAL(actServerKillDone()), this, SLOT(onServerKillDone()));
  }

  // SERVER /////////////////////////////////////////

  void QuantCore::onServerChangeState()
  {
    if (mBridge->isInterpretRunning())
    {
      qDebug("QuantCore::onServerChangeState");
      mBridge->changeServerState();
    }
  }
  void QuantCore::onServerInit()
  {
    qDebug("QuantCore::onServerInit");
    this->onPrint("Server init...", MessageType::STATUS);
  }
  void QuantCore::onServerInitDone()
  {
    // isServerRunnig = true;
    qDebug("QuantCore::onServerInitDone");
    mCanvan->getButtonBar("Bridge")->getButton("Server")->setState(Button::State::ON);
    this->onPrint("Server init done...\n", MessageType::STATUS);
  }
  void QuantCore::onServerKill()
  {
    qDebug("QuantCore::onServerKill");
    this->onPrint("Server kill...", MessageType::STATUS);
  }
  void QuantCore::onServerKillDone()
  {
    // isServerRunnig = false;
    qDebug("QuantCore::onServerKillDone");
    mCanvan->getButtonBar("Bridge")->getButton("Server")->setState(Button::State::OFF);
    this->onPrint("Server kill done...\n", MessageType::STATUS);
  }

  // OTHER /////////////////////////////////////////

  void QuantCore::onSendData(DataNEW data)
  {
    data.setOwener(userName);
    //data.setTime(proxy time); // pridat cas odeslani

    emit actDataSend(data.wrap());
  }

  void QuantCore::onNetDataRecived(QByteArray msg)
  {
    if (DataNEW::isFromOtherOwener(msg, userName))
    {
      QString targetObject = DataNEW::getTarget(msg);
      QString targetMethod = DataNEW::getMethod(msg);
      std::string targetMethod_str = targetMethod.toLatin1().constData();

      // qDebug() << "QuantCore::onNetworkDataRecived targetObject: " << targetObject;
      // qDebug() << "QuantCore::onNetworkDataRecived targetMethod: " << targetMethod;

      switch (DataNEW::getType(msg))
      {
      case DataNEW::DataType::USER:
        QMetaObject::invokeMethod(networkObjects.value(targetObject), targetMethod_str.c_str(), Q_ARG(DataUser, DataUser(msg)));
        break;
      case DataNEW::DataType::PROXY:
        QMetaObject::invokeMethod(networkObjects.value(targetObject), targetMethod_str.c_str(), Q_ARG(DataProxy, DataProxy(msg)));
        break;
      default:
        break;
      }
    }

  }

  void QuantCore::onNet_userJoined(DataUser data)
  {
    qDebug() << "QuantCore::onNet_userJoined";

    this->onPrint("User \"" + data.getValue_string(DataUser::NAME) + "\" has been connected to session", MessageType::STATUS);
    this->addUser(data);

    // odpoved na prihlaseni, jsem zde
    DataUser dataAnswer;
    dataAnswer.setTargetObject("core");
    dataAnswer.setTargetMethod("onNet_userIsHere");

    dataAnswer.setValue(DataUser::NAME, userName);
    dataAnswer.setValue(DataUser::VERSION, "0.40"); // DODELAT
    dataAnswer.setValue(DataUser::BOOL_INTERPRETR, mBridge->isInterpretRunning());
    dataAnswer.setValue(DataUser::BOOL_SERVER, mBridge->isServerRunning());

    this->onSendData(dataAnswer.wrap());
  }

  void QuantCore::onNet_userIsHere(DataUser data)
  {
    qDebug() << "QuantCore::onNet_userIsHere";

    this->onPrint("User \"" + data.getValue_string(DataUser::NAME) + "\" is already connected", MessageType::STATUS);
    this->addUser(data);
  }

  void QuantCore::onNet_userLeaved(DataUser data)
  {
    qDebug() << "QuantCore::onNet_userLeaved";

    this->onPrint("User \"" + data.getValue_string(DataUser::NAME) + "\" exit session", MessageType::STATUS);
    this->removeUser(data);
  }


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

    QuantUser *newUser = new QuantUser(networkPanel, this);
    newUser->setName(name);
    newUser->show();

    lib_users->insert(name, newUser);
    networkPanel->updateProfilesPosition();
  }
  void QuantCore::removeUser(DataUser data)
  {
    QString name = data.getValue_string(DataUser::Key::NAME);

    QuantUser *newUser = lib_users->take(name);
    newUser->close();
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


