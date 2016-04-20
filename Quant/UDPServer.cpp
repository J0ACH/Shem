#include "UDPServer.h"

namespace QuantIDE {

  UDPServer::UDPServer(QWidget *parent) : QWidget(parent)
  {
    // setObjectName("UDPServer");
    objectPattern = QString::null;

    initSocket();
    //QUdpSocket socket;
    //int port;
  }
  void UDPServer::initSocket()
  {
    port = 10000;
    printf("UDP Server starting...\n");
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, port);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
  }

  void UDPServer::readPendingDatagrams()
  {
    printf("UDPServer::readPendinDatagrams ran!\n");	
    while (socket->hasPendingDatagrams()) {

      QByteArray datagram;
      datagram.resize(socket->pendingDatagramSize());

      QHostAddress sender;
      quint16 senderPort;

      socket->readDatagram(datagram.data(), datagram.size(), &sender,
          &senderPort);
      // cout << "some data received on port: " << port << endl;
      // processDatagram(datagram);
    } // end while
  }   // end void

  UDPServer::~UDPServer() {};
  // end class

} // end namespace
