#ifndef POPUP_H
#define POPUP_H

#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

#include "popup.h"



class PopUp : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

    void setPopupOpacity(float opacity);
    float getPopupOpacity() const;

public:
    explicit PopUp( QWidget *parent = 0);

    void mouseDoubleClickEvent(QMouseEvent * event);


signals: //used for notifications and probably had to be updated one time
    void requestNotificationsViewFromCpp();


protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void setPopupText(const QString& text, const QString& color, const int seconds);
    void show();


private slots:
    void hideAnimation();
    void hide();

private:
    QLabel label;
    QGridLayout layout;
    QPropertyAnimation animation;
    float popupOpacity;
    QTimer *timer; //TODO no ptr!

    QColor currentColor;
    int secondsToStay;

};

#endif // POPUP_H
