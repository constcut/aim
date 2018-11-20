#include "aimnotifications.h"
#include <QDebug>


#ifdef ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#endif

NotificationClient::NotificationClient(QObject *parent)
    : QObject(parent)
{
    connect(this, SIGNAL(notificationChanged()), this, SLOT(updateAndroidNotification()));
}

void NotificationClient::setNotification(const QString &notification)
{
    if (m_notification == notification)
        return;

    m_notification = notification;
    emit notificationChanged();
}

QString NotificationClient::notification() const
{
    return m_notification;
}

void NotificationClient::updateAndroidNotification()
{
    #ifdef ANDROID

    qDebug() << "Running android notification attempt";

    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(m_notification);
    QAndroidJniObject::callStaticMethod<void>("at/wavespl/apps/aim/NotificationClient",
                                       "notify",
                                       "(Ljava/lang/String;)V",
                                       javaNotification.object<jstring>());




    #endif
}

//EXAMPLE of android notifications

AimNotifications::AimNotifications(LocalSqlBase *base,PopUp *popUp,QObject *parent):QObject(parent),localBase(base),popUp(popUp)
{
}

AimNotifications::~AimNotifications()
{
}

void AimNotifications::startWatchDog(int setInterval)
{
    int interval = 1000*setInterval;
    connect(&watchDogTimer,SIGNAL(timeout()),this,SLOT(watchDogWoughf()));
    watchDogTimer.start(interval);
}

void AimNotifications::watchDogWoughf()
{
   //qDebug() << "Tick";

   QVariantList currentAims = localBase->getCurrentMomementAims();

   if (currentAims.size())
   {
       qDebug() << "There are aims now! "<<currentAims;

       QString messageText;

       if (currentAims.size() == 1)
       {
           QStringList aimLine = currentAims[0].toStringList();
           messageText = aimLine[1] + " starts now!";
       }
       else
           messageText = QString::number(currentAims.size()) + " aims start now!";

#ifdef ANDROID
        androidNotification.setNotification(messageText);
        //later show popup too, but it should have another shape
#else
       popUp->setPopupText(messageText,"gray",10);
       popUp->show();
#endif
   }
}


