#include "usertoken.h"

#include <QNetworkInterface>

#include <QCryptographicHash>


#ifdef ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#endif


UserToken::UserToken(QObject *parent) : QObject(parent)
{
    prepareToken();
}


void UserToken::prepareToken()
{
    QString result;

    QCryptographicHash crypt(QCryptographicHash::Md5);

    //qDebug() << QNetworkInterface::allInterfaces();

    bool oneExtra = false;

    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
        {

        /*
         *  WifiManager manager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
            WifiInfo info = manager.getConnectionInfo();
            String address = info.getMacAddress();*/

            QString macAddress = interface.hardwareAddress();


            if (macAddress == "00:00:00:00:00:00:00:E0")
                continue;

            if (macAddress.size() == 0)
            {
#ifdef ANDROID

                if (oneExtra == false)
                {
                    QAndroidJniObject javaResult = QAndroidJniObject::callStaticObjectMethod<jstring>("at/wavespl/apps/aim/NotificationClient",
                                                                                             "getMacAddr");
                    qDebug() << javaResult.toString() <<" from android!";
                    macAddress = javaResult.toString();

                    oneExtra = true;
                }
                else
                    continue;
#else
                continue;  
#endif
            }

           qDebug() << "Interface:"+macAddress +"\n";
           macAddress.replace(":","");
            QByteArray macBytes = macAddress.toLocal8Bit();



            crypt.reset();
            crypt.addData(macBytes);

            QString MD5Summ = QString(crypt.result().toHex());

            result += MD5Summ;



            qDebug() << MD5Summ << " : md5";
        }

    token = result;
    //this is first developer version of token
    //actually we can simple add 7 in cycle like (x+7)%256
    //to make hard to recognize symbols, and alsowe can mix them all
    //but there is no such need right now

    ///NOTE: also maybe can add info about OS, or screen - to system it
}
