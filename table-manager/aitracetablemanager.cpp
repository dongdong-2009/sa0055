#include "aitracetablemanager.h"
#include "iec104infotablemanager.h"
#include "logutil.h"
#include "selectpointdialog.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

AiTraceTableManager *AiTraceTableManager::_aiTraceTableManager = 0;

AiTraceTableManager::AiTraceTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable("config_ai_point");
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
}

AiTraceTableManager *AiTraceTableManager::aiTraceTableManager()
{
    if (!_aiTraceTableManager)
    {
        _aiTraceTableManager = new AiTraceTableManager();
    }
    return _aiTraceTableManager;
}

void AiTraceTableManager::deleteRecord(QList<int> aiDescIdList)
{
    QSqlQuery query;
    QString sqlStr = QString("UPDATE config_ai_point SET trace_flag = 0, deadband = 0 WHERE ai_id IN (");

    for (int i = 0; i < aiDescIdList.size(); i++)
    {
        sqlStr += QString::number(aiDescIdList.at(i));
        if (i < aiDescIdList.size() - 1)
        {
            sqlStr += ", ";
        }
        else
        {
            sqlStr += ");";
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

    _refreshData();
}

void AiTraceTableManager::addRecord(QList<SelectPoint> selectPointList)
{
    fetchData(true);

    QList<int> selectPointDescIdList;
    for (int i = 0; i < selectPointList.size(); i++)
    {
        selectPointDescIdList << selectPointList.at(i).getPointDescId();
    }

    bool ok;
    for (int j = 0; j < _model->rowCount(); j++)
    {
        int descId = _model->index(j, Iec104InfoTableManager::ColumnIndex::DescrId).data().toInt(&ok);
        Q_ASSERT(ok);

        bool traceFlag = _model->index(j, Iec104InfoTableManager::ColumnIndex::AI_Trace_Flag).data().toInt(&ok);
        Q_ASSERT(ok);

        if (traceFlag)
        {
            selectPointDescIdList.removeOne(descId);
        }
    }

    if (selectPointDescIdList.size() > 0)
    {
        QSqlQuery query;
        QString sqlStr = QString("UPDATE config_ai_point SET trace_flag = 1, deadband = 0 WHERE ai_id IN (");

        for (int i = 0; i < selectPointDescIdList.size(); i++)
        {
            sqlStr += QString::number(selectPointDescIdList.at(i));
            if (i < selectPointDescIdList.size() - 1)
            {
                sqlStr += ", ";
            }
            else
            {
                sqlStr += ");";
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
    }

    fetchData();
}

void AiTraceTableManager::submitData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(model()->lastError().text());
    }

    _refreshData();
}

void AiTraceTableManager::_refreshData()
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

void AiTraceTableManager::fetchData(bool isAll)
{
    if (!isAll)
    {
        _model->setFilter("trace_flag <> 0");
    }
    else
    {
        _model->setFilter("");
    }

    _refreshData();
}

void AiTraceTableManager::setDeadband(QList<int> aiDescIdList, double deadband)
{
    QSqlQuery query;
    QString sqlStr = QString("UPDATE config_ai_point SET trace_flag = 1, deadband = %1 WHERE ai_id IN (").arg(deadband);

    for (int i = 0; i < aiDescIdList.size(); i++)
    {
        sqlStr += QString::number(aiDescIdList.at(i));
        if (i < aiDescIdList.size() - 1)
        {
            sqlStr += ", ";
        }
        else
        {
            sqlStr += ");";
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

    fetchData();
}
