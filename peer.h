#ifndef PEER_H
#define PEER_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

class Peer : public QObject {
	Q_OBJECT
public:
	Peer(QString newName, QTcpSocket *newSocket);
	QTcpSocket* getSocket();
	QString getName();
	void setName(QString newName);
	QByteArray readMessage();
	int getID();

signals:
	void gotMessage(Peer *peer);
	void disconnected(Peer *peer);

private:
	QTcpSocket *socket;
	QString name;
	int id;

private slots:
	void handleMessage();
	void handleDisconnect();
};

#endif // PEER_H
