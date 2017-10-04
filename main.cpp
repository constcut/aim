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

#include "aimnotifications.h"
#include "runningaims.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ScreenGlobal screen;

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

    //SO in settings: Default, Un Dark or Light, Material Dark or Light + main colors

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    TreeModel aimsTree(&app);
    TreeModel tagsTree(&app);

    localBase.fillTreeModelWithAims(&aimsTree);
    tagsTree.fillWithTagList(localBase.getAllTags());

    context->setContextProperty("localBase",&localBase);
    context->setContextProperty("userSettings",&userSettings);
    context->setContextProperty("screenGlobal22",&screen);

    context->setContextProperty("treeModel",&aimsTree);
    context->setContextProperty("tagTree",&tagsTree);

    //SYSTEM tray actions
    SystemTray tray(&app);
    tray.switchIcon("Aim");
    context->setContextProperty("systemTray",&tray);

    //Notifications
    PopUp popUp;
    AimNotifications notifications(&localBase,&popUp,&app);
    notifications.startWatchDog(30);
    context->setContextProperty("popUpItem",&popUp);

    //Running aims
    RunningAims runningAims(&localBase,&app);
    context->setContextProperty("runningAims",&runningAims);

    //DEBUG
    qDebug() << localBase.getActivityLog("1");
    qDebug() << localBase.getActivityLog("2");
    qDebug() << localBase.getActivityLog("3");
    qDebug() << localBase.getActivityLog("4");
    qDebug() << localBase.getActivityLog("5");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
