#include "projecttreenode.h"
#include "maptablemanager.h"

ProjectTreeNode::ProjectTreeNode(QMap<QString, QString> map, ProjectTreeNode *parentItem):
    TreeNode(parentItem), _map(map)
{
}

ProjectTreeNode::~ProjectTreeNode()
{
}

QString ProjectTreeNode::text()
{
    switch (nodeType())
    {
    case TreeNodeType::DeviceNode:
        return MapTableManager::mapTableManager(MapTableManager::TableIndex::DEV)->nameFromId(deviceId());
    case TreeNodeType::FormulaNode:
        return MapTableManager::mapTableManager(MapTableManager::TableIndex::FORMULA)->nameFromId(formulaId());
    case TreeNodeType::CalcVarNode:
        return MapTableManager::mapTableManager(MapTableManager::TableIndex::CALCVAR)->nameFromId(calcVarId());
    case TreeNodeType::AlarmRuleNode:
        return MapTableManager::mapTableManager(MapTableManager::TableIndex::ALARMRULE)->nameFromId(alarmRuleId());
    default:
        return _map.value(TEXT, "N/A");
    }
}

QString ProjectTreeNode::icon()
{
    return QString(":/icon/icons/%1").arg(_map.value(ICON, "N/A"));
}

bool ProjectTreeNode::isSelectable()
{
    return _map.contains(WIDGET);
}

int ProjectTreeNode::widgetIndex()
{
    return _map.value(WIDGET, "-1").toInt();
}

int ProjectTreeNode::nodeType()
{
    return _map.value(TYPE, "-1").toInt();
}

int ProjectTreeNode::deviceId()
{
    if(nodeType() == TreeNodeType::DeviceNode)
    {
        return _map.value(ID, "-1").toInt();
    }
    else
    {
        return -1;
    }
}

int ProjectTreeNode::formulaId()
{
    if(nodeType() == TreeNodeType::FormulaNode)
    {
        return _map.value(ID, "-1").toInt();
    }
    else
    {
        return -1;
    }
}

int ProjectTreeNode::calcVarId()
{
    if(nodeType() == TreeNodeType::CalcVarNode)
    {
        return _map.value(ID, "-1").toInt();
    }
    else
    {
        return -1;
    }
}

int ProjectTreeNode::alarmRuleId()
{
    if(nodeType() == TreeNodeType::AlarmRuleNode)
    {
        return _map.value(ID, "-1").toInt();
    }
    else
    {
        return -1;
    }
}
