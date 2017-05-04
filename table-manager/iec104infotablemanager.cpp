#include "iec104infotablemanager.h"
#include "logutil.h"
#include "maptablemanager.h"
#include "util.h"
#include <QApplication>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <infotableimportexport.h>

Iec104InfoTableManager *Iec104InfoTableManager::_infoTableManager[DataType::DataTypeNum] = {0, 0, 0, 0, 0};

const QString InfoTableName[] = {"config_di_point", "config_ai_point", "config_do_point", "config_ao_point", "config_ci_point"};

Iec104InfoTableManager *Iec104InfoTableManager::infoTableManager(DataType type)
{
    if (!_infoTableManager[type])
    {
        _infoTableManager[type] = new Iec104InfoTableManager(type);
    }
    return _infoTableManager[type];
}

Iec104InfoTableManager::Iec104InfoTableManager(DataType type, QObject *parent) : type(type)
{
    _model = new QSqlTableModel(parent);
    _model->setTable(InfoTableName[type]);
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
}

Iec104InfoTableManager::~Iec104InfoTableManager()
{
}

QSqlTableModel *Iec104InfoTableManager::model()
{
    return _model;
}

void Iec104InfoTableManager::fetchData(int deviceId)
{
    _model->setFilter(QString("device_id=%1").arg(deviceId));
    _refreshData();
}

void Iec104InfoTableManager::_refreshData()
{
    if (!_model->select())
    {
        ERROR << QString("call %1 fail:%2").arg("select").arg(_model->lastError().text());
    }

    while (_model->canFetchMore())
    {
        _model->fetchMore();
    }
}

void Iec104InfoTableManager::submitData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(_model->lastError().text());
    }

    _refreshData();
}

void Iec104InfoTableManager::addRecord(int deviceId, int descId)
{
    int row = _model->rowCount();
    if (!_model->insertRows(row, 1))
    {
        ERROR << QString("call %1 fail:%2").arg("insertRows").arg(_model->lastError().text());
    }

    if (!_model->setData(_model->index(row, Iec104InfoTableManager::ColumnIndex::DeviceId), deviceId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(_model->lastError().text());
    }

    if (!_model->setData(_model->index(row, Iec104InfoTableManager::ColumnIndex::DescrId), descId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(_model->lastError().text());
    }

    submitData();
}

void Iec104InfoTableManager::addRecordBatch(int deviceId, QList<int> descIdList, QList<InfoTableImportExport::PointInfo> pointList)
{
    QVariantList aiMulList;
    QVariantList aiOffsetList;
    QVariantList doQualList;
    QVariantList doTypeList;

    QVariantList deviceIdList;
    QVariantList pointDescIdList;
    foreach (int descId, descIdList)
    {
        deviceIdList << deviceId;
        pointDescIdList << descId;
    }

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

    switch (type)
    {
    case DataType::DI:
        sqlStr = QString("INSERT INTO %1 (device_id, di_id) VALUES (?, ?)").arg(InfoTableName[type]);
        break;
    case DataType::AI:
        sqlStr = QString("INSERT INTO %1 (device_id, ai_id, multiple, offset) VALUES (?, ?, ?, ?)").arg(InfoTableName[type]);
        foreach (InfoTableImportExport::PointInfo pi, pointList)
        {
            aiMulList << pi.getAiMultipleFactor();
            aiOffsetList << pi.getAiOffsetFactor();
        }

        break;
    case DataType::DO:
        sqlStr = QString("INSERT INTO %1 (device_id, do_id, qualifier, do_type) VALUES (?, ?, ?, ?)").arg(InfoTableName[type]);
        foreach (InfoTableImportExport::PointInfo pi, pointList)
        {
            doQualList << pi.getDoQual();
            doTypeList << pi.getDoType();
        }

        break;
    case DataType::AO:
        sqlStr = QString("INSERT INTO %1 (device_id, ao_id) VALUES (?, ?)").arg(InfoTableName[type]);
        break;
    case DataType::CI:
        sqlStr = QString("INSERT INTO %1 (device_id, ci_id) VALUES (?, ?)").arg(InfoTableName[type]);
        break;
    case DataType::DataTypeNum:
        Q_UNREACHABLE();
    }

    if (!query.prepare(sqlStr))
    {
        ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
    }

    query.addBindValue(deviceIdList);
    query.addBindValue(pointDescIdList);

    if (type == DataType::AI)
    {
        query.addBindValue(aiMulList);
        query.addBindValue(aiOffsetList);
    }
    else if (type == DataType::DO)
    {
        query.addBindValue(doQualList);
        query.addBindValue(doTypeList);
    }

    if (!query.execBatch())
    {
        ERROR << QString("call %1(%2) fail: %3").arg("execBatch").arg(sqlStr).arg(query.lastError().text());
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

    fetchData(deviceId);
}

void Iec104InfoTableManager::deleteRecord(int deviceId, int row, int count)
{
    bool ok;
    QList<int> descIdList;
    for (int i = 0; i < count; i++)
    {
        int descId = _model->index(row + i, Iec104InfoTableManager::ColumnIndex::DescrId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }
        descIdList.append(descId);
    }

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

    sqlStr = QString("SELECT seq_num FROM %1 where device_id = %2;").arg(InfoTableName[type]).arg(deviceId);

    if (!query.prepare(sqlStr))
    {
        ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
    }

    if (!query.exec())
    {
        ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
    }

    QList<int> seqNumList;
    while (query.next())
    {
        seqNumList << query.value(0).toInt();
    }

    sqlStr = QString("DELETE FROM %1 WHERE seq_num IN (").arg(InfoTableName[type]);
    for (int i = 0; i < count && row + i < seqNumList.size(); i++)
    {
        sqlStr += QString::number(seqNumList.at(row + i));
        if(i < count - 1)
        {
            sqlStr += ",";
        }
        else
        {
            sqlStr += ")";
        }
    }

    if (!query.prepare(sqlStr))
    {
        ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
    }

    if (!query.exec())
    {
        ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
    }

    sqlStr = "COMMIT;";
    if (!query.prepare("COMMIT;"))
    {
        ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
    }

    if (!query.exec())
    {
        ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
    }

    fetchData(deviceId);

    MapTableManager::mapTableManager(Util::dataType_2_mapTableIndex(type))->deleteDesc(descIdList);
}

void Iec104InfoTableManager::deleteRecord(int deviceId)
{
    fetchData(deviceId);

    bool ok;
    QList<int> descIdList;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int descId = _model->index(i, Iec104InfoTableManager::ColumnIndex::DescrId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }
        descIdList.append(descId);
    }

    QSqlQuery query;
    QString sqlStr = QString("DELETE FROM %1 WHERE device_id=%2").arg(InfoTableName[type]).arg(deviceId);

    if (!query.prepare(sqlStr))
    {
        ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
    }

    if (!query.exec())
    {
        ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
    }

    fetchData(deviceId);

    MapTableManager::mapTableManager(Util::dataType_2_mapTableIndex(type))->deleteDesc(descIdList);
}
