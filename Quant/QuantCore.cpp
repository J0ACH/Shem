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

    //proxyPanel = new PanelNEW();
    //mCanvan->addPanel(proxyPanel, "ProxySpace");
    
    //networkPanel = new PanelNEW();
    //mCanvan->addPanel(networkPanel, "Network", Qt::DockWidgetArea::LeftDockWidgetArea);

    connect(this, SIGNAL(actCoreInitPrepared()), this, SLOT(onInitCore()));

    connect(mBridge, SIGNAL(interpretBootDoneAct()), this, SLOT(onInterpretBootDone()));
    connect(mBridge, SIGNAL(serverBootDoneAct()), this, SLOT(onServerBootDone()));

    connect(mBridge, SIGNAL(msgNormalAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgNormal(QString)));
    connect(mBridge, SIGNAL(msgStatusAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgStatus(QString)));
    connect(mBridge, SIGNAL(msgEvaluateAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgEvaluate(QString)));
    connect(mBridge, SIGNAL(msgResultAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgAnswer(QString)));
    connect(mBridge, SIGNAL(msgErrorAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgError(QString)));
    connect(mBridge, SIGNAL(msgWarningAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgWarning(QString)));
    connect(mBridge, SIGNAL(msgBundleAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgBundle(QString)));

    connect(mNetwork, SIGNAL(actPrintNormal(QString)), mCanvan->getPanel("Console"), SLOT(onMsgNormal(QString)));
    connect(mNetwork, SIGNAL(actPrintStatus(QString)), mCanvan->getPanel("Console"), SLOT(onMsgStatus(QString)));


    // TEST DATA WRAPPING
    qDebug("\n\n QuantCore::TEST DATA WRAPPING ////////////////////");
    Data testData;
    testData.setValue(USERNAME, "testData user name Jachym");
    testData.setValue(BOOL_BOOT_INTERPRETR, true);
    testData.setValue(FONT_CONSOLE, QFont("Consolas", 15));
    testData.print(USERNAME);
    QByteArray bArr = testData.wrap();

    Data testDataReciver(bArr);
    testDataReciver.print(USERNAME);
    qDebug("\n\nQuantCore::TEST DATA WRAPPING KONEC /////////////////////");
  }

  void QuantCore::onCustomize(Data data)
  {
    userName = data.getValue_string(DataKey::USERNAME);
    // qDebug() << "QuantCore::onCustomize userName:" << userName;
    initNetworkOnStart = true;
    initInterpretOnStart = data.getValue_bool(DataKey::BOOL_BOOT_INTERPRETR);
    initServerOnStart = data.getValue_bool(DataKey::BOOL_BOOT_SERVER);


    

    emit actCoreInitPrepared(); // ceka na initInterpretOnStart a initServerOnStart
  }

  // start procedure
  void QuantCore::onInitCore()
  {
    if (!isCoreRunning)
    {
      isCoreRunning = true;

      if (initNetworkOnStart) { mNetwork->initNetwork(userName); }
      if (initInterpretOnStart) { mBridge->changeInterpretState(); }
    }
  }
  void QuantCore::onInterpretBootDone()
  {
    qDebug("QuantCore::onInterpretBooted");
    if (initServerOnStart) { mBridge->changeServerState(); }
  }
  void QuantCore::onServerBootDone()
  {
    qDebug("QuantCore::onServerBootDone");
  }
  // start procedure



  void QuantCore::onMyMapSet(QMap <QString, QVariant> map)
  {
    foreach(QString oneKey, map.keys())
    {
      qDebug() << "Core::onMapChanged [key: " << oneKey
        << " || value: " << map.value(oneKey).toString() << "]";
    }
  }
  void QuantCore::onEvaluate(QString code)  { mBridge->evaluate(code); }

  void QuantCore::addProxySpace()
  {
    qDebug() << "Core::addProxySpace mCanvan->getScreen: " << mCanvan->centralWidget()->geometry();
    QuantProxy *proxy = new QuantProxy(mCanvan->getPanel("ProxySpace"), this);
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

  QuantCore::~QuantCore()
  {
    qDebug("Core closing...");
    mBridge->killBridge();
    qDebug("Bridge killed...");
  }
}


