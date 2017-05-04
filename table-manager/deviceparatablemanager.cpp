#include "deviceparatablemanager.h"
#include "iec104paratablemanager.h"
#include "logutil.h"
#include "maptablemanager.h"
#include <QSqlError>
#include <QSqlTableModel>

DeviceParaTableManager *DeviceParaTableManager::_deviceParaTableManager = 0;

DeviceParaTableManager *DeviceParaTableManager::deviceParaTableManager()
{
    if (!_deviceParaTableManager)
    {
        _deviceParaTableManager = new DeviceParaTableManager();
    }

    return _deviceParaTableManager;
}

DeviceParaTableManager::DeviceParaTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable("config_device_para");
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
    _model->setSort(ColumnIndex::SeqNum, Qt::SortOrder::AscendingOrder);

    refreshData();
}

QSqlTableModel *DeviceParaTableManager::model()
{
    return _model;
}

void DeviceParaTableManager::refreshData()
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

void DeviceParaTableManager::submitData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(model()->lastError().text());
    }

    refreshData();
}

int DeviceParaTableManager::rowCountFromId(int deviceId)
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

void DeviceParaTableManager::addNewDevice(int deviceId)
{
    int rowCount = _model->rowCount();
    if (!_model->insertRows(rowCount, 1))
    {
        ERROR << QString("call %1 fail:%2").arg("insertRows").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::DeviceId), deviceId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    submitData();
}

void DeviceParaTableManager::deleteDevice(int deviceId)
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
                ERROR << QString("call %1 fail:%2").arg("removeRows").arg(model()->lastError().text());
            }

            submitData();

            /*将Device Map表中valid标志置为0*/
            MapTableManager::mapTableManager(MapTableManager::TableIndex::DEV)->deleteDesc(deviceId);
            return;
        }
    }

    Q_UNREACHABLE();
}

QList<int> DeviceParaTableManager::getDeviceIdList()
{
    QList<int> result;
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int deviceId = _model->index(i, ColumnIndex::DeviceId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        result << deviceId;
    }

    return result;
}
