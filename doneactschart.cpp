#include "doneactschart.h"

void DoneActsChartQML::paint(QPainter* painter){
//first we paint the time measure
    for (auto i = 0; i < 24; ++i){
        int x = (i+1)*hourWidth;
        painter->drawLine(x,aimHeight,x,aimHeight*2);
        painter->drawText(x,aimHeight/2,QString::number(i));
    }
//then aims will come
    QSet<QString> paintedAims;

    for (auto i = 0; i < lines.size(); ++i){
        //switch color too, and maybe set circles in the begin and ending

        int aimIndex = indexation[ lines[i].aimId ];

        switch (aimIndex % 7){
            case 0: painter->setPen(Qt::red); break;
            case 1: painter->setPen(Qt::yellow); break;
            case 2: painter->setPen(Qt::green); break;
            case 3: painter->setPen(Qt::white); break;
            case 4: painter->setPen(Qt::magenta); break;
            case 5: painter->setPen(Qt::cyan); break;
            case 6: painter->setPen(Qt::blue); break;
        }

        //qDebug() << i << " indexation "<<aimIndex;

        int y = (aimHeight*3) + aimHeight*aimIndex; //should insure there is such thing

        int x1 = lines[i].x1;
        int x2 = lines[i].x2;
        painter->drawLine(x1,y,x2,y);

        //maybe make font as small as possible?
        if (paintedAims.contains(lines[i].aimId)==false){
            painter->drawText(x1,y-aimHeight/2,lines[i].aimName);
            paintedAims.insert(lines[i].aimId);
        }

        painter->drawEllipse(x1-5,y-2,5,5);
        painter->drawEllipse(x2+1,y-2,5,5);
    }
}


int getHourPart(QString time){
    return time.mid(0,time.indexOf(":")).toInt();
}

int getTotalSecondsButNoHours(QString time){
    QString withourHours = time.mid(time.indexOf(":")+1);

    QString minutes = withourHours.mid(0,withourHours.indexOf(":"));
    QString seconds = withourHours.mid(withourHours.indexOf(":")+1);

    int totalSeconds = minutes.toInt()*60 + seconds.toInt();
    return totalSeconds;
}

//function of loading aims
void DoneActsChartQML::setSource(QVariantList actsList){

    indexation.clear();
    lines.clear();
    int counting = 0;

    for (auto i = 0; i < actsList.size(); ++i){
        QStringList oneLine = actsList[i].toStringList();

        if (oneLine[3] == "start")
            continue;

        if (indexation.contains(oneLine[2]) == false){
            indexation.insert(oneLine[2],counting);
            ++counting;
        }

        QString timePart = oneLine[4].mid(oneLine[4].indexOf("T")+1);

        //qDebug() << "Time part is "<<timePart<<" for "<<i;

        int hourPart = getHourPart(timePart);
        int restSeconds = getTotalSecondsButNoHours(timePart);

        //qDebug() << "Hour part "<<hourPart<<" ; and total seconds "<<restSeconds;

        int someX = (hourPart + 1)*hourWidth;
        int shift = (restSeconds*hourWidth)/3600; //60*60 seconds in hour
        int backShift = (oneLine[5].toInt()*hourWidth)/3600;

        int x2 = someX + shift;
        int x1 = x2 - backShift;

        DACline line;
        line.x1 = x1; line.x2 = x2;
        line.aimId = oneLine[2];
        line.aimName = oneLine[1];
        lines << line;
    }

    update(); //to insure painting

    //qDebug()<<"Loaded done chart "<<indexation.size()<<" and "<<lines.size();
}

int DoneActsChartQML::getFirstRecordX(){
    if (lines.size() == 0)
        return 0;

    return lines[0].x1;
}
