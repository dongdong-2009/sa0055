#include "alarminfotablemanager.h"
#include "logutil.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

AlarmInfoTableManager *AlarmInfoTableManager::_alarmInfoTableManager = 0;
QString AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::MaxColumnNum] = {
    "序号",       "报警规则",     "装置名称",     "点类型",     "系统点",       "遥测上限",         "遥测下限",     "遥测上上限",
    "遥测下下限", "是否需要确认", "未确认时颜色", "确认后颜色", "报警声音路径", "报警声音播放次数", "是否打印报警", "画面文件路径"};

const QString tableName = "config_alarm_info";

AlarmInfoTableManager *AlarmInfoTableManager::alarmInfoTableManager()
{
    if (!_alarmInfoTableManager)
    {
        _alarmInfoTableManager = new AlarmInfoTableManager();
    }

    return _alarmInfoTableManager;
}

void AlarmInfoTableManager::refreshData()
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

AlarmInfoTableManager::AlarmInfoTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable(tableName);
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
    refreshData();
}

void AlarmInfoTableManager::addNewRecord(int alarmRuleId, QList<SelectPoint> selectPointList)
{
    if (selectPointList.size() == 0)
    {
        return;
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

    for (int i = 0; i < selectPointList.size(); i++)
    {
        SelectPoint point = selectPointList.at(i);
        sqlStr = QString("INSERT INTO %1 (alarm_rule_id, device_id, point_type, point_desc_id) VALUES(%2, %3, %4, %5)")
                     .arg(tableName)
                     .arg(alarmRuleId)
                     .arg(point.getDeviceId())
                     .arg(point.getType())
                     .arg(point.getPointDescId());

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

    refreshData();
}

void AlarmInfoTableManager::fetchData(int alarmRuleId)
{
    _model->setFilter(QString("alarm_rule_id=%1").arg(alarmRuleId));
    refreshData();
}

void AlarmInfoTableManager::deleteRecord(int alarmRuleId)
{
    fetchData(alarmRuleId);

    if(_model->rowCount() > 0)
    {
        if (!_model->removeRows(0, _model->rowCount()))
        {
            ERROR << QString("call %1 fail:%2").arg("removeRows").arg(model()->lastError().text());
        }

        saveData();
    }
}

void AlarmInfoTableManager::deleteRecord(int row, int count)
{
    if (!_model->removeRows(row, count))
    {
        ERROR << QString("call %1 fail:%2").arg("removeRows").arg(model()->lastError().text());
    }

    saveData();
}

void AlarmInfoTableManager::setColumnValue(QList<int> rowList, int column, QVariant value)
{
    foreach (int row, rowList)
    {
        if (!_model->setData(_model->index(row, column), value))
        {
            ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
        }
    }

    saveData();
}

void AlarmInfoTableManager::saveData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(model()->lastError().text());
    }

    refreshData();
}
