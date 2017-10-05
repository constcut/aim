#include "abstractBase.h"

#include <QDebug>
#include <QSqlError>
#include <QtSql>

#include <QRegExp>

#include <QDateTime>
#include <QDate>

#include "listoperations.h"

AbstractSqlBase::AbstractSqlBase(QObject *parent) : QObject(parent)
{

}

void AbstractSqlBase::notifySqlError(QSqlQuery &query)
{
    qDebug() << "SQL error: "<<query.lastError();
    qDebug() << "On request: "<<lastRequestBody;
}

QVariantList AbstractSqlBase::fillList(QSqlQuery &query, int rowsCount) //maybe olso return as link&
{
    QVariantList filledList;

    while (query.next())
    {
        QStringList oneLine;

        for (int i = 0; i < rowsCount; ++i) //sqlite cant figure it out
        {
            oneLine << query.value(i).toString();
            //qDebug() << i << query.value(i).toString();
        }

        filledList << oneLine;
    }

    return filledList;
}

//=======================
/////////////===Local Base==\\\\\\\\\\\\\\\\\\
//=======================

LocalSqlBase::LocalSqlBase(QObject *parent): AbstractSqlBase(parent)
{
    /*
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        qDebug() << "SQLite driver doesn't exists";


    //QSqlDatabase::database("localbase", false).close();
   // QSqlDatabase::removeDatabase("localbase");

     dataBase = QSqlDatabase::addDatabase("QSQLITE");
     dataBase.setDatabaseName(":memory:"); //localbase.sqlite:

     if (dataBase.isValid()==false)
         qDebug() << "Base is not valid!";

     if (dataBase.open()==false)
         qDebug() << "Failed to open sqlite base";
     else
         qDebug() << "SQLite base opened";
         */
}

LocalSqlBase::~LocalSqlBase()
{
    //dataBase.close();
}

QSqlQuery LocalSqlBase::executeRequest(QString requestBody)
{
    lastRequestBody = requestBody;

    QSqlQuery request; //always do on default on sqlite

    if (request.exec(requestBody)==false)
        notifySqlError(request);

    return request;
}

////========================================================================

//Good to make auto creating bases for local one with SQL statements

//==================first start on most simple things=======================

int LocalSqlBase::addCategory(QString categoryName, QString categoryParent)
{
    QString requestBody("INSERT OUTPUT INSERTED.categoryId INTO categories (categoryName,categoryParent) VALUES('%1','%2')");
    requestBody.arg(categoryName,categoryParent);

    QSqlQuery request = executeRequest(requestBody);

    if (request.next())
        return request.value(0).toInt();

    return -1;
}

QVariantList LocalSqlBase::getCatregories() //please check about & return - is it qml fine
{
    QString requestBody = "SELECT * FROM categories"; //on remote would append WHERE userId=''
    ///please think about how to use local & global bases fine - not to copypaste whole code
    QSqlQuery request = executeRequest(requestBody);
    return fillList(request,3);
}


int LocalSqlBase::addFriend(QString friendNick)
{
    QString requestBody("INSERT OUTPUT INSERTED.friendId INTO friends (friendNick) VALUES('%1')");
    requestBody.arg(friendNick);

    QSqlQuery request = executeRequest(requestBody);

    if (request.next())
        return request.value(0).toInt();

    return -1;
}

QVariantList LocalSqlBase::getFriends()
{
    QString requestBody = "SELECT * FROM friends"; //on remote would append WHERE userId=''
    QSqlQuery request = executeRequest(requestBody);
    return fillList(request,2);
}

QString LocalSqlBase::searchFriendByMail(QString mail)
{
    return ""; //unabled to search offline
}

QString LocalSqlBase::searchFriendByPhone(QString phone)
{
    return ""; //unabled to search offline
}

QVariantList LocalSqlBase::getGroups()
{
    QString requestBody = "SELECT * FROM groups";
    QSqlQuery request = executeRequest(requestBody);
    return fillList(request,2);
}

int LocalSqlBase::createGroup(QString groupName) //creater becomes owner
{
    return -1; //not usable offline
}

bool LocalSqlBase::addFriendToGroup(QString groupName, QString friendNick)
{
    return false; //not usable offline
}

bool LocalSqlBase::sendPrivateMessage(QString friendNick, QString message, QString aimId)
{
    QString requestBody("INSERT OUTPUT INSERTED.friendId INTO pendingMessages (messageType,destination,message,aimId) VALUES('f','%1','%2','%3')");
    requestBody.arg(friendNick,message,aimId); //f - means friend type message

    QSqlQuery request = executeRequest(requestBody);

    if (request.next())
        return request.value(0).toInt(); //amount of messages in queue

    return -1;
}
bool LocalSqlBase::sendGroupMessage(QString groupName, QString message, QString aimId)
{
    ///should store in the same temp base, just with different flags
}

