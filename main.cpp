#include <QtGui/QApplication>
#include "beaconwindow.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	BeaconWindow w;
	w.show();
	return a.exec();
}
