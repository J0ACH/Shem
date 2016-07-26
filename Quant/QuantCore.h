#ifndef QUANTCORE_H
#define QUANTCORE_H

#include <QWidget>
#include <QDebug>
#include <QMetaObject>
#include <QMetaMethod>


#include "Customize.h"
#include "ScBridge.h"
#include "UDPServer.h"
#include "Data.h"

#include "Canvan.h"
#include "QuantObjects.h"
#include "NetworkPanel.h"

using namespace SupercolliderBridge;
using namespace Jui;

namespace QuantIDE
{

  class QuantCore : public QObject
  {
    Q_OBJECT

  public:
    QuantCore(CanvanNEW *canvan);
    ~QuantCore();

    void initTestObjects();
    //void addProxySpace();
    //void addNode(QString name);

    public slots :
    void onCustomize(Data);
    
    // CORE /////////////////////////////////////////

    void onCoreInit();
    void onCoreKill();
    
    // NETWORK /////////////////////////////////////////

    void onNetInit();
    void onNetInitDone();
    void onNetKill();
    void onNetKillDone();

    // INTERPRET /////////////////////////////////////////

    void onInterpretInit();
    void onInterpretInitDone();
    void onInterpretKill();
    void onInterpretKillDone();
    
    // SERVER /////////////////////////////////////////

    void onServerInit();
    void onServerInitDone();
    void onServerKill();
    void onServerKillDone();

   // void onServerBootDone();

    void onSendData(DataNEW);
    void onNetDataRecived(QByteArray);

    void onNet_userJoined(DataUser);
    void onNet_userIsHere(DataUser);
    void onNet_userLeaved(DataUser);

    void onEvaluate(QString code);
    void onPrint(QString, MessageType msg = MessageType::NORMAL);


  signals:
    void actCoreInitPrepared();
    //void actCoreKillPrepared();

    void actNetworkBooted();
    void actInterpretBooted();
    void actServerBooted();

    void actPrint(QString, QColor, bool);
    void actDataSend(QByteArray);


  private:
    CanvanNEW *mCanvan;
    ScBridge *mBridge;
    UDPServer *mNetwork;
    Customize *mCustomize;

    QMap<QString, QuantUser*> *lib_users; // knihovna prihlasenych hracu pro komunikujici pres sit 
    NetworkPanel *networkPanel;

    QMap<QString, QObject*> networkObjects; // knihovna pro vsechny objekty komunikujici pres sit 

    QString userName;
    bool isCoreRunning, isInterpretRunning, isServerRunnig, isNetworkRunning;
    bool initNetworkOnStart, initInterpretOnStart, initServerOnStart;

    void addUser(DataUser);
    void removeUser(DataUser);


    QuantProxy *proxy1, *proxy2;

    //QMap <QString, QString> library;
    QuantProxy *proxy;
    QMap <QString, QuantNode> nodeLibrary;
    QMap <QString, QuantControl> controloLibrary;
    QMap <QString, QuantBus> busLibrary;

    //PanelNEW *networkPanel;
    //PanelNEW *proxyPanel;
    //QMap <QString, QuantNode> library;

    QColor colorMsgNormal, colorMsgStatus, colorMsgEvaluate, colorMsgAnswer,
      colorMsgError, colorMsgWarning, colorMsgBundle;

  };
}
#endif // QUANTCORE_H