int LocalSqlBase::addList(QString listName, QString listType)
{
    QString requestBody("INSERT OUTPUT INSERTED.friendId INTO lists (listName,listType) VALUES('%1','%2')");
    requestBody.arg(listName,listType);

    QSqlQuery request = executeRequest(requestBody);

    if (request.next())
        return request.value(0).toInt();

    return -1;
}

int LocalSqlBase::insertListElement(QString listName, QString elementValue)
{
    //value deppends on list type: on string - just it, on aim - its name (or id?)
}

bool LocalSqlBase::editListElement(QString listName, int elementIndex, QString elementValue)
{
    ///had to find list id first, then connect with it adding
}

QVariantList LocalSqlBase::getLists()
{
    QString requestBody = "SELECT * FROM lists";
    QSqlQuery request = executeRequest(requestBody);
    return fillList(request,3);
}
///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//=============FINALY AIMS itself=======================
///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


int LocalSqlBase::addAim(QString aimName, QString timeAndDate, QString comment, QString tag,
                         QString assignTo, /*delayed*/ QString priority,  QString parent, QString progress,
                        QString repeatable, QString privacy)
{

    QString datePart = timeAndDate.mid(0,timeAndDate.indexOf("T"));
    QString timePart;

    if (timeAndDate.indexOf("T") > 0)
        timePart = timeAndDate.mid(timeAndDate.indexOf("T")+1);

    QString requestBody("INSERT INTO aims (aimName,timePart,datePart,comment,tag," //OUTPUT INSERTED.aimId
                        "assignTo,priority,parentAim,repeatable) VALUES('" + aimName + "','" +
                        timePart + "','" + datePart  + "','" + comment + "','" + tag + "','" + assignTo + "','" + priority  +
                         "','" + parent +  "','" + repeatable + "');");


    QSqlQuery request = executeRequest(requestBody);

    if (request.next())
        return request.value(0).toInt();

    return -1;
}

bool LocalSqlBase::editAim(QString aimId,QString aimName, QString timeAndDate, QString comment, QString tag,
                           QString assignTo, /*delayed*/ QString priority,  QString parent, QString progress,
                         QString repeatable, QString privacy)
{
    QString datePart = timeAndDate.mid(0,timeAndDate.indexOf("T"));
    QString timePart;

    if (timeAndDate.indexOf("T") > 0)
        timePart = timeAndDate.mid(timeAndDate.indexOf("T")+1);

    QString requestBody = QString("UPDATE aims SET aimName='%1',timePart='%2',datePart='%3',comment='%4',tag='%5',"
                        "assignTo='%6',priority='%7',parentAim='%8',repeatable='%9'")
            .arg(aimName).arg(timePart).arg(datePart).arg(comment).arg(tag).arg(assignTo).arg(priority).arg(parent).arg(repeatable)
            +  QString(" WHERE aimId='%1';").arg(aimId);
    ///use this practice to replace all the bad code

    qDebug() << requestBody << " formed request";

    QSqlQuery request = executeRequest(requestBody);

    if (request.next())
        return request.value(0).toInt(); //no sure it works..

    return false;
}

bool LocalSqlBase::updateAimProgress(QString aimId, QString progress, QString progressText)
{
    QString requestBody = QString("UPDATE aims SET progress='%1',progressText='%2'")
            .arg(progress).arg(progressText)
            +  QString(" WHERE aimId='%1';").arg(aimId);
    ///use this practice to replace all the bad code
    ///
    QSqlQuery request = executeRequest(requestBody);


    QDateTime now = QDateTime::currentDateTime();
    QString currentMoment = now.toString("yyyy-MM-ddTHH:mm:ss");

    QString logRecord = QString("INSERT INTO progress (aimId,progress,progressText,moment) VALUES('%1','%2','%3','%4');")
            .arg(aimId).arg(progress).arg(progressText).arg(currentMoment);


    if (request.next())
        return request.value(0).toInt();
}


