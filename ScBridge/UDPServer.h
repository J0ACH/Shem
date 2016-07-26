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
    void killNetwork();

    public slots:
    void onSendData(QByteArray data);
    void onDatagramRecived();

  signals:
    void actNetworkBooted();
    void actNetworkKilled();
    void actPrint(QString, MessageType type = MessageType::NORMAL);
    void actNetDataRecived(QByteArray);

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
  };
}

#endif // end UDPServer
