#ifndef TABELSERIALIZE_H
#define TABELSERIALIZE_H

#include <QVector>
#include <QStringList>


class TableSerialize
{
public:

    void addRow(QStringList oneRow);
    QStringList getRow(int index);

    int size();

    void fromString(QString source);
    void fromRawString(QString source);

    QString toString();
    QString toRawString();

protected:
    QVector<QStringList> tableRows;

};

#endif // TABELSERIALIZE_H
