#include "abstractBase.h"

#include <QDebug>
#include <QSqlError>
#include <QtSql>

#include <QRegExp>

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
    QString timePart = timeAndDate.mid(timeAndDate.indexOf("T")+1);
    QString datePart = timeAndDate.mid(0,timeAndDate.indexOf("T"));


    QString requestBody("INSERT INTO aims (aimName,timePart,datePart,comment,tag," //OUTPUT INSERTED.aimId
                        "assignTo,priority,parentAim) VALUES('" + aimName + "','" +
                        timePart + "','" + datePart  + "','" + comment + "','" + tag + "','" + assignTo + "','" + priority  +
                         "','" + parent + "');");


    QSqlQuery request = executeRequest(requestBody);

    if (request.next())
        return request.value(0).toInt();

    return -1;
}

bool LocalSqlBase::editAim(QString aimId,QString aimName, QString timeAndDate, QString comment, QString tag,
                           QString assignTo, /*delayed*/ QString priority,  QString parent, QString progress,
                         QString repeatable, QString privacy)
{
    QString timePart = timeAndDate.mid(timeAndDate.indexOf("T")+1);
    QString datePart = timeAndDate.mid(0,timeAndDate.indexOf("T"));


    QString requestBody = QString("UPDATE aims SET aimName='%1',timePart='%2',timePart='%3',comment='%4',tag='%5',"
                        "assignTo='%6',priority='%7',parent='%8' WHERE aimId='%9';")
            .arg(aimName).arg(timePart).arg(datePart).arg(comment).arg(tag).arg(assignTo).arg(priority).arg(parent).arg(aimId);
    ///use this practice to replace all the bad code

   // qDebug() << requestBody << " formed request";

    QSqlQuery request = executeRequest(requestBody);

    if (request.next())
        return request.value(0).toInt(); //no sure it works..

    return false;
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

    for (int i = 0; i < aimsList.size(); ++i)
    {
        QStringList aimFields =  aimsList[i].toStringList();

        result << aimFields[1]; //aim name
    }

    return result;
}


QVariantList LocalSqlBase::getAims()
{
    QString requestBody = "SELECT * FROM aims";

    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,11); //11 is fields amount

    ///GOOD to scroll qml to add, if there are no aims

    return aimsList;
}

QVariantList LocalSqlBase::getAimsByDate(QString date)
{
    if (date.indexOf("T") != -1)
        date = date.mid(0,date.indexOf("T"));

    QString requestBody = "SELECT * FROM aims WHERE datePart='" + date + "';";

    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,11); //11 is fields amount

    ///GOOD to scroll qml to add, if there are no aims

    return aimsList;
}

QStringList LocalSqlBase::getSingleAim(QString aimId)
{
    QString requestBody = "SELECT * FROM aims WHERE aimId='" + aimId + "';";
    QSqlQuery request = executeRequest(requestBody);

    QVariantList oneLine = fillList(request,11); //11 is fields amount
    QStringList result = oneLine[0].toStringList();

    return result;
}

QVariantList LocalSqlBase::searchAimsByName(QString searchText)
{
    QString requestBody = "SELECT * FROM aims";
    QSqlQuery request = executeRequest(requestBody);

    QVariantList searchResult;
    QVariantList aimsList = fillList(request,11);

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

                           "repeatable text,"
                           "privacy text"

                         ");"); //charset no used sorry

    QSqlQuery request = executeRequest(aimTableCreate);



     return false;
    //return request.next();
}


bool LocalSqlBase::fillTreeModelWithAims(TreeModel *treeModel)
{
    QVariantList allAims = getAims();

    QVariantList rootAims;
    QList<TreeItem*> treeItems;

    for (int i = 0 ; i < allAims.size(); ++i)
    {
        QStringList aimList = allAims[i].toStringList();
        QString parent = aimList[10];
        if (parent.isEmpty())
        {
            rootAims << aimList;
            //and also need to delete it from all aims...
            //not extra nice but:
            allAims.removeAt(i);
            --i; ///shift because of delete

            //ALSO here we create new TreeItem - thats linked
                                        //to rootItem in TreeModel

            //And we would use it later

            QVector<QVariant> listPreparedForTree;

            for (int k = 0; k < aimList.size(); ++i)
            {
                QVariant value = aimList[k];
                listPreparedForTree << value;
            }
                                                ///CHECK about 2nd argument, columns count maybe shouldn't exceed predefined names (4 in our case)

            TreeItem *item = new TreeItem(listPreparedForTree,treeModel->getColumnNamesSize(),treeModel->getRootItem()); ///CONSTRUCTOR!
            //WE MUST SET HERE ALL THE DATA IN ITEM!
            treeItems << item;
        }
    }

    while (rootAims.empty()==false && allAims.size() > 0)
    {
        QVariantList nextRootAims;
        QList<TreeItem*> nextTreeItems;

        for (int i = 0; i < rootAims.size(); ++i)
        {
            QStringList aimList = rootAims[i].toStringList();
            QString aimName = aimList[1];

            for (int j = 0; j < allAims.size(); ++j)
            {
                QStringList nextAimList = allAims[j].toStringList();
                QString parent = nextAimList[10];

                if (parent == aimName)
                {
                    nextRootAims << nextAimList;
                    allAims.removeAt(j);
                    --j;

                    TreeItem *parentItem = treeItems[i];

                    //SO AGAIN: here we create a TreeItem,
                            //and store is somewhere, but also this time
                    //WE NEED to add this one, to its parent TreeItem!
                    //So need to have some sublist

                    QVector<QVariant> listPreparedForTree;

                    for (int k = 0; k < nextAimList.size(); ++i)
                    {
                        QVariant value = nextAimList[k];
                        listPreparedForTree << value;
                    }

                    TreeItem *item = new TreeItem(listPreparedForTree,treeModel->getColumnNamesSize(),parentItem); ///CONSTRUCTOR!
                    //WE MUST SET HERE ALL THE DATA IN ITEM!
                    nextTreeItems << item;

                    parentItem->appendChild(item);

                }
            }
        }

        rootAims = nextRootAims;
    }

    //1: get with no parent
    //a: make list of them b: delete them from the main list
    //2: try to find from the rest those whose parent are in list
    //a: make list of them b: delete them from the main list
    //3: if list of them != 0 and if main list != 0 jump to 2
}
