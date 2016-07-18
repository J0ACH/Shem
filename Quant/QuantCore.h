#ifndef QUANTCORE_H
#define QUANTCORE_H

#include <QWidget>
#include <QDebug>

//#include "Customize.h"
#include "ScBridge.h"
#include "UDPServer.h"

#include "Canvan.h"
#include "QuantObjects.h"
#include "CustomizePanel.h"

using namespace SupercolliderBridge;


namespace QuantIDE
{

  class QuantCore : public QObject
  {
    Q_OBJECT

  public:
    QuantCore(CanvanNEW *canvan);
        ~QuantCore();

    void addProxySpace();
    void addNode(QString name);

    public slots :
    void onCustomize(Data);
    void onInitCore();
    void onInterpretBootDone();
    void onServerBootDone();


    void onMyMapSet(QMap <QString, QVariant>);
    void onEvaluate(QString code);


  signals:
    void actCoreInitPrepared();

    void actNetworkMapSet();


  protected:


  private:
    CanvanNEW *mCanvan;
    ScBridge *mBridge;
    UDPServer *mNetwork;
    Customize *mCustomize;

    PanelNEW *networkPanel;
    QString userName;
    bool isCoreRunning;
    bool initNetworkOnStart, initInterpretOnStart, initServerOnStart;

    //QMap <QString, QString> library;
    QuantProxy *proxy;
    QMap <QString, QuantNode> nodeLibrary;
    QMap <QString, QuantControl> controloLibrary;
    QMap <QString, QuantBus> busLibrary;

    PanelNEW *proxyPanel;
    //QMap <QString, QuantNode> library;

    

  };
}
#endif // QUANTCORE_H

