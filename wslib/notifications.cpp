#include "notifications.h"

#include <QDebug>
#include <QTime>

//UsersLoader* users, SystemTray *systemTray,
Notifications::Notifications(QObject *parent):QObject(parent) //,users(users),systemTray(systemTray)
{
}

Notifications::~Notifications()
{

}

void Notifications::startWatchDog(int setInterval)
{
    int interval = 1000*setInterval;
    connect(&watchDogTimer,SIGNAL(timeout()),this,SLOT(watchDogWoughf()));
    watchDogTimer.start(interval);
}


