#include "util.h"

SelectPoint::Type Util::dataType_2_selectPointType(DataType dataType)
{
    SelectPoint::Type result;
    switch (dataType)
    {
    case DataType::DI:
        result = SelectPoint::Type::DI;
        break;
    case DataType::AI:
        result = SelectPoint::Type::AI;
        break;
    case DataType::DO:
        result = SelectPoint::Type::DO;
        break;
    case DataType::AO:
        result = SelectPoint::Type::AO;
        break;
    case DataType::CI:
        result = SelectPoint::Type::CI;
        break;
    default:
        Q_UNREACHABLE();
    }

    return result;
}

DataType Util::selectPointType_2_dataType(SelectPoint::Types pointTypes, bool *onlyOneType)
{
    DataType result = DataType::DataTypeNum;
    int num = 0;

    if (pointTypes.testFlag(SelectPoint::Type::DI))
    {
        num++;
        result = DataType::DI;
    }

    if (pointTypes.testFlag(SelectPoint::Type::AI))
    {
        num++;
        result = DataType::AI;
    }

    if (pointTypes.testFlag(SelectPoint::Type::DO))
    {
        num++;
        result = DataType::DO;
    }

    if (pointTypes.testFlag(SelectPoint::Type::AO))
    {
        num++;
        result = DataType::AO;
    }

    if (pointTypes.testFlag(SelectPoint::Type::CI))
    {
        num++;
        result = DataType::CI;
    }

    Q_ASSERT(result != DataType::DataTypeNum);

    if (onlyOneType)
    {
        *onlyOneType = num == 1;
    }

    return result;
}

MapTableManager::TableIndex Util::dataType_2_mapTableIndex(DataType type)
{
    MapTableManager::TableIndex result = MapTableManager::TableIndex::MAX;
    switch (type)
    {
    case DataType::DI:
        result = MapTableManager::TableIndex::DI;
        break;
    case DataType::AI:
        result = MapTableManager::TableIndex::AI;
        break;
    case DataType::DO:
        result = MapTableManager::TableIndex::DO;
        break;
    case DataType::AO:
        result = MapTableManager::TableIndex::AO;
        break;
    case DataType::CI:
        result = MapTableManager::TableIndex::CI;
        break;
    default:
        Q_UNREACHABLE();
    }

    return result;
}

DataType Util::integer_2_dataType(int val)
{
    Q_ASSERT(val >= DataType::DI && val <= DataType::CI);
    return DataType(val);
}

QString Util::formulaVarName(int calcVarSeqNum)
{
    return QString("V%1").arg(calcVarSeqNum, 2, 10, QChar('0'));
}

MapTableManager::TableIndex Util::integer_2_mapTableIndex(int val)
{
    Q_ASSERT(val >= MapTableManager::TableIndex::DI && val < MapTableManager::TableIndex::MAX);
    return MapTableManager::TableIndex(val);
}
