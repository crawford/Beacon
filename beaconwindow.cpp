#include "beaconwindow.h"
#include "ui_beaconwindow.h"

BeaconWindow::BeaconWindow(QWidget *parent) : QDialog(parent), ui(new Ui::BeaconWindow) {
	ui->setupUi(this);
}

BeaconWindow::~BeaconWindow() {
	delete ui;
}
