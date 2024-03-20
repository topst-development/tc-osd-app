# -------------------------------------------------
# Project created by QtCreator 2012-11-05T23:21:28
# -------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += qml quick
    QT.gui.CONFIG -= opengl
}

TEMPLATE = app
DESTDIR = bin
CONFIG += release
DEFINES -= UNICODE \
    _UNICODE

CONFIG(debug, debug|release) {
    OBJECTS_DIR = build/debug/obj
    MOC_DIR = build/debug/moc
    UI_HEADERS_DIR = build/debug/ui_header
	TARGET = OSDApp
    INCLUDEPATH += build/debug/ui_header
}
else {
    OBJECTS_DIR = build/release/obj
    MOC_DIR = build/release/moc
    UI_HEADERS_DIR = build/release/ui_header
	TARGET = OSDApp
    INCLUDEPATH += build/release/ui_header
}

INVITE_LIBS = dbus-1 TcUtils

CONFIG += link_pkgconfig
PKGCONFIG += $$INVITE_LIBS

INCLUDEPATH += ./include

SOURCES = src/DBusMsgDefNames.c \
    src/main.cpp \
	src/OSDApp.cpp \
	src/OSDStatusWidget.cpp \
	src/OSDDBusManager.cpp \
	src/OSDIFManager.cpp

HEADERS = include/DBusMsgDef.h \
	include/MediaPlayerType.h \
	include/OSDApp.h \
	include/OSDStatusWidget.h \
	include/OSDDBusManager.h \
	include/OSDIFManager.h

LIBS += -lpthread -lrt

RESOURCES += res/OSDApp.qrc

target.path = /usr/bin

INSTALLS += target
