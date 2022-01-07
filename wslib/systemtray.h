#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QAction>
#include <QSystemTrayIcon>


class SystemTray : public QObject
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = 0);
    ~SystemTray();

    Q_INVOKABLE void showNotification(QString title, QString text, int seconds);
    Q_INVOKABLE void showWarning(QString title, QString text, int seconds);
    Q_INVOKABLE void showError(QString title, QString text, int seconds);

    //Q_INVOKABLE void showPopupMessage(QString text, QString color, int seconds); //later add seconds, + color
    ///MOVED to be sepparated
    ///
    void switchIcon(QString iconName);


signals:
    void signalIconActivated();
    void signalShow();
    void signalQuit();
    void signalAdd();


signals: //used for notifications and probably had to be updated one time
    void requestTrayActionFromCpp();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

public slots:
    void hideIconTray();

private:
    QSystemTrayIcon *trayIcon; //TODO no raw ptr

};

#endif // SYSTEMTRAY_H
