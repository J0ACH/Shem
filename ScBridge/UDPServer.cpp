#include "UDPServer.h"

namespace SupercolliderBridge {

  UDPServer::UDPServer(QObject *parent) :
    QObject(parent),
    mSocket(new QUdpSocket(this))
  {
    qDebug("UDPServer:: new...");
    port = 10000;
    mSocket->bind(QHostAddress::Any, port);

    connect(mSocket, SIGNAL(readyRead()), this, SLOT(onDatagramRecived()));
  }

  void UDPServer::initNetwork(QString name)
  {
    qDebug() << "UDPServer::initNetwork name:" << name;
    emit actPrint("Network init start...", MessageType::STATUS);

    userName = name;
    emit actPrint(tr("Username is set on %1").arg(userName));

    if (isConnectedToNet()) { emit actPrint("Network link is estabilished"); }
    else { qDebug("UDP: ERROR! you are not connected to any network.\n"); return; }

    int count = 0;
    foreach(const QHostAddress &address, QNetworkInterface::allAddresses())
    {
      if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
      {
        if (count == 0) { emit actPrint(tr("\t - local IP address: %1").arg(address.toString())); }
        else { qDebug("UDP WARN: multiple network addresses detected, picking first one"); }
      }
    }

    if (hasBroadcast)
    {
      QString _baddress = interface->addressEntries().at(addressSelector).broadcast().toString();
      broadcastAddress = new QHostAddress(_baddress);
      emit actPrint(tr("Set broadcast address: %1").arg(_baddress));
    }
    else
    {
      qDebug("UDP: WARNING: network seems to have no Broadcast support, setting default one");
      broadcastAddress = new QHostAddress("239.0.0.1");
    }
    emit actPrint(tr("Server starting, listening at port: %1").arg(QString::number(port)));

    if (mSocket->state() == 4)
    {
      emit actPrint("Network init done...\n", MessageType::STATUS);
      qDebug("UDPServer::initNetwork done...");
      emit actNetworkBooted();
    }
    else { qDebug() << "UDP: There is a problem starting the server on port" << port; return; }

    this->sendCode(tr("Hi all my name is %1").arg(userName));
  }

  bool UDPServer::isConnectedToNet()
  {
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
          qDebug() << "UDP: interface seems to have no address";
      }
    }

    return result;
  }

  void UDPServer::sendCode(QString code)
  {
    QString dataMsg = tr("%1||%2").arg(userName, code);

    QByteArray datagram(dataMsg.toStdString().c_str());
    mSocket->writeDatagram(datagram.data(), datagram.size(), *broadcastAddress, port);
  }

  void UDPServer::onSendData(Data data)
  {
    /*
    QString dataMsg = tr("%1||%2").arg(userName, code);

    QByteArray datagram(dataMsg.toStdString().c_str());
    mSocket->writeDatagram(datagram.data(), datagram.size(), *broadcastAddress, port);
    */
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

    QString dataMsg = QString::fromUtf8(datagram);
    QStringList data = dataMsg.split("||");
    QString senderName = data[0];
    QString recivedCode = data[1];

    if (senderName != userName)
    {
      emit actPrint(tr("udpMsg [from %1] -> %2").arg(senderName, recivedCode));
      this->processDatagram(recivedCode);
    }

  }


  void UDPServer::processDatagram(QString code)
  {
    //mBridge->evaluate(code, true);
  }

  UDPServer::~UDPServer(){};
}
