#include "tabelserialize.h"


void TableSerialize::fromRawString(QString source)
{
    if (source.size())
        if (source[source.size()-1]=='\n')
        source = source.mid(0,source.size()-1);

    QStringList lines = source.split(QString("\n"));

    int number = 0;

    while (number < lines.count()) {

        int position = 0;
        /*
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }
        */ //avoid skip spaces for tables (not trees)



        QString lineData = lines[number].mid(position); //there was trimmed
        if (lineData.isEmpty())
            lineData = " "; //fix

        if (!lineData.isEmpty()) {
            QStringList columnStrings = lineData.split("\t",QString::SkipEmptyParts);

            //auto trim \n
            for (int i = 0; i < columnStrings.size(); ++i)
            {
                QString currentString = columnStrings[i];

                if (currentString.size())
                if (currentString[currentString.size()-1] =='\n')
                    currentString = currentString.mid(0,currentString.size()-1);

                if (currentString.isEmpty())
                    currentString==" ";

                columnStrings[i] = currentString;
            }


            tableRows << columnStrings;
        }

        ++number;
    }
}

void TableSerialize::fromString(QString source)
{
    tableRows.clear();

    QByteArray sourceArray = source.toLocal8Bit(); //switch?
    QByteArray decoded = QByteArray::fromBase64(sourceArray);

    QString decodedString = QString(decoded);

    fromRawString(decodedString);

}

QString TableSerialize:: toString()
{
    QString response;
    QString sepparator(4,'\t');

    for (int i = 0; i < tableRows.size(); ++i)
    {
        QStringList currentRow = tableRows[i];

        for (int j = 0; j < currentRow.size(); ++j)
        {
            response += currentRow[j];
            if (j != currentRow.size()-1)
                response += sepparator;

        }

        response += "\n";
    }

    //then cover under base 64
    QByteArray array = response.toUtf8();
    QByteArray base64Array = array.toBase64();
    QString codedResponse = base64Array;

    return codedResponse;
}

void TableSerialize::addRow(QStringList oneRow)
{
    tableRows << oneRow;
}

QStringList TableSerialize::getRow(int index)
{
    return tableRows[index];
}

int TableSerialize::size()
{
    return tableRows.size();
}


QString TableSerialize:: toRawString()
{
    QString response;
    QString sepparator(4,'\t');

    for (int i = 0; i < tableRows.size(); ++i)
    {
        QStringList currentRow = tableRows[i];

        for (int j = 0; j < currentRow.size(); ++j)
        {
            if (currentRow[j].isEmpty() == false)
                response += currentRow[j];
            else
                response += " ";

            if (j != currentRow.size()-1)
                response += sepparator;

        }

        response += "\r\n";
    }
    return response;
}
