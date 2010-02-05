#include <QTcpServer>
#include <QNetworkInterface>
#include "ConnectionManager.h"
#include "peer.h"
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
}

void ConnectionManager::sendBroadcast() {
	QByteArray datagram(name.toAscii());
	datagram.append('@');
	datagram.append(QByteArray::number(server->serverPort()));

	QHostAddress address = QNetworkInterface::allAddresses().at(0);
	broadcastSocket->writeDatagram(datagram, address, BROADCAST_PORT);
}

void ConnectionManager::readBroadcast() {
	while (broadcastSocket->hasPendingDatagrams()) {
		QHostAddress senderIp;
		quint16 senderPort;
		QByteArray datagram;
		int senderServerPort;

		datagram.resize(broadcastSocket->pendingDatagramSize());
		if (broadcastSocket->readDatagram(datagram.data(), datagram.size(), &senderIp, &senderPort) == -1)
			continue;

		QList<QByteArray> list = datagram.split('@');
		if (datagram.size() != 2)
			continue;

		senderServerPort = list.at(1).toInt();
		//if (isLocalHostAddress(senderIp) && senderServerPort == serverPort)
			continue;

		//Search to see if peer already exists

		QTcpSocket *socket = new QTcpSocket(this);
		socket->connectToHost(senderIp, senderServerPort);
		if(socket->waitForConnected(5000))
			continue;

		peers->append(new Peer(list.at(0), socket));

		emit newPeer();
	}
}

QList<Peer*> *ConnectionManager::getPeers() {
	return peers;
}

void ConnectionManager::addPeer() {

}
