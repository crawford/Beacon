#ifndef BEACONWINDOW_H
#define BEACONWINDOW_H

#include <QDialog>

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
};

#endif // BEACONWINDOW_H
