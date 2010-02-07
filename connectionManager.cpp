#include "connectionManager.h"
#include "peer.h"

#include <QTcpServer>
#include <QNetworkInterface>
#include <QMessageBox>

#define BROADCAST_PORT 23493

ConnectionManager::ConnectionManager(QString newName) {
	name = newName;

	server = new QTcpServer(this);
	connect(server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
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

void ConnectionManager::sendOnlineBroadcast() {
	QByteArray datagram(1, ONLINE);
	datagram.append('.');
	datagram.append(name.toAscii());
	datagram.append('.');
	datagram.append(QByteArray::number(server->serverPort()));

	foreach(QHostAddress address, broadcastAddresses) {
		broadcastSocket->writeDatagram(datagram, address, BROADCAST_PORT);
	}
}

void ConnectionManager::sendOfflineBroadcast() {
	QByteArray datagram(1, OFFLINE);
	datagram.append('.');
	datagram.append(name.toAscii());
	datagram.append('.');
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

		QList<QByteArray> list = datagram.split('.');
		if(list.size() != 3)
			continue;

		if(list.at(0).at(0) == ONLINE) {
			senderServerPort = list.at(2).toInt();
			if (localAddresses.contains(senderIP) && senderServerPort == server->serverPort())
				continue;

			QTcpSocket *socket = new QTcpSocket(this);
			socket->connectToHost(senderIP, senderServerPort);
			if(!socket->waitForConnected(5000))
				continue;

			peers->append(new Peer(list.at(1), socket));

			emit changedPeers();
		} else if(list.at(0).at(0) == OFFLINE) {
			foreach(Peer *peer, *peers) {
				if(peer->getName() == list.at(1))
					peers->removeOne(peer);
			}

			emit changedPeers();
		}
	}
}

QList<Peer*> *ConnectionManager::getPeers() {
	return peers;
}

void ConnectionManager::handleNewConnection() {
	while(server->hasPendingConnections()) {
		QTcpSocket *socket = server->nextPendingConnection();
		peers->append(new Peer("Somebody", socket));
	}
	emit changedPeers();
}