bool LocalSqlBase::editTreeAims(TreeModel *aims) //actually here must be QObject
{
    qDebug() << "Editing Tree Aims";

    QVariantList treeAims =  aims->getFullList();
    QVariantList allAims = getAims();

    QVariantList toInsert,toEdit,toDelete;
    compareLists(allAims, treeAims,toInsert,toEdit,toDelete);

    for (int i = 0 ; i < toInsert.size(); ++i)
    {
        QStringList aimLine = toInsert[i].toStringList();

        QString aimId = aimLine[0];
        QString aimName = aimLine[1];
        QString timePart = aimLine[2];
        QString datePart = aimLine[3];
        QString comment = aimLine[4];
        QString tag = aimLine[5];
        QString assignTo = aimLine[6];
        QString priority = aimLine[7];
        QString progress = aimLine[8];
        QString progressText = aimLine[9];
        QString parentAim = aimLine[10];
        //privacy had to be missing

        QString timeAndDate = datePart;
        if (timePart.isEmpty()==false)
            timeAndDate += "T" + timePart;

        //some other fields yet missing
        addAim(aimName,timeAndDate,comment,tag,assignTo,priority,parentAim,progress);
    }

    for (int i = 0; i < toEdit.size(); ++i)
    {
        QStringList aimLine = toEdit[i].toStringList();

        QString aimId = aimLine[0];
        QString aimName = aimLine[1];
        QString timePart = aimLine[2];
        QString datePart = aimLine[3];
        QString comment = aimLine[4];
        QString tag = aimLine[5];
        QString assignTo = aimLine[6];
        QString priority = aimLine[7];
        QString progress = aimLine[8];
        QString progressText = aimLine[9];
        QString parentAim = aimLine[10];
        //privacy had to be missing

        QString timeAndDate = datePart;
        if (timePart.isEmpty()==false)
            timeAndDate += "T" + timePart;

        editAim(aimId,aimName,timeAndDate,comment,tag,assignTo,priority,parentAim,progress);
    }

    //nice if there would be not a real delete but some put in trash
    for (int i = 0; i < toDelete.size(); ++i)
    {
        QStringList aimLine = toDelete[i].toStringList();

        QString aimId = aimLine[0];

        deleteAim(aimId);
    }

    return true;
}




inline uint qHash(const QVariant& varString)
{
    QString stringValue = varString.toString();
    return qHash(stringValue,0xa03f);
}

QVariantList addTagPairs(QString tag)
{
    QVariantList result;
if (tag.isEmpty())
    return result;

    int lastDot = tag.lastIndexOf(".");
    while (lastDot != -1) //hope its right condition
    {
        QStringList tagsPair;
        QString parentTag = tag.mid(0,lastDot); //or no -1. hahaha?

        tagsPair << tag << parentTag;

        result << tagsPair;

        tag=parentTag;
        lastDot = tag.lastIndexOf(".");
    }

    QStringList tagsPair;
    tagsPair << tag << QString(); //and root one

    result << tagsPair;

    return result;
}

QVariantList addTagPairs(QStringList aimLine)
{
    QString tag = aimLine[5];

    //TO MAKE POSSIBLE MULTIPLE TAGS WE JUST NEED TO
    QStringList tagParts = tag.split(" ");

    QVariantList result;

    for (int i = 0; i < tagParts.size(); ++i)
        result += addTagPairs(tagParts[i]);

                return result;
}

QVariantList LocalSqlBase::getAllTags()
{
        QVariantList allAims = getAims();
        QVariantList allTags;

        for (int i = 0; i < allAims.size(); ++i)
        {
           QVariantList thisTag = addTagPairs(allAims[i].toStringList());
            allTags += thisTag;
        }

        QSet<QVariant> norepeats = allTags.toSet();
        QVariantList norepeatsList = norepeats.toList();

        return norepeatsList;
}


bool LocalSqlBase::deleteAim(QString aimId)
{
    QString requestBody("DELETE FROM aims WHERE aimId='"+
                        aimId + "';");

    QSqlQuery request = executeRequest(requestBody);

    return request.next();
}

QStringList LocalSqlBase::getAimsNames()
{
    QVariantList aimsList = getAims();

    QStringList result;
    result << QString();
    result += createListByField(aimsList,1);

    return result;
}

QVariantList LocalSqlBase::getCurrentMomementAims()
{
    QDateTime now(QDateTime::currentDateTime());

    QString dateAndTime = now.toString("yyyy-MM-ddTHH:mm");

    QString timePart,datePart;

    datePart = dateAndTime.mid(0,dateAndTime.indexOf("T"));
    timePart = dateAndTime.mid(dateAndTime.indexOf("T")+1);

    QString requestBody = QString("SELECT * FROM aims WHERE timePart='%1' AND datePart='%2';")
            .arg(timePart).arg(datePart);

    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,13); //11 is fields amount

    return aimsList;
}

QStringList LocalSqlBase::getCurrentMomementAimsNames()
{
   return createListByField(getCurrentMomementAims(),1);
}


