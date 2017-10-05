#include "listoperations.h"

QStringList createListByField(QVariantList source, int fieldIndex)
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


bool areLinesSame(const QStringList &firstLine, const QStringList &secondLine)
{
    for (int i = 0; i < firstLine.size(); ++i) // firstLine.size()
    {
        QString firstString = firstLine[i].trimmed();
        QString secondString = secondLine[i].trimmed();

        if (firstString != secondString)
        {
            qDebug() << "Lines don't fit: "<<firstString<<" "<<secondString;

            return false;
        }
    }

    return true;
}

void compareLists(const QVariantList &firstList, const QVariantList &secondList,
                  QVariantList &toInsert, QVariantList &toEdit, QVariantList &toDelete)
{

    for (int i =0; i < firstList.size(); ++i)
    {
        bool wasFound = false;

        QStringList firstLine = firstList[i].toStringList();
        QString firstCode = firstLine[0];

        for (int j = 0; j < secondList.size(); ++j)
        {
            QStringList secondLine = secondList[j].toStringList();
            QString secondCode = secondLine[0];

            if (firstCode == secondCode)
            {
                wasFound = true;

                if (areLinesSame(firstLine,secondLine) == false)
                {
                    toEdit << secondLine;
                }

                break;
            }

        }

        if (wasFound == false)
            toDelete << firstLine;

    }

     for (int i = 0; i < secondList.size(); ++i)
     {
         QStringList secondLine = secondList[i].toStringList();

         QString secondCode = secondLine[0];

         bool wasFound = false;

         for (int j = 0; j < firstList.size(); ++j)
         {
                QStringList firstLine = firstList[j].toStringList();
                QString firstCode = firstLine[0];

                if (firstCode == secondCode)
                {
                    wasFound = true;
                    break;
                }
         }

         if (wasFound==false)
             toInsert << secondLine;
     }
}

