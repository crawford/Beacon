#include "peer.h"

Peer::Peer(QString newName, QTcpSocket *newSocket) {
	name = newName;
	socket = newSocket;
}

QString Peer::getName() {
	return name;
}

QTcpSocket* Peer::getSocket() {
	return socket;
}
