#include "selectpointfilterproxymodel.h"

#include "selectpointtreenode.h"

SelectPointFilterProxyModel::SelectPointFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

bool SelectPointFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    SelectPointTreeNode *item =
        static_cast<SelectPointTreeNode *>(sourceModel()->index(source_row, filterKeyColumn(), source_parent).internalPointer());

    if(item->nodeType() == SelectPointTreeNode::NodeType::Point)
    {
        return item->text().contains(filterRegExp());
    }
    else
    {
        return true;
    }
}
