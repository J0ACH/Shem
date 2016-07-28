#include "UDPServer.h"

namespace SupercolliderBridge {

  UDPServer::UDPServer(QObject *parent) :
    QObject(parent),
    mSocket(new QUdpSocket(this))
  {
    // qDebug("UDPServer:: new...");
    state = State::OFF;
    port = 10000;
  }

  bool UDPServer::isConnected()
  {
    switch (state)
    {
    case SupercolliderBridge::UDPServer::State::OFF: return false; break;
    case SupercolliderBridge::UDPServer::State::CONNECTED: return true; break;
    }
    return false;
  }

  void UDPServer::initNetwork()
  {
    mSocket->bind(QHostAddress::Any, port);
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(onDatagramRecived()));

    //qDebug() << "UDPServer::initNetwork";
    emit actPrint("Network init...", MessageType::STATUS);

    if (isConnectedToNet())
    {
      // qDebug("UDP: Network link is estabilished");
      emit actPrint("Network link is estabilished");
    }
    else
    {
      //qDebug("UDP: ERROR! you are not connected to any network.\n"); 
      actPrint("You are not connected to any network", MessageType::ERROR);
      return;
    }

    int count = 0;
    foreach(const QHostAddress &address, QNetworkInterface::allAddresses())
    {
      if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
      {
        if (count == 0) { emit actPrint(tr("\t - local IP address: %1").arg(address.toString())); }
        else
        {
          //qDebug("UDP WARN: multiple network addresses detected, picking first one");
          emit actPrint("Multiple network addresses detected, picking first one", MessageType::WARNING);
        }
      }
    }

    if (hasBroadcast)
    {
      QString _baddress = interface->addressEntries().at(addressSelector).broadcast().toString();
      broadcastAddress = new QHostAddress(_baddress);
      // qDebug()<<"UDP: Set broadcast address"<< _baddress;
      emit actPrint(tr("Set broadcast address: %1").arg(_baddress));
    }
    else
    {
      // qDebug("UDP: WARNING: network seems to have no Broadcast support, setting default one");
      broadcastAddress = new QHostAddress("239.0.0.1");
    }
    emit actPrint(tr("Server starting, listening at port: %1").arg(QString::number(port)));

    // qDebug() << "UDP: Set broadcast address" << mSocket->state();
    if (mSocket->state() == 4)
    {
      //   qDebug("UDPServer::initNetwork done...");
      state = State::CONNECTED;
      emit actPrint("Network init done...\n", MessageType::STATUS);
      emit actInitDone();
    }
    else
    {
      //  qDebug() << "UDP: There is a problem starting the server on port" << port;
      emit actPrint(tr("UDP: There is a problem starting the server on port %1").arg(QString::number(port)), MessageType::WARNING);
      return;
    }

  }

  void UDPServer::killNetwork()
  {
    // qDebug("UDPServer::killNetwork");
    disconnect(mSocket, SIGNAL(readyRead()), this, SLOT(onDatagramRecived()));

    mSocket->disconnectFromHost(); 
    
    state = State::OFF;
    emit actPrint("Network kill done...\n", MessageType::STATUS);
    emit actKillDone();
  }

  bool UDPServer::isConnectedToNet()
  {
    // qDebug("UDPServer::isConnectedToNet");
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool result = false;

    for (int i = 0; i < ifaces.count(); i++) {
      QNetworkInterface iface = ifaces.at(i);
      if (iface.flags().testFlag(QNetworkInterface::IsUp) && !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
      {
        // qDebug() << "UDP: got interface:" << iface.name() << "with mac:" << iface.hardwareAddress();

        for (int j = 0; j < iface.addressEntries().count(); j++)
        {
          QString _broadcast = iface.addressEntries().at(j).broadcast().toString();
          // qDebug() << "UDP: addr.:" << iface.addressEntries().at(j).ip().toString();
          // qDebug() << "UDP: netmask:" << iface.addressEntries().at(j).netmask().toString();
          //  qDebug() << "UDP: broadcast:" << _broadcast;

          if (result == false) {
            result = true;
            interface = new QNetworkInterface(iface);
          }

          if (_broadcast.size() > 0) {
            hasBroadcast = true;
            addressSelector = j;
          }
        }

        if (!result)
        {
          //qDebug() << "UDP: interface seems to have no address";
          emit actPrint("Network interface seems to have no address", MessageType::WARNING);
        }
      }
    }

    return result;
  }

  void UDPServer::onSendData(QByteArray objectsData)
  {
    // QString dataMsg = tr("%1||%2").arg(userName, code);

    //  QByteArray datagram(dataMsg.toStdString().c_str());


    mSocket->writeDatagram(objectsData.data(), objectsData.size(), *broadcastAddress, port);
  }

  void UDPServer::onDatagramRecived()
  {
    QByteArray datagram;
    QHostAddress sender;
    quint16 senderPort;

    while (mSocket->hasPendingDatagrams())
    {
      datagram.resize(mSocket->pendingDatagramSize());
      mSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    }

    emit actNetDataRecived(datagram);
  }

  UDPServer::~UDPServer()  { };
}
