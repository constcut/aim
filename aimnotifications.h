#ifndef AIMNOTIFICATIONS_H
#define AIMNOTIFICATIONS_H

#include <QObject>
#include <QTimer>

#include "abstractBase.h"
#include "wslib/popup.h"

//FOR A WHILE WE USE EXAMPLE

class NotificationClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString notification READ notification WRITE setNotification NOTIFY notificationChanged)
public:
    explicit NotificationClient(QObject *parent = 0);

    Q_INVOKABLE void setNotification(const QString &notification);
    QString notification() const;

signals:
    void notificationChanged();

private slots:
    void updateAndroidNotification();

private:
    QString m_notification;
};

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

 public:
    NotificationClient androidNotification;
};




#endif // AIMNOTIFICATIONS_H
