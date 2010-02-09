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
	enum MessageType {ONLINE, NAME, MESSAGE};
	void addPeer(QString name, QTcpSocket *socket);

private slots:
	void readBroadcast();
	void handleNewConnection();
	void handleMessage(Peer *peer);
	void handleDisconnect(Peer *peer);
};

#endif // CONNECTIONMANAGER_H
