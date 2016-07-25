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
    isInterpretRunning = false;
    isServerRunnig = false;
    isNetworkRunning = false;

    lib_users = new QMap<QString, QuantUser*>();

    networkPanel = new NetworkPanel(mCanvan, lib_users);
    mCanvan->addPanel(networkPanel, "Network", Qt::DockWidgetArea::LeftDockWidgetArea);

    networkObjects.insert("core", this);

    connect(this, SIGNAL(actCoreInitPrepared()), this, SLOT(onInitCore()));

    connect(mBridge, SIGNAL(interpretBootDoneAct()), this, SLOT(onNetworkBootDone()));
    connect(mBridge, SIGNAL(serverBootDoneAct()), this, SLOT(onServerBootDone()));

    connect(mNetwork, SIGNAL(actNetworkBooted()), this, SLOT(onNetworkBootDone()));
    connect(mNetwork, SIGNAL(actNetworkKilled()), this, SLOT(onNetworkKillDone()));
    connect(mNetwork, SIGNAL(actPrint(QString, MessageType)), this, SLOT(onPrint(QString, MessageType)));

    connect(this, SIGNAL(actDataSend(QByteArray)), mNetwork, SLOT(onSendData(QByteArray)));
    connect(mNetwork, SIGNAL(actNetDataRecived(QByteArray)), this, SLOT(onNetDataRecived(QByteArray)));

    connect(networkPanel, SIGNAL(actNetworkConnect()), this, SLOT(onInitNetwork()));
    connect(networkPanel, SIGNAL(actNetworkDisconnect()), this, SLOT(onKillNetwork()));

    // TESTS

    this->initTestObjects();

    /*
    QMap<QString, QVariant> lev1;
    QMap<QString, QVariant> lev2;

    lev1.insert("type", "proxy");
    lev1.insert("object", lev2);
    lev2.insert("code1", "aaaa");
    lev2.insert("code2", "bbbb");

    foreach(QString oneKey, lev1.keys())
    {
    qDebug() << "TEST MultiMap key:" << oneKey << " value:" << lev1.value(oneKey) << "type: " << lev1.value(oneKey).type();

    switch (lev1.value(oneKey).type())
    {
    case QVariant::Map:
    foreach(QString oneKey2, lev2.keys())
    {
    qDebug() << "\t - key2:" << oneKey2 << " value2:" << lev2.value(oneKey2);
    }
    break;
    }
    }
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

  void QuantCore::onInitCore()
  {
    qDebug("QuantCore::onInitCore");
    if (!isCoreRunning)
    {
      isCoreRunning = true;

      if (initNetworkOnStart) { this->onInitNetwork(); }
      if (initInterpretOnStart) { mBridge->changeInterpretState(); }
    }
  }

  void QuantCore::onInitNetwork()
  {
    qDebug("QuantCore::onInitNetwork");
    mNetwork->onInitNetwork(userName);
  }

  void QuantCore::onInitInterpret()
  {
    qDebug("QuantCore::onInitInterpret");
  }

  void QuantCore::onInitServer()
  {
    qDebug("QuantCore::onInitServer");
  }

  void QuantCore::onNetworkBootDone()
  {
    qDebug("QuantCore::onNetworkBootDone");

    DataUser data;
    data.setTargetObject("core");
    data.setTargetMethod("onNet_userJoined");

    data.setValue(DataUser::NAME, userName);
    data.setValue(DataUser::VERSION, "0.40"); // DODELAT
    data.setValue(DataUser::BOOL_INTERPRETR, isInterpretRunning);
    data.setValue(DataUser::BOOL_SERVER, isServerRunnig);

    this->addUser(data);
    this->onSendData(data.wrap());
  }
  void QuantCore::onKillNetwork()
  {
    foreach(QString oneName, lib_users->keys())
    {
      //lib_users->value(oneName)->close();

      QuantUser *newUser = lib_users->take(oneName);
      newUser->deleteLater();
      networkPanel->updateProfilesPosition();
    }

    //lib_users->clear();

    DataUser data;
    data.setTargetObject("core");
    data.setTargetMethod("onNet_userLeaved");
    data.setValue(DataUser::NAME, userName);
    this->onSendData(data.wrap());
  }

  void QuantCore::onInterpretBootDone()
  {
    qDebug("QuantCore::onInterpretBootDone");
    if (initServerOnStart) { mBridge->changeServerState(); }
  }
  void QuantCore::onServerBootDone()
  {
    qDebug("QuantCore::onServerBootDone");
  }


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
    dataAnswer.setValue(DataUser::BOOL_INTERPRETR, isInterpretRunning);
    dataAnswer.setValue(DataUser::BOOL_SERVER, isServerRunnig);

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

  void QuantCore::initTestObjects()
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
    mBridge->killBridge();
    qDebug("Bridge killed...");
  }
}


