#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTcpServer>
#include "peer.h"

class ConnectionManager : public QObject {
	Q_OBJECT

public:
	ConnectionManager(QString newName);
	QList<Peer*> *getPeers();
	void sendBroadcast();

signals:
	void newPeer();

private:
	QUdpSocket *broadcastSocket;
	QTcpServer *server;
	QList<Peer*> *peers;
	QString name;
	QList<QHostAddress> broadcastAddresses;
	QList<QHostAddress> localAddresses;

	void updateAddresses();

public slots:
	void readBroadcast();
	void addPeer();

};

#endif // CONNECTIONMANAGER_H
