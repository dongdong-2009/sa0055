#include "treemodel.h"
#include "selectpointtreenode.h"
#include "projecttreenode.h"

#include <QIcon>

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent), _rootItem(0)
{
}

void TreeModel::init2SelectPoint(SelectPoint::Types types, SelectPoint::Source pointSource, bool specifiedDeviceFlag,
                                       int specifiedDeviceId)
{
    _rootItem = SelectPointTreeNode::initSelectPointTree(types, pointSource, specifiedDeviceFlag, specifiedDeviceId);
}

void TreeModel::init2ProjectTree()
{
    _rootItem = ProjectTreeNode::init2ProjectTreeStructure();
    connect(static_cast<ProjectTreeNode*>(_rootItem), &ProjectTreeNode::beginModifyTree, [=](){
        beginResetModel();
    });

    connect(static_cast<ProjectTreeNode*>(_rootItem), &ProjectTreeNode::endModifyTree, [=](){
        endResetModel();
    });
}

TreeModel::~TreeModel()
{
    delete _rootItem;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == 0 && orientation == Qt::Orientation::Horizontal && role == Qt::ItemDataRole::DisplayRole)
    {
        return "列表";
    }

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    TreeNode *parentItem;

    if (!parent.isValid())
    {
        parentItem = _rootItem;
    }
    else
    {
        parentItem = static_cast<TreeNode *>(parent.internalPointer());
    }

    if (row >= 0 && row < parentItem->childCount())
    {
        return createIndex(row, column, parentItem->child(row));
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    TreeNode *item = static_cast<TreeNode *>(index.internalPointer());
    TreeNode *parentItem = item->parentItem();

    if (parentItem == _rootItem)
    {
        return QModelIndex();
    }
    else
    {
        return createIndex(parentItem->row(), 0, parentItem);
    }
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return _rootItem->childCount();
    }
    else
    {
        TreeNode *parentItem = static_cast<TreeNode *>(parent.internalPointer());
        return parentItem->childCount();
    }
}

int TreeModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::ItemDataRole::DisplayRole)
    {
        TreeNode *item = static_cast<TreeNode *>(index.internalPointer());
        return item->text();
    }
    else if(role == Qt::ItemDataRole::DecorationRole)
    {
        TreeNode *item = static_cast<TreeNode *>(index.internalPointer());
        return QIcon(item->icon());
    }
    else
    {
        return QVariant();
    }
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
    {
        TreeNode *item = static_cast<TreeNode *>(index.internalPointer());
        if (item->isSelectable())
        {
            return QAbstractItemModel::flags(index) | Qt::ItemFlag::ItemIsSelectable;
        }
        else
        {
            return QAbstractItemModel::flags(index) & ~Qt::ItemFlag::ItemIsSelectable;
        }
    }

    return QAbstractItemModel::flags(index);
}
