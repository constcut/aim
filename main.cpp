//#include <QApplication>
#include <QtGui>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QQuickStyle>

#include "abstractBase.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    LocalSqlBase localBase(&app);
    localBase.createTablesIfNeeded();

    if (qgetenv("QT_QUICK_CONTROLS_1_STYLE").isEmpty()) {
#ifdef QT_STATIC
        // Need a full path to find the style when built statically
        qputenv("QT_QUICK_CONTROLS_1_STYLE", ":/ExtrasImports/QtQuick/Controls/Styles/Flat");
#else
        qputenv("QT_QUICK_CONTROLS_1_STYLE", "Flat");
#endif
    }

    QQuickStyle::setStyle("Material"); //Universal  + Material

    //SO in settings: Default, Un Dark or Light, Material Dark or Light + main colors

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    context->setContextProperty("localBase",&localBase);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
