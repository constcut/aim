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
        //Better make in another thread
        //+1 more thread could be done
        //for server logging while connected to aim srv

        QString closedLine = anotherLine + QString("\n\n");

        QFile logFile(logFileName);
        logFile.open(QIODevice::Append);
        logFile.write(closedLine.toLocal8Bit());
        logFile.close();
    }
    ///ALSO there can be network logging

    logLines << anotherLine;
    if (logLines.size() > 200) //make configurable
        logLines.removeAt(0);

    //QByteArray lineBytes = anotherLine.toLocal8Bit();
    //fprintf(stdout,"%s",lineBytes.constData()); //dublicate to stdout
}


//==============QML log components=========================


void ConsoleLogQML::paint(QPainter* painter){
  QStringList log = LogHandler::instance->getLines();

  //can make a colored pen

  int counter = 0;
  for (int i = log.size()-1; i >= 0; i--)
  {
     ++counter;
      if (counter > 100) break; //maybe 2000, but return only 200 from them

     QString line =  log[i];
     painter->drawText(20,counter*10,line);
  }
}
