#ifndef DONEACTSCHART_H
#define DONEACTSCHART_H

#include <QQuickPaintedItem>
#include <QPainter>

struct DACline{
    int x1;
    int x2;
    QString aimId;
    QString aimName;
};


class DoneActsChartQML : public QQuickPaintedItem
{
    Q_OBJECT
public:
    DoneActsChartQML():hourWidth(40),aimHeight(40){}

    Q_INVOKABLE void setHourWidth(int newWidth) { hourWidth = newWidth; }
    Q_INVOKABLE void getTotalAims() { indexation.size(); }
    Q_INVOKABLE void setAimHeight(int newHeight) { aimHeight = newHeight; }

    void paint(QPainter* painter);

    Q_INVOKABLE void setSource(QVariantList actsList);
    Q_INVOKABLE int getFirstRecordX();

protected:

    QMap< QString, int > indexation;
    QList< DACline > lines;

    int hourWidth;
    int aimHeight;
};

#endif // DONEACTSCHART_H
