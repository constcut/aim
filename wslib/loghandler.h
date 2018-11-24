#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <QObject>

#include <QQuickPaintedItem>
#include <QPainter>

class LogHandler : public QObject
{
    Q_OBJECT
public:
    explicit LogHandler(QObject *parent = nullptr);

    static LogHandler *instance;

    void addLine(QString anotherLine);

    QtMessageHandler oldHandler;

    QStringList getLines() { return logLines; }

    void setFilename(QString filename) { logFileName = filename; }

protected:
    QStringList logLines;

    QString logFileName;

//signals:
//public slots:
};

class ConsoleLogQML : public QQuickPaintedItem
{
    Q_OBJECT
  public:
    ConsoleLogQML() {}
    ~ConsoleLogQML() {}

    void paint(QPainter* painter);

    //must be autoupdated
};

//ITS NICE TO DUPLICATE QML ITEM WITH WIDGET
///AND BEST IS TO MAKE SPECIAL TEMPLATE FOR THAT

#endif // LOGHANDLER_H
