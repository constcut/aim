#include "systemtray.h"

#include <QMenu>
#include <QSystemTrayIcon>

void SystemTray::switchIcon(QString iconName)
{
    if (iconName == "PPC")
    {
         trayIcon->setIcon(QIcon(":/icons/mainIcon.ico"));
         trayIcon->setToolTip("Контроль производства:" "\n"
                              "Нажмите на иконку, чтобы развернуть скрытое окно");
    }
    if (iconName == "birth")
    {
        trayIcon->setIcon(QIcon(":/icons/birthIcon.ico"));
        trayIcon->setToolTip("Дни рождения предприятия:" "\n"
                             "Нажмите на иконку, чтобы развернуть скрытое окно");
    }
    if (iconName == "IDM")
    {
        trayIcon->setIcon(QIcon(":/icons/mainIcon.ico"));
        trayIcon->setToolTip("IDM:контроль пользователей" "\n"
                             "Нажмите на иконку, чтобы развернуть скрытое окно");
        hideIconTray();
    }
}


SystemTray::SystemTray(QObject *parent) : QObject(parent), trayIcon(0)
{
    QMenu *trayIconMenu = new QMenu();

    QAction * viewWindow = new QAction("Развернуть окно", this);
    QAction * quitAction = new QAction("Выход", this); //trUtf was here

    connect(viewWindow, &QAction::triggered, this, &SystemTray::signalShow);
    connect(quitAction, &QAction::triggered, this, &SystemTray::signalQuit);

    trayIconMenu->addAction(viewWindow);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon();
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/icons/mainIcon.ico")); //must ensure
    trayIcon->show();
    trayIcon->setToolTip("Приложение НИТИ" "\n"
                         "Дождитесь загрузку приложения");

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
