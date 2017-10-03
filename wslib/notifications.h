#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <QObject>
#include <QTimer>

//#include "usersloader.h"
//#include "systemtray.h"

class Notifications : public QObject
{
    Q_OBJECT

public:
    Notifications(QObject *parent = 0);
    ~Notifications();

    void startWatchDog(int setInterval=60*5);

signals:
    void requestViewNotifications();

public slots:

    virtual void watchDogWoughf()=0; //Woughf-woughf!!!

protected:
    QString daysText(int days);

    //UsersLoader *users;
    //SystemTray *systemTray;

    QTimer watchDogTimer;
};



#endif // NOTIFICATIONS_H
