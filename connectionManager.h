#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include "peer.h"

class ConnectionManager : QObject {
	Q_OBJECT

public:
	ConnectionManager();

signals:
	void newPeer();

private:
	QUdpSocket *broadcastSocket;
	QList<Peer> *peers;

public slots:
	void readBroadcast();

};

#endif // CONNECTIONMANAGER_H
