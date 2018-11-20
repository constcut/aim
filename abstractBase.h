#ifndef ABSTRACTBASE_H
#define ABSTRACTBASE_H

#include <QObject>
#include <QVariantList>
#include <QSqlQuery>

#include "wslib/treemodel.h"

class AbstractSqlBase : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSqlBase(QObject *parent = 0);

    //as we start to move it into server

    //Aims
    /*
    Q_INVOKABLE virtual int addAim(QString aimName, QString timeAndDate, QString comment, QString tag,
                                   QString assignTo, QString priority="",  QString parent="", QString progress="",
                                  QString repeatable="", QString privacy="")=0;

    Q_INVOKABLE virtual bool editAim(QString aimId, QString aimName, QString timeAndDate, QString comment, QString tag,
                                     QString assignTo, QString priority="",  QString parent="", QString progress="",
                                   QString repeatable="", QString privacy="")=0; //maybe some params as links should be accesable alone

    Q_INVOKABLE virtual bool deleteAim(QString aimId)=0;

    Q_INVOKABLE virtual QVariantList getAims()=0;
    Q_INVOKABLE virtual QVariantList getAimsByDate(QString date)=0;
    Q_INVOKABLE virtual QStringList getSingleAim(QString aimId)=0;

     Q_INVOKABLE virtual QStringList getAimsNames()=0;

    Q_INVOKABLE virtual QStringList getAimLinks(QString aimName)=0; //aim id
    Q_INVOKABLE virtual bool setAimLinks(QString aimName, QStringList aimLinks)=0; //aim id
    */

    /*
    //Category
    Q_INVOKABLE virtual int addCategory(QString categoryName, QString categoryParent)=0; //name, parent
    Q_INVOKABLE virtual QVariantList getCatregories()=0; //over all - good to build it as a tree - or at least fill as sequence

    //Lists
    Q_INVOKABLE virtual int addList(QString listName, QString listType="string")=0; //type(string, aim, columns=table)
    Q_INVOKABLE virtual int insertListElement(QString listName, QString elementValue)=0;
    Q_INVOKABLE virtual bool editListElement(QString listName, int elementIndex, QString elementValue)=0;
    Q_INVOKABLE virtual QVariantList getLists()=0; //can have a type - default = all

    //Friends
    Q_INVOKABLE virtual QVariantList getFriends()=0;
    Q_INVOKABLE virtual int addFriend(QString friendNick)=0; //nick or tel or email
    Q_INVOKABLE virtual QString searchFriendByMail(QString mail)=0;
    Q_INVOKABLE virtual QString searchFriendByPhone(QString phone)=0;

    //Groups
    Q_INVOKABLE virtual QVariantList getGroups()=0;
    Q_INVOKABLE virtual int createGroup(QString groupName)=0; //group name
    Q_INVOKABLE virtual bool addFriendToGroup(QString groupName, QString friendNick)=0; //group name + friend nick

    //Chats
    Q_INVOKABLE virtual bool sendPrivateMessage(QString friendNick, QString message, QString aimId="")=0; //nick name
    Q_INVOKABLE virtual bool sendGroupMessage(QString groupName, QString message, QString aimId="")=0; //group name
    */

    ///FOR a little while class abadoned untill remote appear so please later inlucde things such as search etc

protected:
    virtual QSqlQuery executeRequest(QString requestBody)=0;

    void notifySqlError(QSqlQuery &query);
    QString lastRequestBody;

    QVariantList fillList(QSqlQuery &query, int rowsCount);

signals:

public slots:
};

///===========================================================================

class LocalSqlBase : public AbstractSqlBase  //RENAME into LocalAimBase
{
        Q_OBJECT
public:
    explicit LocalSqlBase(QObject *parent = 0);
    ~LocalSqlBase();

    //Aims
    Q_INVOKABLE int addAim(QString aimName, QString timeAndDate, QString comment, QString tag,
                           QString assignTo, QString priority="",  QString parent="", QString progress="",
                           QString repeatable="", QString privacy="");

    Q_INVOKABLE bool editAim(QString aimId, QString aimName, QString timeAndDate, QString comment, QString tag,
                             QString assignTo, QString priority="", QString parent="", QString progress="",
                           QString repeatable="", QString privacy=""); //maybe some params as links should be accesable alone

    Q_INVOKABLE bool deleteAim(QString aimId);


    //=====================================

    Q_INVOKABLE QVariantList getAims();
    Q_INVOKABLE QStringList getAimsNames();
    Q_INVOKABLE QStringList getAimsNamesBackwards(); //and with parent

    Q_INVOKABLE QStringList getSingleAim(QString aimId);

    Q_INVOKABLE QVariantList getChildAims(QString parentAimId);
    Q_INVOKABLE QStringList getChildAimsNames(QString parentAimId);

    //=======================================

    Q_INVOKABLE QVariantList getAimsByDate(QString date);
    Q_INVOKABLE QVariantList getAimsByDateOnly(QString date);
    Q_INVOKABLE QVariantList getPeriodHitAimsByDate(QString date);

    Q_INVOKABLE QVariantList getCurrentMomementAims();
    Q_INVOKABLE QStringList getCurrentMomementAimsNames();

    Q_INVOKABLE QVariantList getFutureAims();
    Q_INVOKABLE QVariantList getDelayedAims();

    Q_INVOKABLE QStringList getFutureAimsNames();
    Q_INVOKABLE QStringList getDelayedAimsNames();

    //=====================================


    Q_INVOKABLE QVariantList getAllTags();
    Q_INVOKABLE bool editTreeAims(TreeModel *aims);
    Q_INVOKABLE bool fillTreeModelWithAims();
    Q_INVOKABLE  bool fillTreeModelWithTags();

     //==================================

    Q_INVOKABLE bool addActivity(QString aimId, QString aimName,
                     QString operation, QString totalLength="");

    Q_INVOKABLE QVariantList getActivityLog(QString aimId); //all records
    Q_INVOKABLE QString getActivitySummary(QString aimId);

     Q_INVOKABLE bool updateAimProgress(QString aimId, QString progress, QString progressText);

    //=========================

    Q_INVOKABLE QStringList getAimLinks(QString aimName);
    Q_INVOKABLE bool setAimLinks(QString aimName, QStringList aimLinks);

    //===================================

    //search functions
    Q_INVOKABLE QVariantList searchAimsByName(QString searchText);


    //------------------------------------------------------------------------
    //Local tables creator
    bool createTablesIfNeeded();


    //TreeModels - yet that stay here, as part of base structure, but accesable
    TreeModel aimsTree;
    TreeModel tagsTree;

protected:
    QSqlQuery executeRequest(QString requestBody);

    QSqlError initDatabase();
};

///===========================================================================

//note that its possible to make encryption on database
class RemoteSqlBase : public AbstractSqlBase
{
    //connection on creation - after that few function availible first before login
    Q_OBJECT
public:

    Q_INVOKABLE bool isLoggedIn() { return false;}
    Q_INVOKABLE bool tryLogIn(){ return false;} //username + password

    Q_INVOKABLE bool registration(){ return false;} //username, password, tel, e-mail + some capture when goes into tests
    //after login on registration

    ///Here also must be special functions to load QVariantList into needed tables, to sync

private:
    bool loggedIn;//=false
};

///===========================================================================


///SmartBase - automatically connects to remote if possible, but also always updates locale one
//in fact local one always used as a source but when possible syn with remote one

#endif // ABSTRACTBASE_H
