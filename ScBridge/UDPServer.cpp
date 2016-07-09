#include "UDPServer.h"

#define DEBUG 1
#define PORT 10000

namespace SupercolliderBridge {

  UDPServer::UDPServer(QObject *parent, ScBridge* bridge) :
    QObject(parent),
    mBridge(bridge)
  {
    connect(this, SIGNAL(actPrintStatus(QString)), parent, SLOT(onMsgStatus(QString)));
    connect(this, SIGNAL(actPrintMsg(QString)), parent, SLOT(onMsgNormal(QString)));


  }

  bool UDPServer::isConnectedToNet() {
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool result = false;

    for (int i = 0; i < ifaces.count(); i++) {
      QNetworkInterface iface = ifaces.at(i);
      if (iface.flags().testFlag(QNetworkInterface::IsUp) &&
        !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
      {
        qDebug() << "UDP: got interface:" << iface.name() << "with mac:" << iface.hardwareAddress();

        for (int j = 0; j < iface.addressEntries().count(); j++)
        {

          QString _broadcast = iface.addressEntries().at(j).broadcast().toString();
          qDebug() << "UDP: addr.:" << iface.addressEntries().at(j).ip().toString();
          qDebug() << "UDP: netmask:" << iface.addressEntries().at(j).netmask().toString();
          qDebug() << "UDP: broadcast:" << _broadcast;

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

  QString UDPServer::getUsername() {

    QString name = qgetenv("USER");
    if (name.isEmpty())
      name = qgetenv("USERNAME");
    qDebug() << "UDP: username: " << name;
    return name;
  }

  int UDPServer::initSocket(QString name)
  {
    port = PORT;

    if (!name.isEmpty()) { username = name; }
    else { username = this->getUsername(); }

    emit actPrintStatus("UDP Server init...\n");
    emit actPrintMsg(tr("UDP: my username: %1\n").arg(username));

    emit actPrintMsg("UDP: Checking network setup...\n");

    if (isConnectedToNet()) {
      emit actPrintMsg("UDP: Network link is ESTABLISHED\n");
    }
    else {
      emit actPrintMsg("UDP: ERROR! you are not connected to any network.\n");
      return 1;
    }

    int count = 0;
    foreach(const QHostAddress &address, QNetworkInterface::allAddresses()) {
      if (address.protocol() == QAbstractSocket::IPv4Protocol &&
        address != QHostAddress(QHostAddress::LocalHost)) {
        if (count == 0) {
          emit actPrintMsg(
            tr("UDP: local IP address: %1\n").arg(address.toString()));
        }
        else {
          emit actPrintMsg(
            "UDP WARN: multiple network addresses detected, picking first one\n");
        }
      }
    }

    if (hasBroadcast) {
      QString _baddress =
      interface->addressEntries().at(addressSelector).broadcast().toString();
      broadcastAddress = new QHostAddress(_baddress);
      emit actPrintMsg(tr("UDP: OK broadcast addr.: %1\n").arg(_baddress));
    }
    else {
      emit actPrintMsg("UDP: WARNING: network seems to have no Broadcast "
        "support, setting default one.\n");
      broadcastAddress = new QHostAddress("239.0.0.1");
    }

    emit actPrintMsg(tr("UDP: Server starting, listening at port: %1\n")
      .arg(QString::number(port)));

    socket = new QUdpSocket(this);
    host = new QHostInfo();
    socket->bind(QHostAddress::Any, port);

    if (socket->state() == 4) {
      emit actPrintStatus("UDP: Server init done...\n");
    }
    else {
      emit actPrintMsg(
        tr("UDP: There is a problem starting the server on port %1\n")
        .arg(QString::number(port)));
      return 1;
    }

    //emit actPrintMsg("///////////////////////////////////////////////////////////////////\n\n");

    connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    this->sendCode(tr("\"Hi all my name is %1\n\".postln;").arg(username));
    return 0;
  }

  void UDPServer::send(const char *input) {

    QByteArray datagram(input);
    socket->writeDatagram(datagram.data(), datagram.size(), *broadcastAddress,
      port);
  }

  void UDPServer::sendCode(QString code)
  {
    QString dataMsg = tr("%1||%2").arg(username, code);

    QByteArray datagram(dataMsg.toStdString().c_str());
    socket->writeDatagram(datagram.data(), datagram.size(), *broadcastAddress, port);
  }

  void UDPServer::readPendingDatagrams()
  {
    QByteArray datagram;
    QHostAddress sender;
    quint16 senderPort;

    while (socket->hasPendingDatagrams())
    {
      datagram.resize(socket->pendingDatagramSize());
      socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    }

    QString dataMsg = QString::fromUtf8(datagram);
    QStringList data = dataMsg.split("||");
    QString senderName = data[0];
    QString recivedCode = data[1];

    if (senderName != username)
    {
      //QString postSender = sender.;
      emit actPrintMsg(tr("udpMsg [from %1] -> %2").arg(senderName, recivedCode));
      this->processDatagram(recivedCode);
    }

  } // end void

  void UDPServer::processDatagram(QString code)
  {
    mBridge->evaluate(code, true);
  }

  UDPServer::~UDPServer(){};
  // end class

} // end namespace
