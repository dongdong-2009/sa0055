#include "selectpointtreenode.h"
#include "calcvartablemanager.h"
#include "constvalue.h"
#include "deviceparatablemanager.h"
#include "iec104infotablemanager.h"
#include "maptablemanager.h"
#include "util.h"

void _fillCalcVarNode(SelectPointTreeNode *dataTypeNode, DataType dataType)
{
    bool ok;
    CalcVarTableManager *mgr = CalcVarTableManager::calcVarTableManager();
    mgr->fetchData(dataType);

    for (int i = 0; i < mgr->model()->rowCount(); i++)
    {
        int calcVarId = mgr->model()->index(i, CalcVarTableManager::ColumnIndex::CalcVarId).data().toInt(&ok);
        Q_ASSERT(ok);
        new SelectPointTreeNode(dataTypeNode, dataTypeNode->deviceId(), dataType, calcVarId);
    }
}

void _fillInfoPointNode(SelectPointTreeNode *dataTypeNode, DataType dataType)
{
    bool ok;
    Iec104InfoTableManager *mgr = Iec104InfoTableManager::infoTableManager(dataType);
    mgr->fetchData(dataTypeNode->deviceId());

    for (int i = 0; i < mgr->model()->rowCount(); i++)
    {
        int descId = mgr->model()->index(i, Iec104InfoTableManager::ColumnIndex::DescrId).data().toInt(&ok);
        Q_ASSERT(ok);
        new SelectPointTreeNode(dataTypeNode, dataTypeNode->deviceId(), dataType, descId);
    }
}

void _fillDataTypeNode(SelectPoint::Types selectTypes, SelectPointTreeNode *deviceNode)
{
    SelectPointTreeNode *dataTypeNode;
    int deviceId = deviceNode->deviceId();

    if (deviceId == CalcVarPseudoDeviceId)
    {
        if (selectTypes.testFlag(SelectPoint::Type::DI))
        {
            dataTypeNode = new SelectPointTreeNode(deviceNode, deviceId, DataType::DI);
            _fillCalcVarNode(dataTypeNode, DataType::DI);
        }

        if (selectTypes.testFlag(SelectPoint::Type::AI))
        {
            dataTypeNode = new SelectPointTreeNode(deviceNode, deviceId, DataType::AI);
            _fillCalcVarNode(dataTypeNode, DataType::AI);
        }
    }
    else
    {
        if (selectTypes.testFlag(SelectPoint::Type::DI))
        {
            dataTypeNode = new SelectPointTreeNode(deviceNode, deviceId, DataType::DI);
            _fillInfoPointNode(dataTypeNode, DataType::DI);
        }

        if (selectTypes.testFlag(SelectPoint::Type::AI))
        {
            dataTypeNode = new SelectPointTreeNode(deviceNode, deviceId, DataType::AI);
            _fillInfoPointNode(dataTypeNode, DataType::AI);
        }

        if (selectTypes.testFlag(SelectPoint::Type::DO))
        {
            dataTypeNode = new SelectPointTreeNode(deviceNode, deviceId, DataType::DO);
            _fillInfoPointNode(dataTypeNode, DataType::DO);
        }

        if (selectTypes.testFlag(SelectPoint::Type::AO))
        {
            dataTypeNode = new SelectPointTreeNode(deviceNode, deviceId, DataType::AI);
            _fillInfoPointNode(dataTypeNode, DataType::AO);
        }

        if (selectTypes.testFlag(SelectPoint::Type::CI))
        {
            dataTypeNode = new SelectPointTreeNode(deviceNode, deviceId, DataType::CI);
            _fillInfoPointNode(dataTypeNode, DataType::CI);
        }
    }
}

void _fillDeviceNode(SelectPointTreeNode *rootNode, SelectPoint::Types selectTypes, SelectPoint::Source pointSource, bool specifiedDeviceFlag,
                     int specifiedDeviceId)
{
    SelectPointTreeNode *devNode;

    if (specifiedDeviceFlag)
    {
        devNode = new SelectPointTreeNode(rootNode, specifiedDeviceId);
        _fillDataTypeNode(selectTypes, devNode);
    }
    else
    {
        foreach (int deviceId, DeviceParaTableManager::deviceParaTableManager()->getDeviceIdList())
        {
            devNode = new SelectPointTreeNode(rootNode, deviceId);
            _fillDataTypeNode(selectTypes, devNode);
        }
    }

    if (pointSource == SelectPoint::Source::All)
    {
        devNode = new SelectPointTreeNode(rootNode, CalcVarPseudoDeviceId);
        _fillDataTypeNode(selectTypes, devNode);
    }
}

SelectPointTreeNode *SelectPointTreeNode::initSelectPointTree(SelectPoint::Types types, SelectPoint::Source pointSource, bool specifiedDeviceFlag, int specifiedDeviceId)
{
    SelectPointTreeNode *_rootItem = new SelectPointTreeNode();
    _fillDeviceNode(_rootItem, types, pointSource, specifiedDeviceFlag, specifiedDeviceId);
    return _rootItem;
}
