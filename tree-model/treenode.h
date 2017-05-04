#ifndef TREENODE_H
#define TREENODE_H

#include <QList>

class TreeNode
{
public:
    TreeNode(TreeNode *parentItem = 0);
    virtual ~TreeNode();

    virtual void _appendChild(TreeNode *item);
    virtual void _removeChild(int i);

    virtual QString text() = 0;
    virtual QString icon() = 0;
    virtual bool isSelectable() = 0;

    virtual TreeNode *child(int row);
    virtual int childCount() const;
    virtual TreeNode *parentItem();
    virtual int row() const;

private:
    QList<TreeNode *> _childItems;
    TreeNode *_parentItem;
};

#endif // TREENODE_H
