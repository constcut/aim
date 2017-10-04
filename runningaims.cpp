#include "runningaims.h"

#include <QDebug>

RunningAims::RunningAims(LocalSqlBase *base,QObject *parent) : QObject(parent),localBase(base)
{

}

///ON REFACT THIS FUNCTION GOES TO LIST OPERATIONS AS COMPARE LISTS
//example on refact
QStringList createListByField2(QVariantList source, int fieldIndex)
{
    QStringList result;
    for (int i = 0; i < source.size(); ++i)
    {
        QStringList fields = source[i].toStringList();
        QString exactField = fields[fieldIndex];

        result << exactField;
    }
    return result;
}


QStringList RunningAims::getRunningAimsNames()
{
    return createListByField2(getRunningAims(),1);
}

QStringList RunningAims::getStoppedAimsNames()
{
    return createListByField2(getStoppedAims(),1);
}

void RunningAims::start(QString aimId)
{
    QStringList aimLine = localBase->getSingleAim(aimId);
    active << aimLine;
    activeRuns << QDateTime::currentDateTime();
    localBase->addActivity(aimLine[0],aimLine[1],"start");
}

bool RunningAims::isRunning(QString aimId)
{
    for (int i = 0; i < active.size(); ++i)
    {
        QStringList aimLine = active[i].toStringList();
        if (aimLine[0]==aimId)
            return true;
    }
    return false;
}

int RunningAims::getSecondsPassed(QString aimId)
{
    for (int i = 0; i < active.size(); ++i)
    {
        QStringList aimLine = active[i].toStringList();

        if (aimLine[0]==aimId)
        {
            QDateTime startMoment = activeRuns[i];
            QDateTime currentMoment = QDateTime::currentDateTime();

            return startMoment.secsTo(currentMoment);
        }
    }
    return 0;
}

void RunningAims::stop(QString aimId)
{
    QStringList aimLine = localBase->getSingleAim(aimId);

    for (int i = 0; i < active.size(); ++i)
    {
        QStringList activeLine = active[i].toStringList();

        if (activeLine[0] == aimId)
        {
            active.removeAt(i);
            QDateTime startMoment = activeRuns[i];
            activeRuns.removeAt(i);
            QDateTime endMoment = QDateTime::currentDateTime();

           // qDebug() << "Begin: " << startMoment << "; " << endMoment;

            stopped << aimLine; //or active line and don't touch base again

            int totalSeconds = startMoment.secsTo(endMoment);
            QString secondsString = QString::number(totalSeconds);

            localBase->addActivity(aimLine[0],aimLine[1],"stop",secondsString);
            break;
        }
    }
}

