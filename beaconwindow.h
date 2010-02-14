#ifndef BEACONWINDOW_H
#define BEACONWINDOW_H

#include "connectionManager.h"
#include <QDialog>
#include <QStandardItemModel>
#include <QSystemTrayIcon>

namespace Ui {
	class BeaconWindow;
}

class BeaconWindow : public QDialog {
	Q_OBJECT
public:
	BeaconWindow(QWidget *parent = 0);
	~BeaconWindow();

protected:

private:
	Ui::BeaconWindow *ui;
	QStandardItemModel *peers;
	ConnectionManager *manager;
	QSystemTrayIcon *tray;

private slots:
	void updatePeers();
	void on_btnSend_clicked();
	void on_lstPeers_clicked();
	void handleTrayEvent(QSystemTrayIcon::ActivationReason reason);
};

#endif // BEACONWINDOW_H
