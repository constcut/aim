//#include <QApplication>
#include <QtGui>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>

#include <QQuickStyle>


#include "apptools/treemodel.h"
#include "apptools/systemtray.h"
#include "apptools/popup.h"
#include "apptools/loghandler.h"
#include "apptools/clipboard.h"
#include "aims/abstractBase.h"
#include "aims/userSettings.h"
#include "aims/screenglobal.h"
#include "aims/aimnotifications.h"
#include "aims/runningaims.h"
#include "aims/usertoken.h"
#include "aims/doneactschart.h"

#include <QDebug>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#include <QAndroidService>
#endif



int main(int argc, char *argv[])
{
    #ifdef Q_OS_ANDROID
    QAndroidService service(argc, argv);
    #endif
    QApplication app(argc, argv);

    ScreenGlobal screen;
    LocalSqlBase localBase(&app);
    localBase.createTablesIfNeeded();
    UserSettings userSettings(&app);
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    context->setContextProperty("localBase",&localBase);
    context->setContextProperty("userSettings",&userSettings);
    context->setContextProperty("screenGlobal",&screen);
    context->setContextProperty("treeModel",&localBase.aimsTree);
    context->setContextProperty("tagTree",&localBase.tagsTree);

    SystemTray tray(&app);
    context->setContextProperty("systemTray",&tray);
    PopUp popUp;
    context->setContextProperty("popUpItem",&popUp);

    AimNotifications notifications(localBase, popUp, &app);
    notifications.startWatchDog(30);
    engine.rootContext()->setContextProperty("androidNotify",&notifications.androidNotification);

    RunningAims runningAims(localBase, &app);
    context->setContextProperty("runningAims",&runningAims);

    LogHandler::getInst().setFilename("log.txt");

    UserToken token;
    engine.rootContext()->setContextProperty("userToken",&token);

    Clipboard clip;
    engine.rootContext()->setContextProperty("clipboard",&clip);

    qmlRegisterType<ConsoleLogQML>("wwav.es.app",1,0,"ConsoleLog");
    qmlRegisterType<DoneActsChartQML>("wwav.es.app",1,0,"DoneActsChart");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}


