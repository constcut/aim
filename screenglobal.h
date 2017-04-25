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

    double getDPI();

    //FOR THE calendar - 640/100
    double getCalendarValue() { return getScreenWidth()*1.0/8.0; }

    //elementHeight - in add Aim 800/45
    //widthOffset - 800/20
    //yOffset - 800/50

        //button width one is just 50

    //popup - x y 100 100 width 220 height 450  (next 230 560)


    //contentYmove to search -100 from 800 and 300

    //someitem also in aimView  has 110 from 800

    //highlight bar 200 from 640?? height 50 from 800 ??


protected:

    QScreen *screen;  //maybe once cover by signals
};

#endif // SCREENGLOBAL_H
