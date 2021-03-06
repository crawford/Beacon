#include "peer.h"

static int IDcount = 0;

Peer::Peer(QString newName, QTcpSocket *newSocket) {
	name = newName;
	socket = newSocket;

	id = IDcount++;

	connect(socket, SIGNAL(readyRead()), this, SLOT(handleMessage()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(handleDisconnect()));
}

QString Peer::getName() {
	return name;
}

void Peer::setName(QString newName) {
	name = name;
}

QTcpSocket* Peer::getSocket() {
	return socket;
}

void Peer::handleMessage() {
	emit gotMessage(this);
}

void Peer::handleDisconnect() {
	emit disconnected(this);
}

QByteArray Peer::readMessage() {
	return socket->readAll();
}

int Peer::getID() {
	return id;
}
