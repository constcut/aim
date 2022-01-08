#include "runningaims.h"

#include "listoperations.h"

#include <QDebug>

RunningAims::RunningAims(LocalSqlBase &base, QObject *parent) : QObject(parent), localBase(base)
{
}


QStringList RunningAims::getRunningAimsNames() {
    return createListByField(getRunningAims(), 1);
}


QStringList RunningAims::getStoppedAimsNames() {
    return createListByField(getStoppedAims(), 1);
}


void RunningAims::start(QString aimId) {
    QStringList aimLine = localBase.getSingleAim(aimId);
    active << aimLine;
    activeRuns << QDateTime::currentDateTime();
    localBase.addActivity(aimLine[0],aimLine[1],"start");
}


bool RunningAims::isRunning(QString aimId) {
    for (int i = 0; i < active.size(); ++i) {
        const QStringList aimLine = active[i].toStringList();
        if (aimLine[0] == aimId)
            return true;
    }
    return false;
}


int RunningAims::getSecondsPassed(QString aimId) {
    for (int i = 0; i < active.size(); ++i) {
        QStringList aimLine = active[i].toStringList();
        if (aimLine[0]==aimId) {
            const QDateTime startMoment = activeRuns[i];
            const QDateTime currentMoment = QDateTime::currentDateTime();
            return startMoment.secsTo(currentMoment);
        }
    }
    return 0;
}


void RunningAims::stop(QString aimId)
{
    QStringList aimLine = localBase.getSingleAim(aimId);
    for (int i = 0; i < active.size(); ++i) {
        const QStringList activeLine = active[i].toStringList();
        if (activeLine[0] == aimId) {
            active.removeAt(i);
            const QDateTime startMoment = activeRuns[i];
            activeRuns.removeAt(i);
            const QDateTime endMoment = QDateTime::currentDateTime();
            stopped << aimLine; //or active line and don't touch base again
            const int totalSeconds = startMoment.secsTo(endMoment);
            const QString secondsString = QString::number(totalSeconds);
            localBase.addActivity(aimLine[0], aimLine[1], "stop", secondsString);
            break;
        }
    }
}

