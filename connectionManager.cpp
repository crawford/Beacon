#include "ConnectionManager.h"
#define BROADCAST_PORT 23493

ConnectionManager::ConnectionManager() {
	peers = new QList<Peer>();
	broadcastSocket = new QUdpSocket(this);
	broadcastSocket->bind(BROADCAST_PORT);

	connect(broadcastSocket, SIGNAL(readyRead()), this, SLOT(readBroadcast()));
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

		peers->append(Peer(list.at(0), socket));

		emit newPeer();
	}
}
