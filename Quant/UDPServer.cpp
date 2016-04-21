#include "UDPServer.h"

#define DEBUG 1
#define PORT 10000

namespace QuantIDE {

	UDPServer::UDPServer(QWidget *parent) : QWidget(parent)
	{
		setObjectName("UDPServer");
		objectPattern = QString::null;
		connect(parent, SIGNAL(actConfigDone()), this, SLOT(initSocket()));
		connect(this, SIGNAL(actPrintStatus(QString)), parent, SLOT(onMsgStatus(QString)));
		connect(this, SIGNAL(actPrintMsg(QString)), parent, SLOT(onMsgNormal(QString)));
		/*
		if(this->initSocket()==0){
		//print to graphical console?
		//this->println("UDP Server booted");
		}
		*/
	}
	/*
	void println(const char *input){
	emit println(input);
	}
	*/

	bool UDPServer::isConnectedToNet(){
		QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
		bool result = false;

		for (int i = 0; i < ifaces.count(); i++){
			QNetworkInterface iface = ifaces.at(i);
			if (iface.flags().testFlag(QNetworkInterface::IsUp)
				&& !iface.flags().testFlag(QNetworkInterface::IsLoopBack)){

#ifdef DEBUG
				qDebug() << "UDP: got interface:" << iface.name() << "with mac:" << iface.hardwareAddress();
#endif

				for (int j = 0; j < iface.addressEntries().count(); j++){
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

					if (_broadcast.size() > 0){
						hasBroadcast = true;
						addressSelector = j;
					}
				}

#ifdef DEBUG
				if (!result)
					qDebug() << "UDP: interface seems to have no address";
#endif
			}

		}

		return result;
	}

	int UDPServer::initSocket(){
		port = PORT;

		emit actPrintStatus("UDP Server init...");
		emit actPrintMsg("UDP: Checking network setup...");

		if (isConnectedToNet()) { emit actPrintMsg("UDP: Network link is ESTABLISHED"); }
		else {
			emit actPrintMsg("UDP: ERROR! you are not connected to any network.");
			return 1;
		}

		int count = 0;
		foreach(const QHostAddress &address, QNetworkInterface::allAddresses()) {
			if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
				if (count == 0) { 
					emit actPrintMsg(tr("UDP: local IP address: %1").arg(address.toString()));
				}
				else {
					emit actPrintMsg("UDP WARN: multiple network addresses detected, picking first one");
				}
			}
		}

		if (hasBroadcast){
			QString _baddress = interface->addressEntries().at(addressSelector).broadcast().toString();
			broadcastAddress = new QHostAddress(_baddress);
			emit actPrintMsg(tr("UDP: OK broadcast addr.: %1").arg(_baddress));
		}
		else{
			emit actPrintMsg("UDP: WARNING: network seems to have no Broadcast support, setting default one.");
			broadcastAddress = new QHostAddress("239.1.1.250");
		}

		emit actPrintMsg(tr("UDP: Server starting, listening at port: %1").arg(QString::number(port)));

		socket = new QUdpSocket(this);
		host = new QHostInfo();
		socket->bind(QHostAddress::Any, port);

		if (socket->state() == 4){
			emit actPrintStatus("UDP: Server is ON!");
		}
		else{
			emit actPrintMsg(tr("UDP: There is a problem starting the server on port %1").arg(QString::number(port)));
			return 1;
		}

		emit actPrintMsg("///////////////////////////////////////////////////////////////////");


		connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

		this->send("Hello there! -- test message");
		return 0;
	}

	void UDPServer::send(const char *input){

		QByteArray datagram(input);
		socket->writeDatagram(datagram.data(), datagram.size(),
			*broadcastAddress, port);
	}

	void UDPServer::readPendingDatagrams(){

		while (socket->hasPendingDatagrams()) {

			QByteArray datagram;
			datagram.resize(socket->pendingDatagramSize());

			QHostAddress sender;
			quint16 senderPort;

			socket->readDatagram(datagram.data(), datagram.size(), &sender,
				&senderPort);


			emit actPrintMsg("UDP: Got some data chef!");
			emit actPrintMsg(tr("UDP: sender: %1").arg(sender.toString()));
			emit actPrintMsg(tr("UDP: data: %1").arg(datagram.data()));


			processDatagram(datagram);
		} // end while
	}   // end void

	void UDPServer::processDatagram(QByteArray datagram){
	}

	UDPServer::~UDPServer() {};
	// end class

} // end namespace
