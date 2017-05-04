#include "formuladeftablemanager.h"

#include "logutil.h"
#include "maptablemanager.h"
#include <QSqlError>
#include <QSqlTableModel>

FormulaDefTableManager *FormulaDefTableManager::_formulaDefinitionTableManager = 0;

FormulaDefTableManager *FormulaDefTableManager::formulaDefinitionTableManager()
{
    if (!_formulaDefinitionTableManager)
    {
        _formulaDefinitionTableManager = new FormulaDefTableManager();
    }

    return _formulaDefinitionTableManager;
}

FormulaDefTableManager::FormulaDefTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable("config_formula_definition");
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    refreshData();
}

void FormulaDefTableManager::refreshData()
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

void FormulaDefTableManager::submitData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(model()->lastError().text());
    }

    refreshData();
}

void FormulaDefTableManager::addNewRecord(int formulaId, int formulaType)
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

    if (!_model->setData(_model->index(rowCount, ColumnIndex::FormulaType), formulaType))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    submitData();
}

void FormulaDefTableManager::setFormulaInfo(int formulaId, int *formulaVarNum, QString *formulaExpr)
{
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        if (_model->index(i, ColumnIndex::FormulaId).data().toInt(&ok) == formulaId)
        {
            Q_ASSERT(ok);

            if(formulaVarNum)
            {
                if (!_model->setData(_model->index(i, ColumnIndex::VariableNum), *formulaVarNum))
                {
                    ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
                }
            }

            if(formulaExpr)
            {
                if (!_model->setData(_model->index(i, ColumnIndex::FormulaExpr), *formulaExpr))
                {
                    ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
                }
            }

            submitData();
            return;
        }
    }

    Q_UNREACHABLE();
}

void FormulaDefTableManager::getFormulaInfo(int formulaId, int *formulaType, int *varNum, QString *formulaExpr, int *rowCount)
{
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        if (_model->index(i, ColumnIndex::FormulaId).data().toInt(&ok) == formulaId)
        {
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            if (formulaType)
            {
                *formulaType = _model->index(i, ColumnIndex::FormulaType).data().toInt(&ok);
                if (!ok)
                {
                    ERROR << QString("call %1 fail").arg("toInt");
                }
            }

            if (varNum)
            {
                *varNum = _model->index(i, ColumnIndex::VariableNum).data().toInt(&ok);
                if (!ok)
                {
                    ERROR << QString("call %1 fail").arg("toInt");
                }
            }

            if(formulaExpr)
            {
                *formulaExpr = _model->index(i, ColumnIndex::FormulaExpr).data().toString();
            }

            if(rowCount)
            {
                *rowCount = i;
            }

            return;
        }
    }

    Q_UNREACHABLE();
}

void FormulaDefTableManager::deleteRecord(int row, int count)
{
    if (!_model->removeRows(row, count))
    {
        ERROR << QString("call %1 fail:%2").arg("removeRows").arg(model()->lastError().text());
    }

    submitData();
}

QList<int> FormulaDefTableManager::getFormulaIdList()
{
    QList<int> formulaIdList;
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int formulaId = _model->index(i, ColumnIndex::FormulaId).data().toInt(&ok);
        Q_ASSERT(ok);
        formulaIdList << formulaId;
    }

    return formulaIdList;
}

void FormulaDefTableManager::deleteRecordById(int formulaId)
{
    bool ok;
    for (int i = 0; i < _model->rowCount(); i++)
    {
        int id = _model->index(i, ColumnIndex::FormulaId).data().toInt(&ok);
        if(id == formulaId)
        {
            if (!_model->removeRows(i, 1))
            {
                ERROR << QString("call %1 fail:%2").arg("removeRows").arg(model()->lastError().text());
            }

            submitData();

            MapTableManager::mapTableManager(MapTableManager::TableIndex::FORMULA)->deleteDesc(formulaId);
            return;
        }
    }
}
