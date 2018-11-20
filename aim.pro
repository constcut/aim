#TEMPLATE = app

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

QT += sql qml quick widgets quickcontrols2

android {
    QT += androidextras
    LIBS += -LC:/Qt592/5.9.2/android_armv7/qml/QtWebSockets
}

SOURCES += main.cpp \
    abstractBase.cpp \
    usersettings.cpp \
    screenglobal.cpp \
    wslib/treemodel.cpp \
    wslib/notifications.cpp \
    wslib/popup.cpp \
    wslib/systemtray.cpp \
    wslib/tabelserialize.cpp \
    aimnotifications.cpp \
    runningaims.cpp \
    wslib/loghandler.cpp \
    usertoken.cpp

RESOURCES += qml.qrc \
    icons.qrc
#\
#    icons.qrc

jslib = $$PWD

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    general_notes \
    android-sources/AndroidManifest.xml \
    android-sources/src/at/wavespl/apps/aim/NotificationClient.java \
    TODO \
    qwebchannel.js

HEADERS += \
    abstractBase.h \
    userSettings.h \
    screenglobal.h \
    wslib/treemodel.h \
    wslib/notifications.h \
    wslib/popup.h \
    wslib/systemtray.h \
    wslib/tabelserialize.h \
    aimnotifications.h \
    runningaims.h \
    wslib/loghandler.h \
    usertoken.h

#dunno what is that
INSTALLS += target
