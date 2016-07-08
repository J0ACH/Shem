#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "CodeEditor.h"
#include "ScBridge.h"

#include <QDebug>
#include <QLabel>
#include <QMap>
#include <QPainter>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

#include <QHostInfo>
#include <QNetworkInterface>
#include <QtNetwork/QUdpSocket>

using namespace Jui;
using namespace SupercolliderBridge;

namespace QuantIDE {

  class UDPServer : public QObject
  {
    Q_OBJECT

  public:
    UDPServer(QObject *parent, ScBridge *bridge);
    ~UDPServer();

    public slots:
    int initSocket();
    void readPendingDatagrams();
    void send(const char *input);
    void sendCode(QString code);

  signals:
    // void println(const char *input);
    void actPrintStatus(QString);
    void actPrintMsg(QString);

  private:
    //     int initSocket();
    void pendingDatagramSize();
    bool isConnectedToNet();
    int port;
    int addressSelector;
    bool hasBroadcast;
    void processDatagram(QString);
    QString getUsername();

    QNetworkInterface *interface;
    QHostAddress *broadcastAddress;
    QHostAddress *myAddress;
    QUdpSocket *socket;
    QHostInfo *host;
    QString *username;
    QString objectPattern;

    ScBridge *mBridge;
  };
}

#endif // end UDPServer
