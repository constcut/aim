#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <QObject>
#include <QList>
#include <QMap>

class UserSettings : public QObject
{
    Q_OBJECT
public:
    explicit UserSettings(QObject *parent = 0);


    Q_INVOKABLE QList<bool> getAddAimSettings();
    Q_INVOKABLE void setAddAimSettings(QList<bool> newSettings);

    ///helpable function to set up ui in roper good way

    Q_INVOKABLE QList<bool> getViewAimSettings();
    Q_INVOKABLE void setViewAimSettings(QList<bool> newSettings);

    //colors functions
    Q_INVOKABLE void setStyle(QString styleName, QString themeName); //default, material light, dark, univeral ..etc

    Q_INVOKABLE bool setColor(QString colorType, QString colorValue);
    Q_INVOKABLE QString getColor(QString colorType);

    Q_INVOKABLE quint8 getSortingOrderType(){return orderType;}
    Q_INVOKABLE void setSortingOrderType(quint8 newOrderType){orderType=newOrderType;}

    Q_INVOKABLE bool isDebugBuild(){
        #ifdef QT_DEBUG
            return true;
        #else
                return false;
        #endif
    }

protected: //best idea is make QMap <QString, QVariable> to store all the parameters, and easy expend them

    QMap<QString,bool> viewAimSettings;
    QMap<QString,bool> addAimSettings;

    QString currentStyle;
    QString currentTheme;
    QMap<QString,QString> colors;

    quint8 orderType;
};

#endif // USERSETTINGS_H
