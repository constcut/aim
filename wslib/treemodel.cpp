#include "treemodel.h"

#include <QStringList>
#include <QDebug>
#include <QFile>

//#include <QFileDialog>

Q_DECLARE_METATYPE(QModelIndex)

//HOTFIX function

//IM sorry but there are done some issues in recognizing, when we have no columns
//there is an issue if we would leave at least one field without flags
///PLEASE PAY attention here, on refactoring or glossing

void repairOldFirmStructureStringLine(QStringList& firmLine)
{
    //this function used simply to swap fields for example place code and

    int startSize = firmLine.size();

    //if (firmLine.size() > 3)
    {
        QString flags = firmLine[3];
        firmLine.removeAt(3);
        firmLine << flags;
    }

    if (firmLine.size() != startSize)
        qDebug() << "Fields unequal size "<<firmLine.size();
}

//============Table=============

TreeItem::TreeItem(const QVector<QVariant> &data, int columnsCount, TreeItem *parent)
{
    parentItem = parent;
    itemData = data;

    if (itemData.size() < columnsCount)
        for (int i = itemData.size()-1; i < columnsCount; ++i)
            itemData.append(" ");
}

TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
    return childItems.value(row);
}

int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::columnCount() const
{
    return itemData.count();
}

QVariant TreeItem::data(int column) const
{
    return itemData.value(column);
}

TreeItem *TreeItem::parent()
{
    return parentItem;
}

int TreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

QString TreeItem::exportColumns()
{
    QString result;

    for (int i = 0; i < columnCount(); ++i)
    {
        result += data(i).toString();

        QString sepparator(4,'\t');

        if (i!= columnCount()-1)
            result += sepparator;
    }

    return result;
}

QStringList TreeItem::getColumns(QString parentCode)
{
    QStringList response;

    for (int i = 0; i < columnCount(); ++i)
        response << data(i).toString();


    response << parentCode;

    if (childCount() > 0)
        response << "HasChild";
    else
        response << "NoChild";

    return response;
}

bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool TreeItem::setData(int column, const QVariant &value)
{
    if (column < 0)
        return false;

    QString insertingValue = value.toString();
    insertingValue = insertingValue.trimmed();
    if (insertingValue.isEmpty())
        insertingValue = " ";

    if (column >= itemData.size())
    {
        itemData.append(insertingValue);
        return true;
    }

    itemData[column] = insertingValue;
    return true;
}

///=!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//========================Tree Model============================
///=!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


TreeModel::TreeModel(QObject *parent, QStringList columnsList)
    : QAbstractItemModel(parent),rootItem(0)
{
    newEntryTemplate = " ";

    if (columnsList.size()==0)
    {
        fillTableColumns();
        autoFillRoles();
        createRootElement();
    }
    else
        refillTableColumns(columnsList);
}

void TreeModel::refillTableColumns(QStringList data)
{
    columnNames = data;
    autoFillRoles();
    createRootElement();
}

void TreeModel::fillTableColumns()
{
    columnNames.clear();

    columnNames.append("Aim name");
    columnNames.append("Time");
    columnNames.append("Date");
    columnNames.append("Comment");

    //columnNames.append("flags");
}

void TreeModel::autoFillRoles()
{
    roleNameMapping.clear();

    for (int i = 0; i < columnNames.size(); ++i)
         roleNameMapping[Qt::UserRole + 1 + i]= columnNames[i].toUtf8();
}

void TreeModel::createRootElement()
{
    QVector<QVariant> rootData;
    for (int i = 0; i < columnNames.size(); ++i)
        rootData << columnNames[i];

    if (rootItem)
    {
        delete rootItem;
        rootItem = 0;
    }

    rootItem = new TreeItem(rootData, columnNames.size());
}

QString TreeModel::getColumnName(int index)
{
    if (index < 0 || index >= columnNames.size())
        return "";

        return columnNames[index];
}

int TreeModel::getColumnIndex(QString name)
{
    for (int i = 0; i < columnNames.size(); ++i)
        if (columnNames[i]==name)
            return i;

    return -1;
}

