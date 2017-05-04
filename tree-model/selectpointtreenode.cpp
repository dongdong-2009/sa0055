#include "selectpointtreenode.h"

SelectPointTreeNode::SelectPointTreeNode() : TreeNode(0)
{
    _nodeType = NodeType::Root;
}

SelectPointTreeNode::SelectPointTreeNode(SelectPointTreeNode *parentItem, int deviceId) : TreeNode(parentItem)
{
    _nodeType = NodeType::Device;
    _deviceId = deviceId;
}

SelectPointTreeNode::SelectPointTreeNode(SelectPointTreeNode *parentItem, int deviceId, DataType dataType) : TreeNode(parentItem)
{
    _nodeType = NodeType::PointType;
    _deviceId = deviceId;
    _dataType = dataType;
}

SelectPointTreeNode::SelectPointTreeNode(SelectPointTreeNode *parentItem, int deviceId, DataType dataType, int pointDescId) : TreeNode(parentItem)
{
    _nodeType = NodeType::Point;
    _deviceId = deviceId;
    _dataType = dataType;
    _pointDescId = pointDescId;
}

SelectPointTreeNode::~SelectPointTreeNode()
{
}

QString SelectPointTreeNode::text()
{
    switch (nodeType())
    {
    case NodeType::Root:
        break;

    case NodeType::Device:
        if (deviceId() == CalcVarPseudoDeviceId)
        {
            return CalcVarStr;
        }
        else
        {
            return MapTableManager::mapTableManager(MapTableManager::TableIndex::DEV)->nameFromId(deviceId());
        }

    case NodeType::PointType:
        return EnumTableManager::dataTypeEnumManager()->nameFromId(dataType());

    case NodeType::Point:

        if (deviceId() == CalcVarPseudoDeviceId)
        {
            return MapTableManager::mapTableManager(MapTableManager::TableIndex::CALCVAR)->nameFromId(pointDescId());
        }
        else
        {
            return MapTableManager::mapTableManager(Util::dataType_2_mapTableIndex(dataType()))->nameFromId(pointDescId());
        }
    }

    Q_UNREACHABLE();
}

QString SelectPointTreeNode::icon()
{
    return "N/A";
}
