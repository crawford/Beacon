#ifndef PEER_H
#define PEER_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

class Peer : QObject {
	Q_OBJECT
public:
	Peer(QString newName, QTcpSocket *newSocket);
	QTcpSocket* getSocket();
	QString getName();

signals:
	QByteArray gotMessage();

private:
	QTcpSocket *socket;
	QString name;

slots:
	void handleMessage();
};

#endif // PEER_H
