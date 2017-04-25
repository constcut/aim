#TEMPLATE = app

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

QT += sql qml quick quickcontrols2

android {
    QT += androidextras
}

SOURCES += main.cpp \
    abstractBase.cpp \
    userSettings.cpp \
    screenglobal.cpp

RESOURCES += qml.qrc \
    icons.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    general_notes \
    android-sources/AndroidManifest.xml \
    android-sources/src/at/wavespl/apps/aim/NotificationClient.java

HEADERS += \
    abstractBase.h \
    userSettings.h \
    screenglobal.h

#dunno what is that
INSTALLS += target
