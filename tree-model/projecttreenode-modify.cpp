#include "projecttreenode.h"
#include "mainwindow.h"

void ProjectTreeNode::addNewDeviceNode(int deviceId, bool bInit)
{
    QMap<QString, QString> map;
    map.insert(ID, QString::number(deviceId));
    map.insert(ICON, "device.png");
    map.insert(WIDGET, QString::number(MainWindow::WidgetIndex::DevicePara));
    map.insert(TYPE, QString::number(TreeNodeType::DeviceNode));

    if(bInit)
    {
        new ProjectTreeNode(map, deviceListNode);
    }
    else
    {
        emit rootNode->beginModifyTree();
        new ProjectTreeNode(map, deviceListNode);
        emit rootNode->endModifyTree();
    }
}

void ProjectTreeNode::deleteDeviceNode(int deviceId)
{
    int index = -1;
    for(int i=0; i<deviceListNode->childCount(); i++)
    {
        ProjectTreeNode *devNode = static_cast<ProjectTreeNode *>(deviceListNode->child(i));
        if(devNode->deviceId() == deviceId)
        {
            index = i;
            break;
        }
    }

    if(index >= 0)
    {
        emit rootNode->beginModifyTree();
        deviceListNode->_removeChild(index);
        emit rootNode->endModifyTree();
    }
}

void ProjectTreeNode::addNewFormulaNode(int formulaId, bool bInit)
{
    QMap<QString, QString> map;
    map.insert(ID, QString::number(formulaId));
    map.insert(ICON, "formula.png");
    map.insert(WIDGET, QString::number(MainWindow::WidgetIndex::CommonFormula));
    map.insert(TYPE, QString::number(TreeNodeType::FormulaNode));

    if(bInit)
    {
        new ProjectTreeNode(map, formulaListNode);
    }
    else
    {
        emit rootNode->beginModifyTree();
        new ProjectTreeNode(map, formulaListNode);
        emit rootNode->endModifyTree();
    }
}

void ProjectTreeNode::deleteFormulaNode(int formulaId)
{
    int index = -1;
    for(int i=0; i<formulaListNode->childCount(); i++)
    {
        ProjectTreeNode *formulaNode = static_cast<ProjectTreeNode *>(formulaListNode->child(i));
        if(formulaNode->formulaId() == formulaId)
        {
            index = i;
            break;
        }
    }

    if(index >= 0)
    {
        emit rootNode->beginModifyTree();
        formulaListNode->_removeChild(index);
        emit rootNode->endModifyTree();
    }
}

void ProjectTreeNode::addNewCalcVarNode(int calcVarId, bool bInit)
{
    QMap<QString, QString> map;
    map.insert(ID, QString::number(calcVarId));
    map.insert(ICON, "calcvar.png");
    map.insert(WIDGET, QString::number(MainWindow::WidgetIndex::CalcVar));
    map.insert(TYPE, QString::number(TreeNodeType::CalcVarNode));

    if(bInit)
    {
        new ProjectTreeNode(map, calcVarListNode);
    }
    else
    {
        emit rootNode->beginModifyTree();
        new ProjectTreeNode(map, calcVarListNode);
        emit rootNode->endModifyTree();
    }
}

void ProjectTreeNode::deleteCalcVarNode(int calcVarId)
{
    int index = -1;
    for(int i=0; i<calcVarListNode->childCount(); i++)
    {
        ProjectTreeNode *calcVarNode = static_cast<ProjectTreeNode *>(calcVarListNode->child(i));
        if(calcVarNode->calcVarId() == calcVarId)
        {
            index = i;
            break;
        }
    }

    if(index >= 0)
    {
        emit rootNode->beginModifyTree();
        calcVarListNode->_removeChild(index);
        emit rootNode->endModifyTree();
    }
}

void ProjectTreeNode::addNewAlarmRuleNode(int alarmRuleId, bool bInit)
{
    QMap<QString, QString> map;
    map.insert(ID, QString::number(alarmRuleId));
    map.insert(ICON, "alarmrule.png");
    map.insert(WIDGET, QString::number(MainWindow::WidgetIndex::AlarmRule));
    map.insert(TYPE, QString::number(TreeNodeType::AlarmRuleNode));

    if(bInit)
    {
        new ProjectTreeNode(map, alarmRuleListNode);
    }
    else
    {
        emit rootNode->beginModifyTree();
        new ProjectTreeNode(map, alarmRuleListNode);
        emit rootNode->endModifyTree();
    }
}

void ProjectTreeNode::deleteAlarmRuleNode(int alarmRuleId)
{
    int index = -1;
    for(int i=0; i<alarmRuleListNode->childCount(); i++)
    {
        ProjectTreeNode *alarmRuleNode = static_cast<ProjectTreeNode *>(alarmRuleListNode->child(i));
        if(alarmRuleNode->alarmRuleId() == alarmRuleId)
        {
            index = i;
            break;
        }
    }

    if(index >= 0)
    {
        emit rootNode->beginModifyTree();
        alarmRuleListNode->_removeChild(index);
        emit rootNode->endModifyTree();
    }
}
