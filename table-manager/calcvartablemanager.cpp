#include "calcvartablemanager.h"
#include "calcvarbindtablemanager.h"
#include "logutil.h"
#include "maptablemanager.h"
#include <QSqlError>
#include <QSqlTableModel>

const QString CalcVarTableName = "config_calc_var";

CalcVarTableManager *CalcVarTableManager::_calcVarTableManager = 0;

CalcVarTableManager *CalcVarTableManager::calcVarTableManager()
{
    if (!_calcVarTableManager)
    {
        _calcVarTableManager = new CalcVarTableManager();
    }

    return _calcVarTableManager;
}

CalcVarTableManager::CalcVarTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable(CalcVarTableName);
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    refreshData();
}

void CalcVarTableManager::refreshData()
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

void CalcVarTableManager::submitData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(model()->lastError().text());
    }

    refreshData();
}

void CalcVarTableManager::addRecord(int formulaId, int calcVarId, int formulaType)
{
    int rowCount = _model->rowCount();
    if (!_model->insertRows(rowCount, 1))
    {
        ERROR << QString("call %1 fail:%2").arg("insertRows").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::FormulaId), formulaId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::CalcVarId), calcVarId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::FormulaType), formulaType))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    submitData();
}

void CalcVarTableManager::deleteRecord(int calcVarId)
{ 
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int id = _model->index(i, ColumnIndex::CalcVarId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        if (id == calcVarId)
        {
            if (!_model->removeRows(i, 1))
            {
                ERROR << QString("call %1 fail:%2").arg("removeRows").arg(model()->lastError().text());
            }

            submitData();

            //delete map recrod
            MapTableManager::mapTableManager(MapTableManager::TableIndex::CALCVAR)->deleteDesc(calcVarId);
            return;
        }
    }

    Q_UNREACHABLE();
}

void CalcVarTableManager::getCalVarInfo(int calcVarId, int *formulaId, int *formulaType)
{
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int tempCalcVarId = _model->index(i, ColumnIndex::CalcVarId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        if (tempCalcVarId == calcVarId)
        {
            if(formulaId)
            {
                *formulaId= _model->index(i, ColumnIndex::FormulaId).data().toInt(&ok);
                if (!ok)
                {
                    ERROR << QString("call %1 fail").arg("toInt");
                }
            }

            if(formulaType)
            {
                *formulaType= _model->index(i, ColumnIndex::FormulaType).data().toInt(&ok);
                if (!ok)
                {
                    ERROR << QString("call %1 fail").arg("toInt");
                }
            }

            return;
        }
    }

    Q_UNREACHABLE();
}

void CalcVarTableManager::fetchData(int formulaId)
{
    if (formulaId != -1)
    {
        _model->setFilter(QString("formula_id=%1").arg(formulaId));
    }
    else
    {
        _model->setFilter("");
    }

    refreshData();
}

void CalcVarTableManager::fetchData(DataType type)
{
    _model->setFilter(QString("formula_type=%1").arg(type));
    refreshData();
}

QList<int> CalcVarTableManager::getCalVarIdList(int formulaId)
{
    QList<int> calVarIdList;
    bool ok;

    fetchData(formulaId);
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int calcVarId = _model->index(i, ColumnIndex::CalcVarId).data().toInt(&ok);
        Q_ASSERT(ok);

        calVarIdList << calcVarId;
    }

    return calVarIdList;
}
