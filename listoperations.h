#ifndef LISTOPERATIONS_H
#define LISTOPERATIONS_H

#include <QStringList>
#include <QVariantList>

QStringList createListByField(QVariantList source, int fieldIndex);

QStringList createListByFieldBackwards(QVariantList source, int fieldIndex, int secondField=-1);

void compareLists(const QVariantList &firstList, const QVariantList &secondList,
                  QVariantList &toInsert, QVariantList &toEdit, QVariantList &toDelete);

#endif // LISTOPERATIONS_H
