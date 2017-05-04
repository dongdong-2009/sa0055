#ifndef SELECTPOINT_CPP
#define SELECTPOINT_CPP
#include "selectpoint.h"
#include "maptablemanager.h"
#include "util.h"

#endif // SELECTPOINT_CPP

QString SelectPoint::getDescInfo(int deviceId, int pointType, int pointDescId, QString *pDeviceDesc, QString *pPointDesc)
{
    QString deviceDesc;
    QString pointDesc;

    if (deviceId == CalcVarPseudoDeviceId)
    {
        deviceDesc = CalcVarStr;
        pointDesc = MapTableManager::mapTableManager(MapTableManager::CALCVAR)->nameFromId(pointDescId);
    }
    else
    {
        deviceDesc = MapTableManager::mapTableManager(MapTableManager::TableIndex::DEV)->nameFromId(deviceId);
        if(pointType >= DataType::DI && pointType < DataType::DataTypeNum)
        {
            pointDesc = MapTableManager::mapTableManager(Util::integer_2_mapTableIndex(pointType))->nameFromId(pointDescId);
        }
    }

    if (pDeviceDesc)
    {
        *pDeviceDesc = deviceDesc;
    }

    if (pPointDesc)
    {
        *pPointDesc = pointDesc;
    }

    if(deviceDesc.isEmpty() || pointDesc.isEmpty())
    {
        return "";
    }
    else
    {
        return DevicePointDescFormat.arg(deviceDesc).arg(pointDesc);
    }
}

