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

equals(SYSTEMD, 1){
	DEFINES += USE_SYSTEMD
}

CONFIG(debug, debug|release) {
    OBJECTS_DIR = build/debug/obj
    MOC_DIR = build/debug/moc
    UI_HEADERS_DIR = build/debug/ui_header
    TARGET = LauncherD
    INCLUDEPATH += build/debug/ui_header
}
else {
    OBJECTS_DIR = build/release/obj
    MOC_DIR = build/release/moc
    UI_HEADERS_DIR = build/release/ui_header
    TARGET = Launcher
    INCLUDEPATH += build/release/ui_header
}

INVITE_LIBS = dbus-1 TcUtils
CONFIG += link_pkgconfig
PKGCONFIG += $$INVITE_LIBS
INCLUDEPATH += ./include


SOURCES += src/main.cpp \
    src/LauncherIFManager.cpp \
    src/Launcher.cpp \
    src/TCApplication.cpp \
    src/LauncherDBusManager.cpp \
	src/NowPlayMusic.cpp \
	src/NowPlayRadio.cpp \
	src/NowPlayCarplay.cpp \
	src/HomeScreen.cpp \
	src/StatusWidget.cpp \
	src/ImageProvider.cpp \
    src/DBusMsgDefNames.c

HEADERS += include/LauncherIFManager.h \
    include/Launcher.h \
    include/NowPlayMusic.h \
    include/NowPlayRadio.h \
    include/NowPlayCarplay.h \
    include/HomeScreen.h \
	include/MediaPlayerType.h \
	include/StatusWidget.h \
    include/TCApplication.h \
    include/LauncherDBusManager.h \
	include/ImageProvider.h \
    include/DBusMsgDef.h

LIBS += -lpthread
equals(SYSTEMD, 1){
	LIBS +=-lsystemd
}

RESOURCES += res/Launcher.qrc

target.path = /usr/bin
sources.files = config.xml
sources.path = /usr/share/tcc_launcher
INSTALLS += target \
    sources

