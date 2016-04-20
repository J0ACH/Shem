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

#include <QtNetwork/QUdpSocket>
#include <QHostInfo>
#include <QNetworkInterface>

using namespace Jui;
using namespace SupercolliderBridge;

namespace QuantIDE {

  class UDPServer : public QWidget
  {
    Q_OBJECT

    public:
      UDPServer(QWidget *parent = 0);
      ~UDPServer();

      public slots:
      void readPendingDatagrams();
      void send(const char *input);

    private:
      void initSocket();
      void pendingDatagramSize();
      bool isConnectedToNet();
      int port;
      int hasBroadcast;


      QHostAddress *broadcastAddress;
      QHostAddress *myAddress;
      QUdpSocket *socket;
      QHostInfo *host;
      QString objectPattern;
  };
}

#endif // end UDPServer
