#include "QuantCore.h"

namespace QuantIDE
{
    QuantCore::QuantCore(CanvanNEW *canvan) :
    QObject(canvan),
    mCanvan(canvan),
    mBridge(new ScBridge(canvan)),
    mNetwork(new UDPServer(canvan))
  {
    qDebug("Core init...");
    isCoreRunning = false;

    proxyPanel = new PanelNEW();
    mCanvan->addPanel(proxyPanel, "ProxySpace");

    networkPanel = new PanelNEW();
    mCanvan->addPanel(networkPanel, "Network", Qt::DockWidgetArea::LeftDockWidgetArea);
       
    connect(this, SIGNAL(actCoreInitPrepared()), this, SLOT(onInitCore()));

    connect(mBridge, SIGNAL(interpretBootDoneAct()), this, SLOT(onInterpretBootDone()));
    connect(mBridge, SIGNAL(serverBootDoneAct()), this, SLOT(onServerBootDone()));

    connect(mBridge, SIGNAL(msgNormalAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgNormal(QString)));
    connect(mBridge, SIGNAL(msgStatusAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgStatus(QString)));
    connect(mBridge, SIGNAL(msgEvaluateAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgEvaluate(QString)));
    connect(mBridge, SIGNAL(msgResultAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgResult(QString)));
    connect(mBridge, SIGNAL(msgErrorAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgError(QString)));
    connect(mBridge, SIGNAL(msgWarningAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgWarning(QString)));
    connect(mBridge, SIGNAL(msgBundleAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgBundle(QString)));

    connect(mNetwork, SIGNAL(actPrintNormal(QString)), mCanvan->getPanel("Console"), SLOT(onMsgNormal(QString)));
    connect(mNetwork, SIGNAL(actPrintStatus(QString)), mCanvan->getPanel("Console"), SLOT(onMsgStatus(QString)));    
  }

  void QuantCore::onCustomize(Data data)
  {
    userName = data.getValue_string(DataKey::USERNAME);
   // qDebug() << "QuantCore::onCustomize userName:" << userName;
    initNetworkOnStart = true;
    initInterpretOnStart = data.getValue_bool(DataKey::BOOL_BOOT_INTERPRETR);
    initServerOnStart = data.getValue_bool(DataKey::BOOL_BOOT_SERVER);


    networkPanel->setColorHeader(data.getValue_color(DataKey::COLOR_PANEL_HEADER));
    networkPanel->setColorBackground(data.getValue_color(DataKey::COLOR_PANEL_BACKGROUND));
    networkPanel->setColorTitle(data.getValue_color(DataKey::COLOR_TEXT));
    networkPanel->setFontTitle(data.getValue_font(DataKey::FONT_SMALL));
    networkPanel->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    networkPanel->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    networkPanel->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));

    proxyPanel->setColorHeader(data.getValue_color(DataKey::COLOR_PANEL_HEADER));
    proxyPanel->setColorBackground(data.getValue_color(DataKey::COLOR_PANEL_BACKGROUND));
    proxyPanel->setColorTitle(data.getValue_color(DataKey::COLOR_TEXT));
    proxyPanel->setFontTitle(data.getValue_font(DataKey::FONT_SMALL));
    proxyPanel->setColorNormal(data.getValue_color(DataKey::COLOR_NORMAL));
    proxyPanel->setColorOver(data.getValue_color(DataKey::COLOR_OVER));
    proxyPanel->setColorActive(data.getValue_color(DataKey::COLOR_ACTIVE));

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


