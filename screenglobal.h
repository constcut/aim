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

protected:

    QScreen *screen;  //TODO no raw ptrs
};

#endif // SCREENGLOBAL_H
