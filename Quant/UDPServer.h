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

    private:
      void initSocket();
      void pendingDatagramSize();
      int port;
      QUdpSocket *socket;
      QString objectPattern;
  };
}

#endif // end UDPServer
