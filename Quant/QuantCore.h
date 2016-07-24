#ifndef QUANTCORE_H
#define QUANTCORE_H

#include <QWidget>
#include <QDebug>
#include <QMetaObject>
#include <QMetaMethod>


#include "ScBridge.h"
#include "UDPServer.h"
#include "Data.h"

#include "Canvan.h"
#include "QuantObjects.h"
#include "CustomizePanel.h"

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

    void onInitCore();
    void onInitNetwork();
    void onInitInterpret();
    void onInitServer();

    void onNetworkBootDone();
    void onInterpretBootDone();
    void onServerBootDone();

    void onSendData(DataNEW);
    void onNetDataRecived(QByteArray);

    void onNet_userJoined(DataUser);
    void onNet_userIsHere(DataUser);

    void onEvaluate(QString code);
    void onPrint(QString, MessageType msg = MessageType::NORMAL);


  signals:
    void actCoreInitPrepared();

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

    QMap<QString, QObject*> networkObjects; // knihovna pro vsechny objekty komunikujici pres sit 

    QString userName;
    bool isCoreRunning, isInterpretRunning, isServerRunnig, isNetworkRunning;
    bool initNetworkOnStart, initInterpretOnStart, initServerOnStart;




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

