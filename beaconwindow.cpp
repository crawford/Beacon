#include "beaconwindow.h"
#include "ui_beaconwindow.h"
#include <QStandardItem>

BeaconWindow::BeaconWindow(QWidget *parent) : QDialog(parent), ui(new Ui::BeaconWindow) {
	peers = new QStandardItemModel(this);
	peers->appendRow(new QStandardItem("Test1"));
	peers->appendRow(new QStandardItem("Test2"));

	ui->setupUi(this);
	ui->lstPeers->setModel(peers);
}

BeaconWindow::~BeaconWindow() {
	delete ui;
}
