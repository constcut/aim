#include "abstractBase.h"

#include <QDebug>
#include <QSqlError>
#include <QtSql>

#include <QRegExp>

#include <QDateTime>

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

//=============!!!!!!!!!!!!!!!!!!!!!!!!!====================
/////////HEY please move that function outside it could be used in many places
/// nice to make file something like listoperations.h
//=============!!!!!!!!!!!!!!!!!!!!!!!!!====================

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
        //1: should make from it another QVariantList
        //this means - add field parent, and also contain all other fields
        //as extra option there could be an exercise, for addin field parent, into table,
        //and on edit of that field - reload model, if was changed parent one

        //2: compare 2 Lists this could be done by something like void UsersLoader::compareLists
        //also same thing was in rmp

        //3: use 3 lists - to insert, to delete, to edit

        //great if they can work just by QStringList, even if they would work slow first
}


///TO USE THIS FUNCTION WE MUST HAVE STORED VARIABLES IN CLASS LSB
///
///
///


inline uint qHash(const QVariant& varString)
{
    QString stringValue = varString.toString();
    return qHash(stringValue,0xa03f);
}

QVariantList addTagPairs(QStringList aimLine)
{
    QString tag=aimLine[5]; //~ :)
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

QVariantList LocalSqlBase::getAllTags()
{
        QVariantList allAims = getAims();

        QVariantList allTags;

        for (int i = 0; i < allAims.size(); ++i)
        {
           QVariantList thisTag = addTagPairs(allAims[i].toStringList());
           //Must be putted in set then
           //OR can just first summ all of them,
           //then put into set - deleting all the repeats

            allTags += thisTag;
        }

        QSet<QVariant> norepeats = allTags.toSet();

        QVariantList norepeatsList = norepeats.toList();

        return norepeatsList;

        //1: get all aims, trace through them adding new tags to list

        //2: make function that adds to set pairs (tag,parent), and creates as many as needed for dotted tags

        //3: from that set of pairs we fill the TreeModel -
            ///BUT here we can only make QVariantList yet - so we make it, with 2 QStrings tag, parent; and then make TreeModel outside
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
    //later make a filter that not already done or something simmiliar

    QString requestBody = "SELECT * FROM aims";

    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,2);  //no need too much mostly id + name, that are first 2

    QStringList result;
    result << QString(); //first string is empty

    for (int i = 0; i < aimsList.size(); ++i)
    {
        QStringList aimFields =  aimsList[i].toStringList();

        result << aimFields[1]; //aim name
    }

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


QVariantList LocalSqlBase::getAims()
{
    QString requestBody = "SELECT * FROM aims";

    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,13); //11 is fields amount

    ///GOOD to scroll qml to add, if there are no aims

    return aimsList;
}

QVariantList LocalSqlBase::getAimsByDate(QString date)
{
    if (date.indexOf("T") != -1)
        date = date.mid(0,date.indexOf("T"));

    QString requestBody = "SELECT * FROM aims WHERE datePart='" + date + "';";

    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,13); //11 is fields amount

    ///GOOD to scroll qml to add, if there are no aims

    return aimsList;
}

QStringList LocalSqlBase::getSingleAim(QString aimId)
{
    QString requestBody = "SELECT * FROM aims WHERE aimId='" + aimId + "';";
    QSqlQuery request = executeRequest(requestBody);

    QVariantList oneLine = fillList(request,13); //11 is fields amount
    QStringList result = oneLine[0].toStringList();

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

bool LocalSqlBase::createTablesIfNeeded()
{

     //QLatin1String aimTableCreate("create table genres(id integer primary key, name varchar);");

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

                           //OH looks like we didn't recreate everything
                           //after deleting this field - so
                           //now there are some errors in qml for repeatable
                           //and privacy that are fine with old base, lol

                           "repeatable text,"
                           "privacy text"

                         ");"); //charset no used sorry

    QSqlQuery request = executeRequest(aimTableCreate);



     return false;
    //return request.next();
}


bool LocalSqlBase::fillTreeModelWithAims(TreeModel *treeModel)
{
    treeModel->fillWithAimList(getAims());

    return false;

    //function moved into void TreeModel::fillWithAimList(QVariantList allAims)

    //1: get with no parent
    //a: make list of them b: delete them from the main list
    //2: try to find from the rest those whose parent are in list
    //a: make list of them b: delete them from the main list
    //3: if list of them != 0 and if main list != 0 jump to 2
}
