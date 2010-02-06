#include "connectionManager.h"
#include "peer.h"

#include <QTcpServer>
#include <QNetworkInterface>

#define BROADCAST_PORT 23493

ConnectionManager::ConnectionManager(QString newName) {
	name = newName;

	server = new QTcpServer(this);
	connect(server, SIGNAL(newConnection()), this, SLOT(addPeer()));
	server->listen();

	peers = new QList<Peer*>();
	broadcastSocket = new QUdpSocket(this);
	broadcastSocket->bind(BROADCAST_PORT);

	connect(broadcastSocket, SIGNAL(readyRead()), this, SLOT(readBroadcast()));

	updateAddresses();
}

void ConnectionManager::updateAddresses()  {
	broadcastAddresses.clear();
	localAddresses.clear();

	foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
		foreach(QNetworkAddressEntry entry, interface.addressEntries()) {
			QHostAddress broadcast = entry.broadcast();
			if(!broadcast.isNull() && broadcast != QHostAddress::LocalHost && broadcast != QHostAddress::LocalHostIPv6) {
				broadcastAddresses.append(broadcast);
				localAddresses.append(entry.ip());
			}
		}
	}
}

void ConnectionManager::sendBroadcast() {
	QByteArray datagram(name.toAscii());
	datagram.append('@');
	datagram.append(QByteArray::number(server->serverPort()));

	foreach(QHostAddress address, broadcastAddresses) {
		broadcastSocket->writeDatagram(datagram, address, BROADCAST_PORT);
	}
}

void ConnectionManager::readBroadcast() {
	while (broadcastSocket->hasPendingDatagrams()) {
		QHostAddress senderIP;
		quint16 senderPort;
		QByteArray datagram;
		int senderServerPort;

		datagram.resize(broadcastSocket->pendingDatagramSize());
		if (broadcastSocket->readDatagram(datagram.data(), datagram.size(), &senderIP, &senderPort) == -1)
			continue;

		QList<QByteArray> list = datagram.split('@');
		if (list.size() != 2)
			continue;

		senderServerPort = list.at(1).toInt();
		if (localAddresses.contains(senderIP) && senderServerPort == server->serverPort())
			continue;

		QTcpSocket *socket = new QTcpSocket(this);
		socket->connectToHost(senderIP, senderServerPort);
		if(!socket->waitForConnected(5000))
			continue;

		peers->append(new Peer(list.at(0), socket));

		emit newPeer();
	}
}

QList<Peer*> *ConnectionManager::getPeers() {
	return peers;
}

void ConnectionManager::addPeer() {
	while(server->hasPendingConnections()) {
		QTcpSocket *socket = server->nextPendingConnection();
		peers->append(new Peer("Somebody", socket));
	}
	emit newPeer();
}
