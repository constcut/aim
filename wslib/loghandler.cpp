#include "loghandler.h"

LogHandler* LogHandler::instance = 0;

void newLogMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString typeLetter=" ";

    if (type == QtDebugMsg)
        typeLetter = "D";
    else
    if (type == QtInfoMsg)
        typeLetter = "I";
    else
    if (type == QtWarningMsg)
        typeLetter = "W";
    else
    if (type == QtCriticalMsg)
        typeLetter = "C";
    else
    if (type == QtFatalMsg)
        typeLetter = "F";

    QString logLine =  QString("[%1][%2:%3:%4] %5")
            .arg(typeLetter).arg(context.file).arg(context.function).arg(context.line).arg(msg);

    LogHandler::instance->addLine(logLine);

    LogHandler::instance->oldHandler(type, context, logLine); //msg
}

LogHandler::LogHandler(QObject *parent) : QObject(parent)
{
    instance = this;
    oldHandler = qInstallMessageHandler(newLogMessageHandler);
}


void LogHandler::addLine(QString anotherLine)
{
    if (logFileName.isEmpty() == false)
    {
        //Save line to file
    }
    ///ALSO there can be network logging

    logLines << anotherLine;
    if (logLines.size() > 1000) //make configurable
        logLines.removeAt(0);

    QByteArray lineBytes = anotherLine.toLocal8Bit();

    //fprintf(stdout,"%s",lineBytes.constData()); //dublicate to stdout
}
