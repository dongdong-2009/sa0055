#ifndef UTIL_H
#define UTIL_H
#include "datatype.h"
#include "selectpoint.h"
#include "maptablemanager.h"

class Util
{
public:
    static DataType selectPointType_2_dataType(SelectPoint::Types pointTypes, bool *onlyOneType = 0);
    static SelectPoint::Type dataType_2_selectPointType(DataType dataType);

    static MapTableManager::TableIndex dataType_2_mapTableIndex(DataType type);
    static DataType integer_2_dataType(int val);

    static QString formulaVarName(int calcVarSeqNum);
    static MapTableManager::TableIndex integer_2_mapTableIndex(int val);
};

#endif // UTIL_H
