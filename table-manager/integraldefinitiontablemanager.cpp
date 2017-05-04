#include "integraldefinitiontablemanager.h"
#include "logutil.h"
#include "maptablemanager.h"
#include <QSqlError>
#include <QSqlTableModel>

IntegralDefinitionTableManager *IntegralDefinitionTableManager::_integralDefTableManager = 0;

IntegralDefinitionTableManager *IntegralDefinitionTableManager::integralDefTableManager()
{
    if (!_integralDefTableManager)
    {
        _integralDefTableManager = new IntegralDefinitionTableManager();
    }

    return _integralDefTableManager;
}

IntegralDefinitionTableManager::IntegralDefinitionTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable("config_integral_definition");
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    refreshData();
}

void IntegralDefinitionTableManager::refreshData()
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

void IntegralDefinitionTableManager::submitData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(_model->lastError().text());
    }

    refreshData();
}

void IntegralDefinitionTableManager::addNewRecord(int integralId)
{
    Q_ASSERT(integralId >= 0);

    int rowCount = _model->rowCount();

    if (!_model->insertRows(rowCount, 1))
    {
        ERROR << QString("call %1 fail:%2").arg("insertRows").arg(_model->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::IntegralId), integralId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(_model->lastError().text());
    }

    submitData();
}

void IntegralDefinitionTableManager::deleteRecord(int row, int count)
{
    QList<int> integralVarIdList;
    bool ok;

    for (int i = row; i < row + count; i++)
    {
        int integralVarId = _model->index(i, ColumnIndex::IntegralId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail!").arg("toInt");
        }
        integralVarIdList.append(integralVarId);
    }

    if (!_model->removeRows(row, count))
    {
        ERROR << QString("call %1 fail:%2").arg("removeRows").arg(_model->lastError().text());
    }

    submitData();

    foreach (int integralVarId, integralVarIdList)
    {
        MapTableManager::mapTableManager(MapTableManager::TableIndex::INTEGRAL)->deleteDesc(integralVarId);
    }
}
