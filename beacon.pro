# -------------------------------------------------
# Project created by QtCreator 2010-02-04T23:38:33
# -------------------------------------------------
QT += network
TARGET = beacon
TEMPLATE = app
SOURCES += main.cpp \
    beaconwindow.cpp \
    connectionManager.cpp \
    peer.cpp
HEADERS += beaconwindow.h \
    connectionManager.h \
    peer.h
FORMS += beaconwindow.ui
RESOURCES += icons.qrc
RC_FILE += beacon.rc
