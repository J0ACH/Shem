#include "UDPServer.h"

#define DEBUG 1

namespace QuantIDE {

  UDPServer::UDPServer(QWidget *parent) : QWidget(parent)
  {
    setObjectName("UDPServer");
    objectPattern = QString::null;

    this->initSocket();
    //QUdpSocket socket;
    //int port;
  }

  bool UDPServer::isConnectedToNet(){
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool result = false;

    for (int i = 0; i < ifaces.count(); i++)
    {
      QNetworkInterface iface = ifaces.at(i);
      if ( iface.flags().testFlag(QNetworkInterface::IsUp)
	  && !iface.flags().testFlag(QNetworkInterface::IsLoopBack) )
      {

#ifdef DEBUG
	qDebug() << "UDP: got interface:" << iface.name() << "with mac:" << iface.hardwareAddress();
#endif

	for (int j=0; j<iface.addressEntries().count(); j++)
	{
#ifdef DEBUG
	  qDebug() << "UDP: " << iface.addressEntries().at(j).ip().toString()
	    << " | " << iface.addressEntries().at(j).netmask().toString();
#endif

	  if (result == false)
	    result = true;
	}
      }

    }

    return result;
  }

  void UDPServer::initSocket()
  {
    port = 10000;

    qDebug() << "///////////// UDP Server //////////////////////////////////////////";

    qDebug() << "UDP: Checking network setup...";
    if(isConnectedToNet()){
      qDebug() << "UDP: Are we connected?";
      qDebug() << "UDP: Link ESTABLISHED";
    }else{
      qDebug() << "UDP: Are we connected?";
      qDebug() << "UDP: ERROR! you are not connected to any network.";
      return;      
    }

    int count = 0;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
      if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
	if(count==0){
	  qDebug() << "UDP: local IP address: " << address.toString();
	  //broadcastAddress = new QHostAddress("239.20.4.1");		
	  broadcastAddress = new QHostAddress("172.20.4.255");		
	}else{
	  qDebug() << "UDP WARN: multiple network addresses detected, picking first one";
	}
      }    
    }    

    hasBroadcast = QHostAddress::Broadcast;

    qDebug() << "UDP: Server starting, listening at port: " << port;
    if(hasBroadcast==1){
      qDebug() << "UDP: OK network seems to have broadcast support!";
    }else{
      qDebug() << "UDP: NO network don't have broadcast support!";

    }

    socket = new QUdpSocket(this);
    host = new QHostInfo();
    socket->bind(QHostAddress::Any, port);


    if(socket->state()==4){
      qDebug() << "UDP: Server is ON!";
    }else{
      qDebug() << "UDP: There is a problem starting the server on port " << port;

    }

    qDebug() << "///////////////////////////////////////////////////////////////////";

    connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    this->sendHello();
  }

  void UDPServer::sendHello(){
    QByteArray datagram("Hello there!");
    socket->writeDatagram(datagram.data(), datagram.size(), 
	*broadcastAddress , port);
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
      qDebug() << "got some data!";
      // processDatagram(datagram);
    } // end while
  }   // end void

  UDPServer::~UDPServer() {};
  // end class

} // end namespace