QVariantList LocalSqlBase::getFutureAims()
{
    QDate today(QDate::currentDate());

    QString requestBody = "SELECT * FROM aims WHERE datePart NOT NULL;";

    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,13); //11 is fields amount

    QVariantList futureAims;

    for (int i = 0; i < aimsList.size(); ++i)
    {
        QStringList aimLine = aimsList[i].toStringList();
        QString dateValue = aimLine[3];

        QDate date = QDate::fromString(dateValue,"yyyy-MM-dd");

        if (date > today)
            futureAims << aimLine;
    }

    return futureAims;
}

QVariantList LocalSqlBase::getDelayedAims()
{
    QDate today(QDate::currentDate());

    QString requestBody = "SELECT * FROM aims WHERE datePart NOT NULL;";

    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,13); //11 is fields amount

    QVariantList pastAims;

    for (int i = 0; i < aimsList.size(); ++i)
    {
        QStringList aimLine = aimsList[i].toStringList();
        QString dateValue = aimLine[3];

        QDate date = QDate::fromString(dateValue,"yyyy-MM-dd");

        if (date < today)
            pastAims << aimLine;
    }

    return pastAims;
}

QStringList LocalSqlBase::getFutureAimsNames()
{
    return createListByField(getFutureAims(),1);
}

QStringList LocalSqlBase::getDelayedAimsNames()
{
    return createListByField(getDelayedAims(),1);
}


QVariantList LocalSqlBase::getAims()
{
    QString requestBody = "SELECT * FROM aims";

    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,13); //11 is fields amount

    return aimsList;
}

QVariantList LocalSqlBase::getChildAims(QString parentAimId)
{
    QString requestBody =
            QString("SELECT * FROM aims WHERE parentAim='%1';")
            .arg(parentAimId);

    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,13); //11 is fields amount

    return aimsList;
}

QStringList LocalSqlBase::getChildAimsNames(QString parentAimId)
{
    QVariantList aimsList = getChildAims(parentAimId);
    return createListByField(aimsList,1);
}


QVariantList LocalSqlBase::getAimsByDate(QString date)
{
    QVariantList aimsList = getAimsByDateOnly(date);
            aimsList += getPeriodHitAimsByDate(date);

    return aimsList;
}

QVariantList LocalSqlBase::getAimsByDateOnly(QString date)
{
    if (date.indexOf("T") != -1)
        date = date.mid(0,date.indexOf("T"));

    QString requestBody = "SELECT * FROM aims WHERE datePart='" + date + "';";

    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,13); //11 is fields amount

    return aimsList;
}

bool canDateHitPeriod(QString originDate, QString period, QString searchDate)
{
    QDate originDateValue = QDate::fromString(originDate,"yyyy-MM-dd");
    QDate searchDateValue = QDate::fromString(searchDate,"yyyy-MM-dd");

    if (searchDateValue <= originDateValue)
        return false;

    QString daysPeriod = period.mid(0,period.indexOf("d"));
    qint64 daysAmount = daysPeriod.toInt();
    qint64 daysBetween = originDateValue.daysTo(searchDateValue);

    if (daysBetween%daysAmount==0)
        return true;

    return false;
}

QVariantList LocalSqlBase::getPeriodHitAimsByDate(QString date)
{
    if (date.indexOf("T") != -1)
        date = date.mid(0,date.indexOf("T"));

    QString requestBody = "SELECT * FROM aims WHERE datePart NOT NULL AND repeatable NOT NULL;";

    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,13); //11 is fields amount

    QVariantList hittedPeriodAims;

    //then we must check each of the date + period, for posibility to hit date
    for (int i = 0; i < aimsList.size(); ++i)
    {
        QStringList aimLine = aimsList[i].toStringList();

        QString originDate = aimLine[3];
        QString period = aimLine[11]; //actually 11, but only after reboot of base.. terrible

        if (canDateHitPeriod(originDate,period,date))
            hittedPeriodAims << aimLine;
    }

    return hittedPeriodAims;
}

QStringList LocalSqlBase::getSingleAim(QString aimId)
{
    QString requestBody = "SELECT * FROM aims WHERE aimId='" + aimId + "';";
    QSqlQuery request = executeRequest(requestBody);

    QVariantList allLines = fillList(request,13); //11 is fields amount
    QStringList result;

    if (allLines.size() > 0)
        result = allLines[0].toStringList(); //taking only first line

    return result;
}

