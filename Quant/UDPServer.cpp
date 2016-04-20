#include "UDPServer.h"

#define DEBUG 1
#define PORT 10000

namespace QuantIDE {

  UDPServer::UDPServer(QWidget *parent) : QWidget(parent)
  {
    setObjectName("UDPServer");
    objectPattern = QString::null;

    if(this->initSocket()==0){
      //print to graphical console?
      //this->println("UDP Server booted");
    }
  }

  void println(const char *input){
    emit println(input);
  }

  bool UDPServer::isConnectedToNet(){
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool result = false;

    for (int i = 0; i < ifaces.count(); i++){
      QNetworkInterface iface = ifaces.at(i);
      if ( iface.flags().testFlag(QNetworkInterface::IsUp)
          && !iface.flags().testFlag(QNetworkInterface::IsLoopBack) ){

#ifdef DEBUG
        qDebug() << "UDP: got interface:" << iface.name() << "with mac:" << iface.hardwareAddress();
#endif

        for (int j=0; j<iface.addressEntries().count(); j++){
#ifdef DEBUG
          QString _broadcast = iface.addressEntries().at(j).broadcast().toString();
          qDebug() << "UDP: addr.:" << iface.addressEntries().at(j).ip().toString();
          qDebug() << "UDP: netmask:" << iface.addressEntries().at(j).netmask().toString();
          qDebug() << "UDP: broadcast:" << _broadcast;
#endif

          if (result == false){
            result = true;
            interface = new QNetworkInterface(iface);
          }

          if(_broadcast.size()>0){
            hasBroadcast=true;
            addressSelector = j;
          }
        }

#ifdef DEBUG
        if(!result)
          qDebug() << "UDP: interface seems to have no address";
#endif
      }

    }

    return result;
  }

  int UDPServer::initSocket(){
    port = PORT;

#ifdef DEBUG
    qDebug() << "///////////// UDP Server //////////////////////////////////////////";
    qDebug() << "UDP: Checking network setup...";
#endif
    if(isConnectedToNet()){
#ifdef DEBUG
      qDebug() << "UDP: Network link is ESTABLISHED";
#endif
    }else{
#ifdef DEBUG
      qDebug() << "UDP: ERROR! you are not connected to any network.";
#endif
      return 1;      
    }

    int count = 0;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
      if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
        if(count==0){
#ifdef DEBUG
          qDebug() << "UDP: local IP address: " << address.toString();
#endif
        }else{
#ifdef DEBUG
          qDebug() << "UDP WARN: multiple network addresses detected, picking first one";

#endif
        }
      }    
    }    

#ifdef DEBUG
    if(hasBroadcast){
      QString _baddress = interface->addressEntries().at(addressSelector).broadcast().toString();
      broadcastAddress = new QHostAddress(_baddress);		
      qDebug() << "UDP: OK broadcast addr.:" << _baddress;
    }else{
      qDebug() << "UDP: WARNING: network seems to have no Broadcast support, setting default one.";
      broadcastAddress = new QHostAddress("239.1.1.250");		
    }
    qDebug() << "UDP: Server starting, listening at port: " << port;
#endif

    socket = new QUdpSocket(this);
    host = new QHostInfo();
    socket->bind(QHostAddress::Any, port);



#ifdef DEBUG
    if(socket->state()==4){
      qDebug() << "UDP: Server is ON!";
    }else{
      qDebug() << "UDP: There is a problem starting the server on port " << port;
      return 1;
    }

    qDebug() << "///////////////////////////////////////////////////////////////////";
#endif

    connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    this->send("Hello there! -- test message");
    return 0;
  }

  void UDPServer::send(const char *input){

    QByteArray datagram(input);
    socket->writeDatagram(datagram.data(), datagram.size(), 
        *broadcastAddress , port);
  }

  void UDPServer::readPendingDatagrams(){

    while (socket->hasPendingDatagrams()) {

      QByteArray datagram;
      datagram.resize(socket->pendingDatagramSize());

      QHostAddress sender;
      quint16 senderPort;

      socket->readDatagram(datagram.data(), datagram.size(), &sender,
          &senderPort);

#ifdef DEBUG
      qDebug() << "UDP: Got some data chef!";
      qDebug() << "UDP: sender: " << sender;
      qDebug() << "UDP: data: " << datagram.data();
#endif

      processDatagram(datagram);
    } // end while
  }   // end void

  void UDPServer::processDatagram(QByteArray datagram){
  }

  UDPServer::~UDPServer() {};
  // end class

} // end namespace
