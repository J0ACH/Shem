#include "UDPServer.h"

namespace QuantIDE {

  UDPServer::UDPServer(QWidget *parent) : QWidget(parent)
  {
    setObjectName("UDPServer");
    objectPattern = QString::null;

    this->initSocket();
    //QUdpSocket socket;
    //int port;
  }
  void UDPServer::initSocket()
  {
    port = 10000;

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
      if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        qDebug() << "UDP: local IP address: " << address.toString();
    }    

    qDebug() << "UDP: Server starting, listening at port: " << port;

    socket = new QUdpSocket(this);
    host = new QHostInfo();
    socket->bind(QHostAddress::LocalHost, port);



    if(socket->state()==4){
      qDebug() << "UDP: Server is ON!";
    }else{
      qDebug() << "UDP: There is a problem starting the server on port " << port;

    }

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
