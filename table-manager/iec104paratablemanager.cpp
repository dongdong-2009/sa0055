#include "iec104paratablemanager.h"
#include "logutil.h"
#include <QSqlError>
#include <QSqlTableModel>

Iec104ParaTableManager *Iec104ParaTableManager::_iec104ParaTableManager = 0;

const QString Iec104ParaTableName = "config_iec104_para";

Iec104ParaTableManager *Iec104ParaTableManager::iec104ParaTableManager()
{
    if (!_iec104ParaTableManager)
    {
        _iec104ParaTableManager = new Iec104ParaTableManager();
    }

    return _iec104ParaTableManager;
}

Iec104ParaTableManager::Iec104ParaTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable(Iec104ParaTableName);
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
    refreshData();
}

void Iec104ParaTableManager::refreshData()
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

void Iec104ParaTableManager::submitData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(_model->lastError().text());
    }

    refreshData();
}

int Iec104ParaTableManager::rowCountFromId(int deviceId)
{
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        if (_model->index(i, ColumnIndex::DeviceId).data().toInt(&ok) == deviceId)
        {
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }
            return i;
        }
    }

    Q_UNREACHABLE();
}

void Iec104ParaTableManager::addNewIec104ParaRecord(int deviceId)
{
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int id = _model->index(i, ColumnIndex::DeviceId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        if (id == deviceId)
        {
            Q_ASSERT_X(false, "addNewIec104ParaRecord", qPrintable(QString("deviceId=%1 already exist").arg(deviceId)));
            return;
        }
    }

    int rowCount = _model->rowCount();
    if (!_model->insertRows(rowCount, 1))
    {
        ERROR << QString("call %1 fail:%2").arg("insertRows").arg(_model->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::DeviceId), deviceId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(_model->lastError().text());
    }

    submitData();
}

void Iec104ParaTableManager::deleteIec104ParaRecord(int deviceId)
{
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int id = _model->index(i, ColumnIndex::DeviceId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        if (id == deviceId)
        {
            if (!_model->removeRows(i, 1))
            {
                ERROR << QString("call %1 fail:%2").arg("removeRows").arg(_model->lastError().text());
            }

            submitData();
            return;
        }
    }

    Q_UNREACHABLE();
}
