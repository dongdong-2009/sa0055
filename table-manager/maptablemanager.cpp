#include "maptablemanager.h"
#include "constvalue.h"
#include "logutil.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

MapTableManager::MapTableManager(QString mapTable, QString mapId, QString mapName, QString mapValid, QObject *parent)
    : AbstractMapTableManager(mapTable, mapId, mapName, parent)
{
    model()->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
    mapValidColumnIndex = model()->fieldIndex(mapValid);

    _currentMaxId = Desc_ID_Start;
}

void MapTableManager::copyModel2Map()
{
    refresh();

    bool ok;
    for (int i = 0; i < model()->rowCount(); i++)
    {
        int id = model()->index(i, idColumnIndex()).data().toInt(&ok);
        Q_ASSERT(ok);

        QString name = model()->index(i, nameColumnIndex()).data().toString();
        Q_ASSERT(ok);

        bool valid = model()->index(i, idColumnIndex()).data().toInt(&ok) == 0 ? false : true;
        Q_ASSERT(ok);

        MapRecord *pRecord = new MapRecord(id, name, valid);
        id4MapRecord.insert(id, pRecord);
        name4MapRecord.insert(name, pRecord);
        if (id > _currentMaxId)
        {
            _currentMaxId = id;
        }
    }
}

void MapTableManager::copyMap2Model()
{
    QSqlQuery query;
    QString sqlStr = "BEGIN;";

    if (!query.prepare(sqlStr))
    {
        ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
    }

    if (!query.exec())
    {
        ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
    }

    sqlStr = QString("DELETE FROM %1 ;").arg(tableName());
    if (!query.prepare(sqlStr))
    {
        ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
    }

    if (!query.exec())
    {
        ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
    }

    foreach (MapRecord *pRecord, id4MapRecord)
    {
        sqlStr = QString("INSERT INTO %1 VALUES(%2, '%3', %4);").arg(tableName()).arg(pRecord->id()).arg(pRecord->name()).arg(pRecord->valid() ? 1 : 0);
        if (!query.prepare(sqlStr))
        {
            ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
        }

        if (!query.exec())
        {
            ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
        }
    }

    sqlStr = "COMMIT;";
    if (!query.prepare(sqlStr))
    {
        ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
    }

    if (!query.exec())
    {
        ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
    }

    if(!model()->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(model()->lastError().text());
    }
}

int MapTableManager::idFromName(QString name) const
{
    int result = -1;
    if (name4MapRecord.contains(name))
    {
        MapRecord *pRecord = name4MapRecord.value(name);
        if (pRecord->valid())
        {
            result = pRecord->id();
        }
    }
    return result;
}

QString MapTableManager::nameFromId(int id) const
{
    QString result = "";
    if (id4MapRecord.contains(id))
    {
        MapRecord *pRecord = id4MapRecord.value(id);
        if (pRecord->valid())
        {
            result = pRecord->name();
        }
    }
    return result;
}

int MapTableManager::modifyDesc(int oldId, QString oldDesc, QString newDesc)
{
    if (oldDesc == newDesc)
    {
        return oldId;
    }
    else if (newDesc.isEmpty())
    {
        return Error::EmptyDescr;
    }

    if (name4MapRecord.contains(newDesc))
    {
        MapRecord *pNewRecord = name4MapRecord.value(newDesc);
        if (pNewRecord->valid())
        {
            return Error::AlreadyExist;
        }
        else
        {
            name4MapRecord.remove(newDesc);
            id4MapRecord.remove(pNewRecord->id());
            delete pNewRecord;
        }
    }

    if (id4MapRecord.contains(oldId))
    {
        name4MapRecord.remove(oldDesc);
        id4MapRecord.remove(oldId);
        delete id4MapRecord.value(oldId);
    }
    else
    {
        Q_UNREACHABLE();
    }

    MapRecord *pRecord = new MapRecord(oldId, newDesc, true);
    id4MapRecord.insert(oldId, pRecord);
    name4MapRecord.insert(newDesc, pRecord);

    return oldId;
}

int MapTableManager::currentMaxId()
{
    return _currentMaxId;
}

int MapTableManager::addDesc(QString newDesc)
{
    int newId = -1;
    if (newDesc.isEmpty())
    {
        return Error::EmptyDescr;
    }

    if (name4MapRecord.contains(newDesc))
    {
        MapRecord *pNewRecord = name4MapRecord.value(newDesc);
        if (pNewRecord->valid())
        {
            return Error::AlreadyExist;
        }
        else
        {
            pNewRecord->setValid(true);
            newId = pNewRecord->id();
        }
    }
    else
    {
        newId = ++_currentMaxId;
        MapRecord *pAddRecord = new MapRecord(newId, newDesc, true);
        id4MapRecord.insert(newId, pAddRecord);
        name4MapRecord.insert(newDesc, pAddRecord);
    }

    return newId;
}

void MapTableManager::deleteDesc(int oldId)
{
    if (id4MapRecord.contains(oldId))
    {
        MapRecord *pOldRecord = id4MapRecord.value(oldId);
        pOldRecord->setValid(false);
    }
    else
    {
        Q_UNREACHABLE();
    }
}

void MapTableManager::deleteDesc(QList<int> oldIdList)
{
    for (int k = 0; k < oldIdList.size(); k++)
    {
        deleteDesc(oldIdList.at(k));
    }
}
