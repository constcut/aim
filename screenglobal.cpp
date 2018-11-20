#include "screenglobal.h"
#include <QGuiApplication>

ScreenGlobal::ScreenGlobal(QObject *parent):QObject(parent),screen(QGuiApplication::primaryScreen()){
    //WEELL actually it just should work different on different platforms
}

int ScreenGlobal::getScreenWidth(){
#ifdef Q_OS_ANDROID
    return screen->geometry().width();
#else
   return 480;
#endif
}

int ScreenGlobal::getScreenHeight(){
#ifdef Q_OS_ANDROID
    return screen->geometry().height();
#else
    return 800;
#endif
}

double ScreenGlobal::adaptXSize(int xValue){
#ifdef Q_OS_ANDROID
    return xValue/480.0*screen->geometry().width();
#else
    return xValue;
#endif
}

double ScreenGlobal::adaptYSize(int yValue){
#ifdef Q_OS_ANDROID
    return yValue/800.0*screen->geometry().height();
#else
    return yValue;
#endif
}

double ScreenGlobal::getDPI(){
    return screen->logicalDotsPerInch();
}
