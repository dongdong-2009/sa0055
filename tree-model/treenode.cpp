#include "treenode.h"

TreeNode::TreeNode(TreeNode *parentItem) : _parentItem(parentItem)
{
    if(_parentItem)
    {
        _parentItem->_appendChild(this);
    }
}

TreeNode::~TreeNode()
{
    qDeleteAll(_childItems);
}

void TreeNode::_appendChild(TreeNode *item)
{
    _childItems.append(item);
}

void TreeNode::_removeChild(int i)
{
    TreeNode *node = child(i);
    _childItems.removeAt(i);

    delete node;
}

TreeNode *TreeNode::child(int row)
{
    return _childItems.value(row);
}

int TreeNode::childCount() const
{
    return _childItems.count();
}

TreeNode *TreeNode::parentItem()
{
    return _parentItem;
}

int TreeNode::row() const
{
    if (_parentItem)
    {
        return _parentItem->_childItems.indexOf(const_cast<TreeNode *>(this));
    }
    else
    {
        return 0;
    }
}
