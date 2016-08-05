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
#include "Library.h"
#include "NetworkPanel.h"
#include "TimePanel.h"

using namespace SupercolliderBridge;
using namespace Jui;

namespace QuantIDE
{

  class QuantCore : public QObject
  {
    Q_OBJECT

  public:
    QuantCore(CanvanNEW *canvan);
    QuantCore(CanvanNEW *canvan, QString appName, int sendPort, int listenPort);
    ~QuantCore();

    void initControls();
    //void initProxy();
    //void addNode(QString name);

    public slots :
    void onCustomize(Data);

    // CORE /////////////////////////////////////////

    void onCoreInit();
    void onCoreKill();

    // NETWORK /////////////////////////////////////////

    void onNetChangeState();
    void onNetInitDone();
    void onNetKillDone();

    // INTERPRET /////////////////////////////////////////

    void onInterpretChangeState();
    void onInterpretInit();
    void onInterpretInitDone();
    void onInterpretKill();
    void onInterpretKillDone();

    // SERVER /////////////////////////////////////////

    void onServerChangeState();
    void onServerInit();
    void onServerInitDone();
    void onServerKill();
    void onServerKillDone();
    void onServerStatus(QStringList);

    // OBJECTS /////////////////////////////////////////

    void onObjectDataChanged(DataNEW);
    void onNetDataRecived(QByteArray);

    // OTHERS /////////////////////////////////////////

    void onEvaluate(QString code, bool print = false);
    void onPrint(QString, MessageType msg = MessageType::NORMAL);


  signals:
    void actCoreInitPrepared();
    //void actCoreKillPrepared();

    void actNetworkBooted();
    void actInterpretBooted();
    void actServerBooted();

    void actPrint(QString, QColor, bool);
    void actObjectDataSend(QByteArray);


  private:
    CanvanNEW *mCanvan;
    ScBridge *mBridge;
    UDPServer *mNetwork;
    Customize *mCustomize;

    // LIBRARIES /////////////////////////////////////////

    Library lib_usersNEW;

    QuantProxy *proxy;
    QMap<QString, QuantUser*> *lib_users; // knihovna prihlasenych hracu pro komunikujici pres sit 

    NetworkPanel *networkPanel;
    TimePanel *timePanel;

    Text *textServerMeter, *textServerSynths, *textServerGroups;

    QString userName;

    bool isCoreRunning, isNetworkRunning;
    bool initNetworkOnStart, initInterpretOnStart, initServerOnStart;

    void addUser(DataUser);
    void removeUser(DataUser);

   // void setProxySpace(DataProxy);
    

    QColor colorMsgNormal, colorMsgStatus, colorMsgEvaluate, colorMsgAnswer, colorMsgError, colorMsgWarning, colorMsgBundle;

  };
}
#endif // QUANTCORE_H