int TreeModel::columnsAmount()
{
    return columnNames.size() ;
}

TreeModel::~TreeModel()
{
    if (rootItem)
    {
        delete rootItem;
        rootItem = 0;
    }
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QString TreeModel::getFieldByIndex(QModelIndex index, int field)
{
    TreeItem *item = getItem(index);

    if (item->columnCount() > field)
    {
        QString string  = item->data(field).toString();
        return string;
    }

    return "";
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(role - Qt::UserRole - 1);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    return roleNameMapping;
}

void TreeModel::setupModelData(const QString &linesData, TreeItem *parent)
{
    QStringList  lines = linesData.split(QString("\n"));

    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {

        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {

            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            parents.last()->appendChild(new TreeItem(columnData, columnNames.size(), parents.last()));
        }

        ++number;
    }
}

//=========import + export============

QString TreeModel::getModelDataString()
{
    QStringList exportData;

    for (int i = 0; i < rootItem->childCount(); ++i)
    {
        TreeItem *currentItem = rootItem->child(i);
        QString oneLine = currentItem->exportColumns();

        exportData.append(oneLine);

        if (currentItem->childCount())
            addChildrenExport(exportData,currentItem,1);
    }

    QString response;

    for (int i = 0; i < exportData.size(); ++i)
        response += exportData[i] + "\n";

    return response;
}

QList<QStringList> TreeModel::getFullList()
{
    QList<QStringList> response;

    for (int i = 0; i < rootItem->childCount(); ++i)
    {
        TreeItem *currentItem = rootItem->child(i);
        QStringList oneLine = currentItem->getColumns();

        //oneLine << " ";
        repairOldFirmStructureStringLine(oneLine);

        response << oneLine;

        if (currentItem->childCount())
            addChildrenList(response,currentItem);
    }

    return response;
}

bool TreeModel::setValueOnCondition(int conditionIndex, QString conditionText,
                         int settingIndex, QString settingText)
{
    //MOST SIMPLE WAY EVER
    return rootItem->setValueOnCondition(conditionIndex,conditionText,settingIndex,settingText);
}

bool TreeItem::setValueOnCondition(int conditionIndex, QString conditionText, int settingIndex, QString settingText)
{
    if (itemData.size() > conditionIndex)
    {
        if (itemData[conditionIndex].toString() == conditionText)
        {
            if (itemData.size() > settingIndex)
            {
                itemData[settingIndex] = settingText;
                return true;
            }
            else
                return false;
        }
    }

    for (int i = 0; i < childItems.size(); ++i)
    {
        if (childItems[i]->setValueOnCondition(conditionIndex,conditionText,settingIndex, settingText)) //recurse, but it wouldn't grow too mush
            return true;
    }

    return false;
}


bool TreeModel::setValueOnConditionIfEmpty(int conditionIndex, QString conditionText,
                         int settingIndex, QString settingText)
{
    //MOST SIMPLE WAY EVER
    return rootItem->setValueOnConditionIfEmpty(conditionIndex,conditionText,settingIndex,settingText);
}

bool TreeItem::setValueOnConditionIfEmpty(int conditionIndex, QString conditionText, int settingIndex, QString settingText)
{
    if (itemData.size() > conditionIndex)
    {
        if (itemData[conditionIndex].toString() == conditionText)
        {
            if (itemData.size() > settingIndex)
            {
                QString tempString = itemData[settingIndex].toString().trimmed();
                if (tempString.isEmpty()) ///SET value only if there is no value
                {
                    itemData[settingIndex] = settingText;
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
    }

    for (int i = 0; i < childItems.size(); ++i)
    {
        if (childItems[i]->setValueOnConditionIfEmpty(conditionIndex,conditionText,settingIndex, settingText)) //recurse, but it wouldn't grow too mush
            return true;
    }

    return false;
}



//not set yet
void TreeModel::addChildrenList(QList<QStringList> &outputList, TreeItem *parent)
{
    QStringList parentLine = parent->getColumns();
    QString parentCode =  parentLine[1];

    for (int i = 0; i < parent->childCount(); ++i)
    {
        TreeItem *currentItem = parent->child(i);
        QStringList oneLine = currentItem->getColumns(parentCode);

        //oneLine << " ";
        repairOldFirmStructureStringLine(oneLine);

        outputList.append(oneLine);

        if (currentItem->childCount()) //recurse should leave - it always possible
            addChildrenList(outputList,currentItem);
    }
}

void TreeModel::addChildrenExport(QStringList &outputList, TreeItem *parent, int level)
{
    for (int i = 0; i < parent->childCount(); ++i)
    {
        TreeItem *currentItem = parent->child(i);
        QString oneLine = currentItem->exportColumns();

        QString spaces(4*level,' ');

        oneLine = spaces + oneLine;
        outputList.append(oneLine);

        if (currentItem->childCount())
            addChildrenExport(outputList,currentItem,level+1);
    }
}

void TreeModel::setModelDataString(const QString &linesData)
{

    beginResetModel();

    if (rootItem)
    {
        delete rootItem;
        rootItem = 0;
    }

    createRootElement();
    endResetModel();;

    beginResetModel();
    setupModelData(linesData,rootItem);
    endResetModel();
}

void TreeModel::fillWithAimList(QVariantList allAims)
{

    beginResetModel();

    if (rootItem)
    {
        delete rootItem;
        rootItem = 0;
    }

    createRootElement();
    endResetModel();;

    beginResetModel();

    //action itself


    //action end

    endResetModel();
}

void TreeModel::saveToFile(QString filename)
{

    if (filename.isEmpty())
        return;
        //filename = QFileDialog::getSaveFileName(0,"Save tree struct","","(*.structure)");

    QFile outputFile;
    outputFile.setFileName(filename);
    outputFile.open(QIODevice::WriteOnly);
    QString exportData = getModelDataString();
    outputFile.write(exportData.toUtf8());
    outputFile.close();
}

void TreeModel::loadFromFile(QString filename)
{
    if (filename.isEmpty())
        return;
        //filename = QFileDialog::getOpenFileName(0,"Load tree structure","","(*.structure)");

    QFile inputFile;
    inputFile.setFileName(filename);
    inputFile.open(QIODevice::ReadOnly);
    QString importData = inputFile.readAll();
    setModelDataString(importData);
    inputFile.close();
}


bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TreeModel::setDataSpecial(const QModelIndex &index, const QVariant &value, int column)
{
    TreeItem *item = getItem(index);
    bool result = item->setData(column, value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    //checkup

    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}


TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}



bool TreeModel::printTopValues()
{
    for (int i = 0; i < rootItem->childCount(); ++i)
    {
        TreeItem *currentItem = rootItem->child(i);
        QString fullLine;

        for (int j = 0; j < currentItem->columnCount(); ++j)
            fullLine += " [" + QString::number(j) + "] " + currentItem->data(j).toString();

        qDebug() <<fullLine;
    }
    return true;
}

bool TreeModel::addTopItem(const QModelIndex &index)
{
    Q_UNUSED(index);

    QVector<QVariant> handData;
    handData << newEntryTemplate << " " << " "; //position?

    TreeItem *item = rootItem;
    TreeItem *handMade = new TreeItem(handData, columnNames.size(),item);

    beginInsertRows(QModelIndex(),item->childCount(),item->childCount());
    item->appendChild(handMade);
    endInsertRows();

    return true;
}

bool TreeModel::addChildItem(const QModelIndex &index)
{
    QVector<QVariant> handData;
    handData << newEntryTemplate << " " << " "; //position?

    TreeItem *item = getItem(index);
    TreeItem *handMade = new TreeItem(handData, columnNames.size(), item);

    beginInsertRows(index,item->childCount(),item->childCount());
    item->appendChild(handMade);
    endInsertRows();

    return true;
}

bool TreeModel::setItemText(const QModelIndex &index, const QString text, int column)
{
    setDataSpecial(index,text,column);

    return true;
}

bool TreeModel::removeItem(const QModelIndex &index)
{
    if (index == QModelIndex())
        return false;

    removeRow(index.row(),index.parent());
    return true;
}
