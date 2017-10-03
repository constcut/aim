#include "aimnotifications.h"
#include <QDebug>


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

        popUp->setPopupText(messageText,"gray",10);
        popUp->show();
   }
}


