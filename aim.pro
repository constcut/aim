
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

QT += sql qml quick widgets quickcontrols2

android {
    QT += androidextras
}

SOURCES += main.cpp \
    abstractBase.cpp \
    usersettings.cpp \
    screenglobal.cpp \
    wslib/treemodel.cpp \
    wslib/popup.cpp \
    wslib/systemtray.cpp \
    wslib/tabelserialize.cpp \
    aimnotifications.cpp \
    runningaims.cpp \
    wslib/loghandler.cpp \
    usertoken.cpp \
    doneactschart.cpp

RESOURCES += qml.qrc \
    icons.qrc

jslib = $$PWD

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    android-sources/AndroidManifest.xml \
    android-sources/build.gradle \
    android-sources/gradle.properties \
    android-sources/gradle/wrapper/gradle-wrapper.jar \
    android-sources/gradle/wrapper/gradle-wrapper.properties \
    android-sources/gradlew \
    android-sources/gradlew.bat \
    android-sources/res/values/libs.xml \
    general_notes \
    android-sources/src/at/wavespl/apps/aim/NotificationClient.java \
    TODO \
    qwebchannel.js \
    android-sources/src/at/wavespl/apps/aim/NotificationService.java

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
    usertoken.h \
    wslib/clipboard.h \
    doneactschart.h

INSTALLS += target
