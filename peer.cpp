#include "peer.h"

Peer::Peer(QString newName, QTcpSocket *newSocket) {
	name = newName;
	socket = newSocket;

	connect(socket, SIGNAL(readyRead()), this, SLOT(handleMessage()));
}

QString Peer::getName() {
	return name;
}

QTcpSocket* Peer::getSocket() {
	return socket;
}

void Peer::handleMessage() {
	emit gotMessage(socket->readAll());
}
