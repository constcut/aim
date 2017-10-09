#include "screenglobal.h"


#include <QGuiApplication>

ScreenGlobal::ScreenGlobal(QObject *parent):QObject(parent),screen(QGuiApplication::primaryScreen())
{
    //WEELL actually it just should work different on different platforms
}


int ScreenGlobal::getScreenWidth()
{
#if WIN32
    return 480;
#endif
    return screen->geometry().width();
}

int ScreenGlobal::getScreenHeight()
{
#if WIN32
    return 800;
#endif
    return screen->geometry().height();
}

double ScreenGlobal::adaptXSize(int xValue)
{
#if WIN32
    return xValue;
#endif
    return xValue/480.0*screen->geometry().width();
}

double ScreenGlobal::adaptYSize(int yValue)
{
#if WIN32
    return yValue;
#endif
    return yValue/800.0*screen->geometry().height();
}

double ScreenGlobal::getDPI()
{
    return screen->logicalDotsPerInch();
}
