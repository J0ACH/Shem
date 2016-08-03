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
    UDPServer(QObject *parent, int send_port, int listen_port);
    ~UDPServer();

    bool isConnected();

    void initNetwork();
    void killNetwork();

    public slots:
    void onSendData(QByteArray data);
    void onDatagramRecived();

  signals:
    void actInitDone();
    void actKillDone();
    void actPrint(QString, MessageType type = MessageType::NORMAL);
    void actNetDataRecived(QByteArray);

  private:
    enum class State { OFF, CONNECTED };

    QUdpSocket *mSocket;
    QNetworkInterface *interface;
    QHostAddress *broadcastAddress;

    State state;
    int sendPort, listenPort;

    void pendingDatagramSize();
    bool isConnectedToNet();
    int addressSelector;
    bool hasBroadcast;
  };
}

#endif // end UDPServer
