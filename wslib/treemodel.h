#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QList>
#include <QVariant>
#include <QVector>
#include <QAbstractItemModel>
#include <QModelIndex>

class TreeItem
{
public:
    explicit TreeItem(const QVector<QVariant> &data, int columnsCount, TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int childNumber() const;
    TreeItem *parent();

    bool setData(int column, const QVariant &value);
    bool removeChildren(int position, int count);

    QString exportColumns();

    QStringList getColumns(QString parentCode="");

    bool setValueOnCondition(int conditionIndex, QString conditionText,
                             int settingIndex, QString settingText);

    bool setValueOnConditionIfEmpty(int conditionIndex, QString conditionText,
                             int settingIndex, QString settingText);

private:
    QList<TreeItem*> childItems;
    QVector<QVariant> itemData;
    TreeItem *parentItem;
};



class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum TreeModelRoles
    {
        ///please name here roles also
    };

    explicit TreeModel(QObject *parent = 0, QStringList columnsList = QStringList());
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const override;


    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;


    Q_INVOKABLE QString getFieldByIndex(QModelIndex index, int field);

    Q_INVOKABLE bool printTopValues();

    Q_INVOKABLE QString getAimId(const QModelIndex &index);

    Q_INVOKABLE bool addTopItem(const QModelIndex &index);
    Q_INVOKABLE bool addChildItem(const QModelIndex &index);
    Q_INVOKABLE bool setItemText(const QModelIndex &index, const QString text, int column=0);
    Q_INVOKABLE bool removeItem(const QModelIndex &index);

    Q_INVOKABLE QString getColumnName(int index);
    Q_INVOKABLE int getColumnIndex(QString name);
    Q_INVOKABLE int columnsAmount();


    Q_INVOKABLE QString getModelDataString();
    void setModelDataString(const QString &linesData);


    Q_INVOKABLE void fillWithAimList(QVariantList allAims);
     Q_INVOKABLE void fillWithTagList(QVariantList allTags);


    Q_INVOKABLE void saveToFile(QString filename="");
    Q_INVOKABLE void loadFromFile(QString filename="");

    QVariantList getFullList();


    bool setValueOnCondition(int conditionIndex, QString conditionText,
                             int settingIndex, QString settingText);

    bool setValueOnConditionIfEmpty(int conditionIndex, QString conditionText,
                             int settingIndex, QString settingText);


    TreeItem* getFirstItem()
    {
        if (rootItem->childCount())
            return rootItem->child(0);
        return 0;
    }

   void refillTableColumns(QStringList data);

   void setNewEntryTemplate(QString newTempate) { newEntryTemplate = newTempate; }

   TreeItem *getRootItem() { return rootItem; }
   size_t getColumnNamesSize() { return columnNames.size(); }

private:

   QString newEntryTemplate;

    bool setDataSpecial(const QModelIndex &index, const QVariant &value, int column);//for columns edit special thing
    TreeItem *getItem(const QModelIndex &index) const;

    void setupModelData(const QString &linesData, TreeItem *parent);
    void addChildrenExport(QStringList &outputList, TreeItem *parent, int level);

    void addChildrenList(QVariantList &outputList, TreeItem *parent);

    TreeItem *rootItem;
    QHash<int, QByteArray> roleNameMapping;

protected:
    void fillTableColumns();
    void autoFillRoles();
    void createRootElement();

    QList<QString> columnNames;
};
#endif // TREEMODEL_H
