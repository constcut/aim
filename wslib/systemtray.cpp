#include "systemtray.h"

#include <QMenu>
#include <QSystemTrayIcon>

void SystemTray::switchIcon(QString iconName)
{
    if (iconName == "Aim")
    {
         trayIcon->setIcon(QIcon(":/icons/tray.png")); //aimIcon
         trayIcon->setToolTip("Wavesplat aim");
    }
    if (iconName == "Recipe")
    {
        trayIcon->setIcon(QIcon(":/icons/recipeIcon.ico"));
        trayIcon->setToolTip("Wavesplat recipe");
    }
    if (iconName == "Boook")
    {
        trayIcon->setIcon(QIcon(":/icons/boookIcon.ico"));
        trayIcon->setToolTip("Wavesplat boook");
        //hideIconTray();
    }
}


SystemTray::SystemTray(QObject *parent) : QObject(parent), trayIcon(0)
{
    QMenu *trayIconMenu = new QMenu();

    //MAY MISS SPECIAL FIELDS

    QAction * viewWindow = new QAction("Show", this);
    QAction * addAction = new QAction("Add", this);
   // QAction * startStopAction = new QAction("Start>", this);
    QAction * quitAction = new QAction("Quit", this); //trUtf was here

    connect(viewWindow, &QAction::triggered, this, &SystemTray::signalShow);
    connect(quitAction, &QAction::triggered, this, &SystemTray::signalQuit);
    connect(addAction, &QAction::triggered, this, &SystemTray::signalAdd);

    trayIconMenu->addAction(viewWindow);
    trayIconMenu->addAction(addAction);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon();
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/icons/mainIcon.ico")); //must ensure
    trayIcon->show();
    trayIcon->setToolTip("Wavesplat");

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

SystemTray::~SystemTray()
{
    if (trayIcon)
    {
        trayIcon->hide();
        delete trayIcon;
    }
}

void SystemTray::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){
    case QSystemTrayIcon::Trigger:
        emit signalIconActivated(); //on press tray icon
        break;
    default:
        break;
    }
}

void SystemTray::hideIconTray()
{
    trayIcon->hide();
}

//in future we can use different ways of activation: QSystemTrayIcon::MiddleClick, QSystemTrayIcon::DoubleClick

void SystemTray::showNotification(QString title, QString text, int seconds)
{
    trayIcon->showMessage(title,text,QSystemTrayIcon::Information,seconds*1000);
}

void SystemTray::showWarning(QString title, QString text, int seconds)
{
    trayIcon->showMessage(title,text,QSystemTrayIcon::Warning,seconds*1000);
}

void SystemTray::showError(QString title, QString text, int seconds)
{
    trayIcon->showMessage(title,text,QSystemTrayIcon::Critical,seconds*1000);
}

/*
void SystemTray::showPopupMessage(QString text, QString color, int seconds)
{
    popUp->setPopupText(text,color,seconds);
    popUp->show();
}
*/
