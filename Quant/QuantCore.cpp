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

    connect(this, SIGNAL(actCoreInitPrepared()), this, SLOT(onInitCore()));

    connect(mBridge, SIGNAL(interpretBootDoneAct()), this, SLOT(onNetworkBootDone()));
    connect(mBridge, SIGNAL(serverBootDoneAct()), this, SLOT(onServerBootDone()));

    connect(mNetwork, SIGNAL(actNetworkBooted()), this, SLOT(onNetworkBootDone()));
    connect(mNetwork, SIGNAL(actPrint(QString, MessageType)), this, SLOT(onPrint(QString, MessageType)));

    connect(this, SIGNAL(actDataSend(QByteArray)), mNetwork, SLOT(onSendData(QByteArray)));
    connect(mNetwork, SIGNAL(actNetworkDataRecived(QByteArray)), this, SLOT(onNetworkDataRecived(QByteArray)));

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

      if (initNetworkOnStart) { mNetwork->initNetwork(userName); }
      if (initInterpretOnStart) { mBridge->changeInterpretState(); }
    }
  }

  void QuantCore::onInitNetwork()
  {
    qDebug("QuantCore::onInitNetwork");
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
    data.setOwener(userName);
    data.setTargetObject("this");
    data.setTargetMethod("networkDataRecived_semaphor");

    data.setValue(DataUser::NAME, userName);
    data.setValue(DataUser::VERSION, "0.40"); // DODELAT
    data.setValue(DataUser::BOOL_INTERPRETR, isInterpretRunning);
    data.setValue(DataUser::BOOL_SERVER, isServerRunnig);

    emit actDataSend(data.wrap());
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

  void QuantCore::initTestObjects()
  {
    proxy1 = new QuantProxy(mCanvan->centralWidget(), this);
    proxy1->setGeometry(50, 25, 300, 150);
    proxy1->show();
    connect(proxy1, SIGNAL(actDataSend(DataNEW)), this, SLOT(onSendData(DataNEW)));
    //connect(this, SIGNAL(actDataSend(DataNEW)), proxy1, SLOT(onNetworkDataRecived(DataNEW)));

    proxy2 = new QuantProxy(mCanvan->centralWidget(), this);
    proxy2->setGeometry(400, 25, 300, 150);
    proxy2->show();
    connect(proxy2, SIGNAL(actDataSend(DataNEW)), this, SLOT(onSendData(DataNEW)));
    //connect(this, SIGNAL(actDataSend(DataNEW)), proxy2, SLOT(onNetworkDataRecived(DataNEW)));
  }

  void QuantCore::onSendData(DataNEW data)
  {
    data.setOwener(userName);

    emit actDataSend(data.wrap());
  }

  void QuantCore::onNetworkDataRecived(QByteArray msg)
  {
    //  if (DataNEW::isFromOtherOwener(msg, userName))
    //  {

    DataProxy dataProxy(msg);
    QString targetObject = DataNEW::getTarget(msg);
    QString targetMethod = DataNEW::getMethod(msg);
    qDebug() << "QuantCore::onNetworkDataRecived targetObject: " << targetObject;
    qDebug() << "QuantCore::onNetworkDataRecived targetMethod: " << targetMethod;

    switch (DataNEW::getType(msg))
    {
    case DataNEW::DataType::USER:
      QMetaObject::invokeMethod(this, "networkDataRecived_semaphor", Qt::AutoConnection, Q_ARG(DataUser, DataUser(msg)));
      break;
    case DataNEW::DataType::PROXY:
      dataProxy = DataProxy(msg);
      QMetaObject::invokeMethod(proxy1, "onDataRecived", Qt::AutoConnection, Q_ARG(DataProxy, dataProxy));
      // this->onPrint("RECIVED DATA TYPE: PROXY", MessageType::STATUS);
      // this->onPrint(dataProxy.print("QuantCore::onNetworkDataRecived"));
      break;
    default:
      break;
    }
    // }

  }

  void QuantCore::networkDataRecived_semaphor(DataUser data)
  {
    qDebug() << "QuantCore::networkDataRecived_semaphor";
    this->onPrint("RECIVED DATA TYPE: USER", MessageType::STATUS);
    this->onPrint(data.print("QuantCore::networkDataRecived_semaphor"));
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


