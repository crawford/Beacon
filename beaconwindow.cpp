#include "beaconwindow.h"
#include "ui_beaconwindow.h"
#include <QStandardItem>
#include <QMessageBox>
#include <QInputDialog>
#include <QSettings>
#include <QMenu>

BeaconWindow::BeaconWindow(QWidget *parent) : QDialog(parent), ui(new Ui::BeaconWindow) {	
	//Load configuration settings
	QSettings settings("config.ini", QSettings::IniFormat, this);
	QString name = settings.value("Name", "").toString();
	while(name == "") {
		name = QInputDialog::getText(this, "User Name", "Enter your name", QLineEdit::Normal, "", 0,
					Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
		settings.setValue("Name", name);
		settings.sync();
	}

	//Setup model for peers
	peers = new QStandardItemModel(this);

	//Create connections to all peers
	manager = new ConnectionManager(name);
	connect(manager, SIGNAL(changedPeers()), this, SLOT(updatePeers()));
	manager->sendOnlineBroadcast();

	//Setup ui
	ui->setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
	ui->lstPeers->setModel(peers);

	//Create system tray
	tray = new QSystemTrayIcon(this);
	tray->setIcon(QIcon(":/Icons/megaphone.png"));

	QMenu *menu = new QMenu(this);
	menu->setDefaultAction(menu->addAction("Send Message...", this, SLOT(show())));
	menu->addAction("Quit", this, SLOT(close()));
	tray->setContextMenu(menu);
	connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(handleTrayEvent(QSystemTrayIcon::ActivationReason)));
	tray->show();
}

BeaconWindow::~BeaconWindow() {
	tray->hide();
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

	on_lstPeers_clicked();
}

void BeaconWindow::on_btnSend_clicked() {
	QModelIndexList selection = ui->lstPeers->selectionModel()->selectedRows();
	QList<Peer*> *peerLinks = manager->getPeers();

	foreach(QModelIndex index, selection) {
		for(int i = 0; i < peerLinks->size(); i++) {
			if(peers->itemFromIndex(index)->data().toInt() == peerLinks->at(i)->getID()) {
				peerLinks->at(i)->getSocket()->write(ui->txtMessage->toPlainText().toAscii());
				i = peerLinks->size();
			}
		}
	}
}

void BeaconWindow::on_lstPeers_clicked() {
	ui->btnSend->setEnabled(ui->lstPeers->selectionModel()->selectedIndexes().size() != 0);
}

void BeaconWindow::handleTrayEvent(QSystemTrayIcon::ActivationReason reason) {
	if(reason == QSystemTrayIcon::DoubleClick) {
		show();
	}
}
