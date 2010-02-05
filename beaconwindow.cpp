#include "beaconwindow.h"
#include "ui_beaconwindow.h"
#include <QStandardItem>

BeaconWindow::BeaconWindow(QWidget *parent) : QDialog(parent), ui(new Ui::BeaconWindow) {
	peers = new QStandardItemModel(this);
	peers->appendRow(new QStandardItem("Test1"));
	peers->appendRow(new QStandardItem("Test2"));

	manager = new ConnectionManager("Remote Test");
	connect(manager, SIGNAL(newPeer()), this, SLOT(addPeer()));
	manager->sendBroadcast();

	ui->setupUi(this);
	ui->lstPeers->setModel(peers);
}

BeaconWindow::~BeaconWindow() {
	delete ui;
}

void BeaconWindow::addPeer() {
	peers->clear();

	QList<Peer*> *peerLinks = manager->getPeers();
	for(int i = 0; i < peerLinks->size(); i++) {
		peers->appendRow(new QStandardItem(peerLinks->at(i)->getName()));
	}
}
