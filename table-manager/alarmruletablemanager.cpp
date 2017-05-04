#include "alarmruletablemanager.h"
#include "logutil.h"
#include <QSqlError>
#include <QSqlTableModel>

AlarmRuleTableManager *AlarmRuleTableManager::_alarmRuleTableManager = 0;

AlarmRuleTableManager *AlarmRuleTableManager::alarmRuleTableManager()
{
    if (!_alarmRuleTableManager)
    {
        _alarmRuleTableManager = new AlarmRuleTableManager();
    }
    return _alarmRuleTableManager;
}

AlarmRuleTableManager::AlarmRuleTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable("config_alarm_rule");
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
    refreshData();
}

void AlarmRuleTableManager::refreshData()
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

void AlarmRuleTableManager::submitData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(model()->lastError().text());
    }

    refreshData();
}

void AlarmRuleTableManager::getAlarmRuleInfo(int alarmRuleId, int *alarmCauseId, int *alarmSubCauseValue, int *row)
{
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int id = _model->index(i, ColumnIndex::AlarmRuleId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        if (id == alarmRuleId)
        {
            if(alarmCauseId)
            {
                *alarmCauseId = _model->index(i, ColumnIndex::AlarmCauseId).data().toInt(&ok);
                Q_ASSERT(ok);
            }

            if(alarmSubCauseValue)
            {
                *alarmSubCauseValue = _model->index(i, ColumnIndex::AlarmSubCauseValue).data().toInt(&ok);
                Q_ASSERT(ok);
            }

            if(row)
            {
                *row = i;
            }

            return;
        }
    }

    Q_ASSERT(false);
}

void AlarmRuleTableManager::setAlarmSubCause(int alarmRuleId, int alarmSubCause)
{
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int tempId = _model->index(i, ColumnIndex::AlarmRuleId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        if (tempId == alarmRuleId)
        {
            if (!_model->setData(_model->index(i, ColumnIndex::AlarmSubCauseValue), alarmSubCause))
            {
                ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
            }

            submitData();

            return;
        }
    }

    Q_ASSERT(false);
}

void AlarmRuleTableManager::addNewRecord(int alarmRuleId, int alarmCauseId)
{
    int rowCount = _model->rowCount();
    if (!_model->insertRows(rowCount, 1))
    {
        ERROR << QString("call %1 fail:%2").arg("insertRows").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::AlarmRuleId), alarmRuleId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::AlarmCauseId), alarmCauseId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    submitData();
}

void AlarmRuleTableManager::deleteRecord(int alarmRuleId)
{
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int tempId = _model->index(i, ColumnIndex::AlarmRuleId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        if (tempId == alarmRuleId)
        {
            if (!_model->removeRows(i, 1))
            {
                ERROR << QString("call %1 fail:%2").arg("removeRows").arg(model()->lastError().text());
            }

            submitData();
            return;
        }
    }

    Q_ASSERT(false);
}

QList<int> AlarmRuleTableManager::getAlarmRuleIdList()
{
    QList<int> alarmRuleIdList;

    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int id = _model->index(i, ColumnIndex::AlarmRuleId).data().toInt(&ok);
        Q_ASSERT(ok);
        alarmRuleIdList << id;
    }

    return alarmRuleIdList;
}
