#ifndef USERTOKEN_H
#define USERTOKEN_H

#include <QObject>

class UserToken : public QObject
{
    Q_OBJECT
public:
    explicit UserToken(QObject *parent = nullptr);

    Q_INVOKABLE QString getToken() { return token; }

protected:

    void prepareToken();

    QString token;

signals:

public slots:
};

#endif // USERTOKEN_H
