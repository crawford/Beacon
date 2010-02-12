#include "beaconwindow.h"
#include "ui_beaconwindow.h"
#include <QStandardItem>
#include <QMessageBox>

BeaconWindow::BeaconWindow(QWidget *parent) : QDialog(parent), ui(new Ui::BeaconWindow) {
	peers = new QStandardItemModel(this);
	peers->appendRow(new QStandardItem("Test1"));
	peers->appendRow(new QStandardItem("Test2"));

	manager = new ConnectionManager(qApp->arguments().at(1));
	connect(manager, SIGNAL(changedPeers()), this, SLOT(updatePeers()));
	manager->sendOnlineBroadcast();

	ui->setupUi(this);
	ui->lstPeers->setModel(peers);
}

BeaconWindow::~BeaconWindow() {
	delete ui;
}

void BeaconWindow::updatePeers() {
	peers->clear();

	QList<Peer*> *peerLinks = manager->getPeers();
	for(int i = 0; i < peerLinks->size(); i++) {
		QStandardItem *item = new QStandardItem(peerLinks->at(i)->getName());
		item->setData(peerLinks->at(i)->getID());
		peers->appendRow(item);
	}
}

void BeaconWindow::on_btnSend_clicked() {
	QModelIndexList selection = ui->lstPeers->selectionModel()->selectedRows();
	QList<Peer*> *peerLinks = manager->getPeers();

	foreach(QModelIndex item, selection) {
		for(int i = 0; i < peerLinks->size(); i++) {
			if(item.data().toInt() == peerLinks->at(i)->getID()) {
				peerLinks->at(i)->getSocket()->write(ui->txtMessage->toPlainText().toAscii());
				i = peerLinks->size();
			}
		}
	}
}
