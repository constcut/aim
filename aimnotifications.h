#ifndef AIMNOTIFICATIONS_H
#define AIMNOTIFICATIONS_H

#include <QObject>
#include <QTimer>

#include "abstractBase.h"
#include "wslib/popup.h"

//on refact inherit from notifications default
class AimNotifications : QObject
{
    Q_OBJECT

public:
    AimNotifications(LocalSqlBase *base,PopUp *popUp,QObject *parent = 0);
    ~AimNotifications();

    void startWatchDog(int setInterval=60); //1min

signals:
    void requestViewNotifications();

public slots:
    void watchDogWoughf(); //Woughf-woughf!!!

protected:
    QTimer watchDogTimer;
    LocalSqlBase *localBase;

    PopUp *popUp; //or maybe popup had to be registered to be used from qml

};

#endif // AIMNOTIFICATIONS_H
