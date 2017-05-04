#include "calcvarbindtablemanager.h"

#include "logutil.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
const QString CalcVarBindTableName = "config_calc_var_bind";

CalcVarBindTableManager *CalcVarBindTableManager::_calcVarBindTableManager = 0;
CalcVarBindTableManager *CalcVarBindTableManager::calcVarBindTableManager()
{
    if (!_calcVarBindTableManager)
    {
        _calcVarBindTableManager = new CalcVarBindTableManager();
    }
    return _calcVarBindTableManager;
}

CalcVarBindTableManager::CalcVarBindTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable(CalcVarBindTableName);
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
    refreshData();
}

void CalcVarBindTableManager::addNewRecord(int calcVarId, int varSeqNum, int deviceId, int pointType, int pointDescId)
{
    int rowCount = _model->rowCount();
    if (!_model->insertRows(rowCount, 1))
    {
        ERROR << QString("call %1 fail:%2").arg("insertRows").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::CalcVarId), calcVarId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::VarSeqNum), varSeqNum))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::DeviceId), deviceId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::PointType), pointType))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::PointDescId), pointDescId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    submitData();
}

void CalcVarBindTableManager::updateRecord(int calcVarId, int varSeqNum, int deviceId, int pointType, int pointDescId)
{
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int tempCalcVarId = _model->index(i, ColumnIndex::CalcVarId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        int tempVarSeqNum = _model->index(i, ColumnIndex::VarSeqNum).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        if (tempCalcVarId == calcVarId && tempVarSeqNum == varSeqNum)
        {
            if (!_model->setData(_model->index(i, ColumnIndex::DeviceId), deviceId))
            {
                ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
            }

            if (!_model->setData(_model->index(i, ColumnIndex::PointType), pointType))
            {
                ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
            }

            if (!_model->setData(_model->index(i, ColumnIndex::PointDescId), pointDescId))
            {
                ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
            }

            submitData();
            return;
        }
    }

    addNewRecord(calcVarId, varSeqNum, deviceId, pointType, pointDescId);
}

void CalcVarBindTableManager::deleteRecord(int calcVarId)
{
    QSqlQuery query;
    QString sqlStr = QString("DELETE FROM %1 WHERE calc_id = %2").arg(CalcVarBindTableName).arg(calcVarId);

    if (!query.prepare(sqlStr))
    {
        ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
    }

    if (!query.exec())
    {
        ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
    }

    refreshData();
}

void CalcVarBindTableManager::fetchData(int calcVarId)
{
    _model->setFilter(QString("calc_id=%1").arg(calcVarId));
    _model->setSort(ColumnIndex::VarSeqNum, Qt::SortOrder::AscendingOrder);
    refreshData();
}

void CalcVarBindTableManager::refreshData()
{
    if (!_model->select())
    {
        ERROR << QString("call %1 fail:%2").arg("select").arg(model()->lastError().text());
    }

    while (_model->canFetchMore())
    {
        _model->fetchMore();
    }
}

void CalcVarBindTableManager::submitData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(model()->lastError().text());
    }

    refreshData();
}
