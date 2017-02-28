//#include <QApplication>
#include <QtGui>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "abstractBase.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    LocalSqlBase localBase(&app);
    localBase.createTablesIfNeeded();

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    context->setContextProperty("localBase",&localBase);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
