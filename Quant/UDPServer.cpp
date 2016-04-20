#include "UDPServer.h"

namespace QuantIDE {

UDPServer::UDPServer(QWidget *parent) : QWidget(parent) {
  // setObjectName("UDPServer");
  objectPattern = QString::null;

  QUdpSocket socket;
  int port;

  void UDPServer::initSocket() {
    port = 10000;
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, port);
    connect(socket, SIGNAL(readyRead()), this SLOT(readPendingDatagrams()));
  }

  void UDPServer::readPendinDatagrams() {
    while (socket->hasPendingDatagrams()) {

      QByteArray datagram;
      datagram.resize(socket->pendingDatagramSize());

      QHostAddress sender;
      quint16 senderPort;

      socket->readDatagram(datagram.data(), datagram.size(), &sender,
                           &senderPort);
      // cout << "some data received on port: " << port << endl;
      processDatagram(datagram);
    } // end while
  }   // end void

  UDPServer::~UDPServer();
} // end class

} // end namespace
