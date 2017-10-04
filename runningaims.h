#ifndef RUNNINGAIMS_H
#define RUNNINGAIMS_H

#include <QObject>

#include <QVariantList>
#include <QMap>

#include <QDateTime>

#include "abstractBase.h"

class RunningAims : public QObject
{
    Q_OBJECT
public:

    //it could be sepparated later if needed but now more comfort of holding it together with database

    explicit RunningAims(LocalSqlBase *base,QObject *parent = 0);

    Q_INVOKABLE QVariantList getRunningAims() { return active; }
    Q_INVOKABLE QVariantList getStoppedAims() { return stopped; }

    Q_INVOKABLE void start(QString aimId);
    Q_INVOKABLE void stop(QString aimId);

    Q_INVOKABLE int getSecondsPassed(QString aimId);

    Q_INVOKABLE bool isRunning(QString aimId);

    Q_INVOKABLE QStringList getRunningAimsNames();
    Q_INVOKABLE QStringList getStoppedAimsNames();

protected:
    QVariantList active; //current running
    QVariantList stopped; //already done

    LocalSqlBase *localBase;

    QList<QDateTime> activeRuns; //Map<QString

signals:

public slots:
};

#endif // RUNNINGAIMS_H
