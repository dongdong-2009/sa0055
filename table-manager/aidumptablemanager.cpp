#include "aidumptablemanager.h"
#include "iec104infotablemanager.h"
#include "logutil.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTime>

AiDumpTableManager *AiDumpTableManager::_aiDumpTableManager = 0;

AiDumpTableManager *AiDumpTableManager::aiDumpTableManager()
{
    if (!_aiDumpTableManager)
    {
        _aiDumpTableManager = new AiDumpTableManager();
    }

    return _aiDumpTableManager;
}

AiDumpTableManager::AiDumpTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable("config_ai_point");
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
}

void AiDumpTableManager::deleteRecord(QList<int> aiDescIdList)
{
    QSqlQuery query;
    QString sqlStr = QString("UPDATE config_ai_point SET dump_flag = 0, dump_interval = 5 WHERE ai_id IN (");

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
        ERROR << QString("call %1(%2) fail:%3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
    }

    if (!query.exec())
    {
        ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
    }

    _refreshData();
}

void AiDumpTableManager::addRecord(QList<SelectPoint> selectPointList)
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

        bool dumpFlag = _model->index(j, Iec104InfoTableManager::ColumnIndex::AI_Dump_Flag).data().toInt(&ok);
        Q_ASSERT(ok);

        if (dumpFlag)
        {
            selectPointDescIdList.removeOne(descId);
        }
    }

    if (selectPointDescIdList.size() > 0)
    {
        QSqlQuery query;
        QString sqlStr = QString("UPDATE config_ai_point SET dump_flag = 1, dump_interval = 5 WHERE ai_id IN (");

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

void AiDumpTableManager::submitData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(model()->lastError().text());
    }

    _refreshData();
}

void AiDumpTableManager::_refreshData()
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

void AiDumpTableManager::fetchData(bool isAll)
{
    if (!isAll)
    {
        _model->setFilter("dump_flag <> 0");
    }
    else
    {
        _model->setFilter("");
    }

    _refreshData();
}

void AiDumpTableManager::setInterval(QList<int> aiDescIdList, int interval)
{
    QSqlQuery query;
    QString sqlStr = QString("UPDATE config_ai_point SET dump_flag = 1, dump_interval = %1 WHERE ai_id IN (").arg(interval);

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
