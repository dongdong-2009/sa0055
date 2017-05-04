#include "enumtablemanager.h"
#include "logutil.h"
#include <QSqlTableModel>

EnumTableManager::EnumTableManager(QString mapTable, QString mapId, QString mapName, QObject *parent)
    : AbstractMapTableManager(mapTable, mapId, mapName, parent)
{
}
