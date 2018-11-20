#ifndef SCREENGLOBAL_H
#define SCREENGLOBAL_H

#include <QObject>
#include <QScreen>

class ScreenGlobal: public QObject
{
    Q_OBJECT

public:
    ScreenGlobal(QObject *parent = 0);

    Q_INVOKABLE int getScreenWidth();
    Q_INVOKABLE int getScreenHeight();

    Q_INVOKABLE double getDPI();

    Q_INVOKABLE double adaptXSize(int xValue);
    Q_INVOKABLE double adaptYSize(int yValue);

    //double getCalendarValue() { return getScreenWidth()*1.0/8.0; }

protected:

    QScreen *screen;  //maybe once cover by signals
};

#endif // SCREENGLOBAL_H
