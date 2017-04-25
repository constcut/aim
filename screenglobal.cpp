#include "screenglobal.h"


#include <QGuiApplication>

ScreenGlobal::ScreenGlobal(QObject *parent):QObject(parent),screen(QGuiApplication::primaryScreen())
{
    //WEELL actually it just should work different on different platforms
}


int ScreenGlobal::getScreenWidth()
{
    return screen->geometry().width();
}

int ScreenGlobal::getScreenHeight()
{
    return screen->geometry().height();
}

double ScreenGlobal::getDPI()
{
    return screen->logicalDotsPerInch();
}
