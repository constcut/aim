#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <QObject>

class LogHandler : public QObject
{
    Q_OBJECT
public:
    explicit LogHandler(QObject *parent = nullptr);

    static LogHandler *instance;

    void addLine(QString anotherLine);

    QtMessageHandler oldHandler;

protected:
    QStringList logLines;

    QString logFileName;



signals:

public slots:
};

#endif // LOGHANDLER_H
