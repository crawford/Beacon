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

			addPeer(list.at(1), socket);
		}
	}
}

void ConnectionManager::addPeer(QString name, QTcpSocket *socket) {
	Peer *peer = new Peer(name, socket);
		connect(peer, SIGNAL(gotMessage(Peer*)), this, SLOT(handleMessage(Peer*)));
		connect(peer, SIGNAL(disconnected(Peer*)), this, SLOT(handleDisconnect(Peer*)));
		peers->append(peer);

		emit changedPeers();
}

QList<Peer*> *ConnectionManager::getPeers() {
	return peers;
}

void ConnectionManager::handleNewConnection() {
	while(server->hasPendingConnections()) {
		QTcpSocket *socket = server->nextPendingConnection();

		QByteArray message(1, NAME);
		socket->write(message);
		if(socket->waitForReadyRead(3000)) {
			QByteArray message = socket->readAll();
			QList<QByteArray> list = message.split('.');
			if(list.size() == 2) {
				if(list.at(0).at(0) == NAME) {
					addPeer(list.at(1), socket);
					continue;
				}
			}
		}

		socket->disconnectFromHost();
	}
}

void ConnectionManager::handleMessage(Peer *peer) {
	QList<QByteArray> list = peer->readMessage().split('.');
	if(list.at(0).at(0) == NAME) {
		if(list.size() == 1) {
			QByteArray response(1, NAME);
			response += '.';
			response += name;

			peer->getSocket()->write(response);
		}
	} else {
		QMessageBox::information(0, "Got Message", QString(list.at(0)));
	}
}

void ConnectionManager::handleDisconnect(Peer *peer) {
	peers->removeOne(peer);
	emit changedPeers();
}
