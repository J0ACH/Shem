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

class UDPServer : public QWidget {
  Q_OBJECT

public:
  UDPServer(QWidget *parent = 0);
  ~UDPServer();

public slots:
  int initSocket();
  void readPendingDatagrams();
  void send(const char *input);

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
  void processDatagram(QByteArray);
  QString getUsername();

  QNetworkInterface *interface;
  QHostAddress *broadcastAddress;
  QHostAddress *myAddress;
  QUdpSocket *socket;
  QHostInfo *host;
  QString *username;
  QString objectPattern;
};
}

#endif // end UDPServer