QVariantList LocalSqlBase::searchAimsByName(QString searchText)
{
    QString requestBody = "SELECT * FROM aims";
    QSqlQuery request = executeRequest(requestBody);

    QVariantList searchResult;
    QVariantList aimsList = fillList(request,12);

    QRegExp pattern("*" + searchText + "*");
    pattern.setPatternSyntax(QRegExp::Wildcard);

    for (int i = 0; i < aimsList.size(); ++i)
    {
        QStringList aimLine= aimsList[i].toStringList();
        QString aimName = aimLine[1];

        if (pattern.exactMatch(aimName))
            searchResult.append(aimLine);

    }

    return searchResult;
}


QStringList LocalSqlBase::getAimLinks(QString aimName)
{
    QStringList filledList;

    return filledList;
}

bool LocalSqlBase::setAimLinks(QString aimName, QStringList aimLinks)
{

    return true;
}

//=================Here must be create functions - to be able start from very begining========


QSqlError  LocalSqlBase::initDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("local.sqlite");

    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();

    for (int i =0 ;i < tables.size(); ++i)
        qDebug() << tables;

    if (tables.contains("aims", Qt::CaseInsensitive)
        && tables.contains("categories", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery q;

    //ok migrate all the initiation here


    return QSqlError();
}


//========================================================================
//Activity things

bool LocalSqlBase::addActivity(QString aimId, QString aimName,
                 QString operation, QString totalLength)
{
    QDateTime now = QDateTime::currentDateTime();
    QString currentMoment = now.toString("yyyy-MM-ddTHH:mm:ss");

    QString requestBody("INSERT INTO actions (aimName,aimId,operation,moment,totalLength) VALUES('"
                        +aimName + "','" + aimId  + "','" + operation + "','" + currentMoment + "','" + totalLength + "');");

    QSqlQuery request = executeRequest(requestBody);
    if (request.next())
        return request.value(0).toInt();

    return false;
}

QVariantList LocalSqlBase::getActivityLog(QString aimId)
{
    //only last 100 records - make limit in future pay attention!
    QString requestBody("SELECT * FROM actions WHERE aimId='" + aimId + "';");
    QSqlQuery request = executeRequest(requestBody);
    QVariantList activityLogResult = fillList(request,13);
    return activityLogResult;
}

QString LocalSqlBase::getActivitySummary(QString aimId)
{
    QVariantList actLog = getActivityLog(aimId);
    int countStarts=0,countStops=0;

    int totalSeconds = 0;

    for (int i = 0; i < actLog.size(); ++i)
    {
       QStringList actionLine = actLog[i].toStringList();

       if (actionLine[3] == "start")
           ++countStarts;
       if (actionLine[3] == "stop")
       {
           ++countStops;

           QString actionLength = actionLine[5];
           int secondsAmount = actionLength.toInt();
           totalSeconds += secondsAmount;
       }
    }

    QString result = "Summary: " + QString::number(countStarts)
            + " starts\n" + QString::number(countStops) + " stops\n"
            + "Total seconds spent: " + QString::number(totalSeconds);

    return result;
}

//End of activity things
//========================================================================

bool LocalSqlBase::createTablesIfNeeded()
{
    qDebug() << initDatabase();

    QString aimTableCreate("CREATE TABLE IF NOT EXISTS aims (" //
                           "aimId integer primary key autoincrement NOT NULL,"
                           "aimName text NOT NULL,"
                           "timePart text,"//"timeAndDate text,"
                           "datePart text,"
                           "comment text,"
                           "tag text,"
                           "assignTo text NOT NULL,"

                           "priority text,"
                           "progress text,"
                           "progressText text,"
                           "parentAim text,"
                           "repeatable text,"
                           "privacy text,"
                           "duration text"

                         ");");

    QSqlQuery request = executeRequest(aimTableCreate);

    QString activityTableCreate("CREATE TABLE IF NOT EXISTS actions (" //
                           "actId integer primary key autoincrement NOT NULL,"
                           "aimName text NOT NULL,"
                           "aimId text NOT NULL,"
                           "operation text,"
                           "moment text,"
                           "totalLength text" //helper for stops
                         ");");

    QSqlQuery request2 = executeRequest(activityTableCreate);

    QString progressTableCreate("CREATE TABLE IF NOT EXISTS progress (" //
                           "recordId integer primary key autoincrement NOT NULL,"
                           "aimId text NOT NULL,"
                           "progress text,"
                           "progressText text"
                           "moment text"
                         ");");

    QSqlQuery request3 = executeRequest(progressTableCreate);

     return request.isValid() && request2.isValid() && request3.isValid();
}


bool LocalSqlBase::fillTreeModelWithAims()
{
    aimsTree.fillWithAimList(getAims());
    return false;
}

bool  LocalSqlBase::fillTreeModelWithTags()
{
    tagsTree.fillWithTagList(getAllTags());
    return false;
}
