#include "QuantCore.h"

namespace QuantIDE
{
  QuantCore::QuantCore(CanvanNEW *canvan, Customize *customize) :
    QObject(canvan),
    mCanvan(canvan),
    mCustomize(customize),
    mBridge(new ScBridge(canvan)),
    mNetwork(new UDPServer(canvan, mBridge))
  {
    qDebug("Core init...");

    proxyPanel = new PanelNEW();
    proxyPanel->setFontTitle(customize->getFont("font_shem_TextSmall"));
    proxyPanel->setColorTitle(customize->getColor("color_shem_Over"));
    mCanvan->addPanel(proxyPanel, "ProxySpace");

    initInterpretOnStart = mCustomize->getBool("bool_shem_onStartBoot_Interpret");
    initServerOnStart = mCustomize->getBool("bool_shem_onStartBoot_Server");

    qDebug() << "QuantCore::initInterpretOnStart: " << initInterpretOnStart;
    qDebug() << "QuantCore::initServerOnStart: " << initServerOnStart;

    connect(mBridge, SIGNAL(interpretBootDoneAct()), this, SLOT(onInterpretBootDone()));
    connect(mBridge, SIGNAL(serverBootDoneAct()), this, SLOT(onServerBootDone()));

    connect(mBridge, SIGNAL(msgNormalAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgNormal(QString)));
    connect(mBridge, SIGNAL(msgStatusAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgStatus(QString)));
    connect(mBridge, SIGNAL(msgEvaluateAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgEvaluate(QString)));
    connect(mBridge, SIGNAL(msgResultAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgResult(QString)));
    connect(mBridge, SIGNAL(msgErrorAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgError(QString)));
    connect(mBridge, SIGNAL(msgWarningAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgWarning(QString)));
    connect(mBridge, SIGNAL(msgBundleAct(QString)), mCanvan->getPanel("Console"), SLOT(onMsgBundle(QString)));

    if (initInterpretOnStart) { mBridge->changeInterpretState(); }
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

  void QuantCore::onCustomize(Customize *custom)
  {
    qDebug() << "QuantCore::onCustomize";
  }

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


