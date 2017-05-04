#include "serverparatablemanager.h"
#include "logutil.h"
#include "datatype.h"
#include <QSqlError>
#include <QSqlTableModel>

ServerParaTableManager *ServerParaTableManager::_serverParaTableManager = 0;
const QString ServerParaTableName = "config_server_para";

ServerParaTableManager::ServerParaTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable(ServerParaTableName);
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    if (!_model->select())
    {
        ERROR << QString("call %1 fail:%2").arg("select").arg(model()->lastError().text());
    }

    while (_model->canFetchMore())
    {
        _model->fetchMore();
    }

    if(_model->rowCount() == 0)
    {
        _model->insertRow(0);
        _model->setData(_model->index(0, ColumnIndex::ServerRunMode), RunMode::Standby);
        submitData();
    }

    Q_ASSERT(_model->rowCount() == 1);
}

void ServerParaTableManager::submitData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(model()->lastError().text());
    }
}

ServerParaTableManager *ServerParaTableManager::serverParaTableManager()
{
    if (!_serverParaTableManager)
    {
        _serverParaTableManager = new ServerParaTableManager();
    }

    return _serverParaTableManager;
}
