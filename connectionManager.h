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
	void sendOnlineBroadcast();
	void sendOfflineBroadcast();

signals:
	void changedPeers();

private:
	QUdpSocket *broadcastSocket;
	QTcpServer *server;
	QList<Peer*> *peers;
	QString name;
	QList<QHostAddress> broadcastAddresses;
	QList<QHostAddress> localAddresses;

	void updateAddresses();
	enum MessageType {ONLINE, NAME, OFFLINE};

public slots:
	void readBroadcast();
	void handleNewConnection();

};

#endif // CONNECTIONMANAGER_H
