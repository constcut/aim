TEMPLATE = app

QT += sql qml quick widgets

SOURCES += main.cpp \
    abstractBase.cpp

RESOURCES += qml.qrc \
    icons.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    general_notes

HEADERS += \
    abstractBase.h

#INSTALLS += target
