#ifndef SELECTPOINTTREEITEM_H
#define SELECTPOINTTREEITEM_H

#include "constvalue.h"
#include "datatype.h"
#include "enumtablemanager.h"
#include "maptablemanager.h"
#include "treenode.h"
#include "util.h"
#include <QList>
#include <QVariant>

class SelectPointTreeNode : public TreeNode
{
public:
    enum NodeType
    {
        Root,
        Device,
        PointType,
        Point
    };

    explicit SelectPointTreeNode();
    explicit SelectPointTreeNode(SelectPointTreeNode *parentItem, int deviceId);
    explicit SelectPointTreeNode(SelectPointTreeNode *parentItem, int deviceId, DataType dataType);
    explicit SelectPointTreeNode(SelectPointTreeNode *parentItem, int deviceId, DataType dataType, int pointDescId);

    ~SelectPointTreeNode();
    QString text();
    QString icon();
    bool isSelectable() { return nodeType() == SelectPointTreeNode::NodeType::Point; }

    NodeType nodeType() { return _nodeType; }

    int deviceId() { return _deviceId; }

    DataType dataType() { return _dataType; }

    int pointDescId() { return _pointDescId; }

    static SelectPointTreeNode *initSelectPointTree(SelectPoint::Types types, SelectPoint::Source pointSource, bool specifiedDeviceFlag,
                                                    int specifiedDeviceId);

private:
    NodeType _nodeType;
    int _deviceId;
    DataType _dataType;
    int _pointDescId;
};

#endif // SELECTPOINTTREEITEM_H
