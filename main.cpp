//#include <QApplication>
#include <QtGui>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>

#include <QQuickStyle>

#include "abstractBase.h"
#include "userSettings.h"
#include "screenglobal.h"

#include "wslib/treemodel.h"
#include "wslib/systemtray.h"
#include "wslib/popup.h"

#include "wslib/loghandler.h"

#include "aimnotifications.h"
#include "runningaims.h"

#include "usertoken.h"

#include "wslib/clipboard.h"

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

    //QPixmap pixmap("C:/dev/projects/aim/aim/wsplash.png");
    //QSplashScreen splash(pixmap);
    //splash.show();
    //splash.showMessage("Loading libraries");

    LocalSqlBase localBase(&app);
    localBase.createTablesIfNeeded();



    UserSettings userSettings(&app);

    if (qgetenv("QT_QUICK_CONTROLS_1_STYLE").isEmpty()) {
#ifdef QT_STATIC
        // Need a full path to find the style when built statically
        qputenv("QT_QUICK_CONTROLS_1_STYLE", ":/ExtrasImports/QtQuick/Controls/Styles/Flat");
#else
        qputenv("QT_QUICK_CONTROLS_1_STYLE", "Flat");
#endif
    }

    QQuickStyle::setStyle("Material"); //Universal  + Material

    //splash.showMessage("Loading libraries: local base loaded");
    //SO in settings: Default, Un Dark or Light, Material Dark or Light + main colors
    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();
    //TreeModel aimsTree(&app);  - moved into localBase
    //TreeModel tagsTree(&app);

    context->setContextProperty("localBase",&localBase);
    context->setContextProperty("userSettings",&userSettings);
    context->setContextProperty("screenGlobal",&screen);

    context->setContextProperty("treeModel",&localBase.aimsTree);
    context->setContextProperty("tagTree",&localBase.tagsTree);


    //SYSTEM tray actions
    SystemTray tray(&app);


    tray.switchIcon("Aim");

    context->setContextProperty("systemTray",&tray);

    //Notifications
    PopUp popUp;
    context->setContextProperty("popUpItem",&popUp);

    AimNotifications notifications(&localBase,&popUp,&app);
    notifications.startWatchDog(30);
    engine.rootContext()->setContextProperty("androidNotify",&notifications.androidNotification);

    //Running aims
    RunningAims runningAims(&localBase,&app);
    context->setContextProperty("runningAims",&runningAims);

    //Log handler
    LogHandler logHandler;
    logHandler.setFilename("log.txt");

    //User token
    UserToken token;
    engine.rootContext()->setContextProperty("userToken",&token);

    Clipboard clip;
    engine.rootContext()->setContextProperty("clipboard",&clip);

    qmlRegisterType<ConsoleLogQML>("wwav.es.app",1,0,"ConsoleLog");


    engine.load(QUrl(QStringLiteral("qrc:/main.qml"))); //of course qrc:/main.qml

    //splash.hide();
    //popUp.setPopupText("hello","red",10);

    #ifdef Q_OS_ANDROID
        //try to bind
        ///https://doc-snapshots.qt.io/qt5-5.12/qandroidservice.html
    #endif

    return app.exec();
}


