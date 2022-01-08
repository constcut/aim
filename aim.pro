
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

QT += sql qml quick widgets quickcontrols2

android {
    QT += androidextras
}

SOURCES += main.cpp \
    aims/AimBase.cpp \
    aims/AimNotifications.cpp \
    aims/DoneActsChart.cpp \
    aims/ListOperations.cpp \
    aims/RunningAims.cpp \
    aims/ScreenGlobal.cpp \
    aims/UserSettings.cpp \
    aims/UserToken.cpp \
    apptools/treemodel.cpp \
    apptools/popup.cpp \
    apptools/systemtray.cpp \
    apptools/tabelserialize.cpp \
    apptools/loghandler.cpp

RESOURCES += qml.qrc \
    icons.qrc



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
    aims/AimBase.h \
    aims/AimNotifications.h \
    aims/DoneActsChart.h \
    aims/ListOperations.h \
    aims/RunningAims.h \
    aims/ScreenGlobal.h \
    aims/UserSettings.h \
    aims/UserToken.h \
    apptools/treemodel.h \
    apptools/notifications.h \
    apptools/popup.h \
    apptools/systemtray.h \
    apptools/tabelserialize.h \
    apptools/loghandler.h \
    apptools/clipboard.h


INSTALLS += target
