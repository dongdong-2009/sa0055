#include "maptablemanager.h"
#include "util.h"

MapTableManager *MapTableManager::_mapTableManager[TableIndex::MAX] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const QString MapTableNames[] = {"config_di_map",       "config_ai_map",     "config_do_map",        "config_ao_map",
                                 "config_ci_map",       "config_device_map", "config_formula_map",   "config_integral_map",
                                 "config_calc_var_map", "config_curve_map",  "config_alarm_rule_map"};

MapTableManager *MapTableManager::mapTableManager(TableIndex index)
{
    if (!_mapTableManager[index])
    {
        _mapTableManager[index] = new MapTableManager(MapTableNames[index]);
    }

    return _mapTableManager[index];
}


void MapTableManager::CopyMap2Model()
{
    for (int i = TableIndex::DI; i < TableIndex::MAX; i++)
    {
        TableIndex tableIndex = Util::integer_2_mapTableIndex(i);
        MapTableManager::mapTableManager(tableIndex)->copyMap2Model();
    }
}

void MapTableManager::CopyModel2Map()
{
    for (int i = TableIndex::DI; i < TableIndex::MAX; i++)
    {
        TableIndex tableIndex = Util::integer_2_mapTableIndex(i);
        MapTableManager::mapTableManager(tableIndex)->copyModel2Map();
    }
}
