#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "Data.h"

#include <QDebug>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QtNetwork/QUdpSocket>

namespace SupercolliderBridge
{
  class UDPServer : public QObject
  {
    Q_OBJECT

  public:
    UDPServer(QObject *parent);
    ~UDPServer();

    void initNetwork(QString name);
    void sendCode(QString code);

    public slots:
    void onSendData(Data data);
    void onDatagramRecived();

  signals:
    void actPrintNormal(QString);
    void actPrintStatus(QString);

  private:
    QUdpSocket *mSocket;

    QNetworkInterface *interface;
    QHostAddress *broadcastAddress;

    int port;
    QString userName;

    void pendingDatagramSize();
    bool isConnectedToNet();
    int addressSelector;
    bool hasBroadcast;
    void processDatagram(QString);
  };
}

#endif // end UDPServer
