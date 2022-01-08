
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

QT += sql qml quick widgets quickcontrols2

android {
    QT += androidextras
}

SOURCES += main.cpp \
    aims/abstractBase.cpp \
    aims/aimnotifications.cpp \
    aims/listoperations.cpp \
    aims/usersettings.cpp \
    aims/screenglobal.cpp \
    apptools/treemodel.cpp \
    apptools/popup.cpp \
    apptools/systemtray.cpp \
    apptools/tabelserialize.cpp \
    aims/runningaims.cpp \
    apptools/loghandler.cpp \
    aims/usertoken.cpp \
    aims/doneactschart.cpp

RESOURCES += qml.qrc \
    icons.qrc

jslib = $$PWD

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
#include(deployment.pri)

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
    android-sources/src/at/wavespl/apps/aim/NotificationService.java

HEADERS += \
    aims/abstractBase.h \
    aims/listoperations.h \
    aims/userSettings.h \
    aims/screenglobal.h \
    apptools/treemodel.h \
    apptools/notifications.h \
    apptools/popup.h \
    apptools/systemtray.h \
    apptools/tabelserialize.h \
    aims/aimnotifications.h \
    aims/runningaims.h \
    apptools/loghandler.h \
    aims/usertoken.h \
    apptools/clipboard.h \
    aims/doneactschart.h

INSTALLS += target
