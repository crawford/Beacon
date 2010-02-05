#ifndef PEER_H
#define PEER_H

#include <QTcpSocket>
#include <QString>

class Peer {
public:
	Peer(QString newName, QTcpSocket *newSocket);
	QTcpSocket* getSocket();
	QString getName();

private:
	QTcpSocket *socket;
	QString name;
};

#endif // PEER_H
