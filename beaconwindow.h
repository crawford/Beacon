#ifndef BEACONWINDOW_H
#define BEACONWINDOW_H

#include <QDialog>
#include <QStandardItemModel>
#include "connectionManager.h"

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

private slots:
	void updatePeers();
};

#endif // BEACONWINDOW_H
