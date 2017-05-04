#include "plancurvetablemanager.h"
#include "logutil.h"
#include "maptablemanager.h"
#include <QSqlError>
#include <QSqlTableModel>

PlanCurveTableManager *PlanCurveTableManager::_planCurveTableManager = 0;

QString PlanCurveTableManager::ColumnName[] = {"序号",     "曲线名称",         "曲线地址",     "曲线类型", "计算方法",
                                               "装置名称", "曲线起始遥测(AI)", "实时遥测(AI)", "单位"};

PlanCurveTableManager *PlanCurveTableManager::planCurveTableManager()
{
    if (!_planCurveTableManager)
    {
        _planCurveTableManager = new PlanCurveTableManager();
    }

    return _planCurveTableManager;
}

PlanCurveTableManager::PlanCurveTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable("config_plan_curve");
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    refreshData();
}

void PlanCurveTableManager::refreshData()
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

void PlanCurveTableManager::submitData()
{
    if (!_model->submitAll())
    {
        ERROR << QString("call %1 fail:%2").arg("submitAll").arg(model()->lastError().text());
    }

    refreshData();
}

void PlanCurveTableManager::addNewRecord(int curveId)
{
    Q_ASSERT(curveId >= 0);

    bool ok;
    int rowCount = _model->rowCount();

    int addr;
    if (rowCount > 0)
    {
        addr = _model->index(rowCount - 1, ColumnIndex::CurveAddr).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        addr += 0x121;
    }
    else
    {
        addr = 25089;
    }

    if (!_model->insertRows(rowCount, 1))
    {
        ERROR << QString("call %1 fail:%2").arg("insertRows").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::CurveId), curveId))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(rowCount, ColumnIndex::CurveAddr), addr))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    submitData();
}

void PlanCurveTableManager::deleteRecord(int row, int count)
{
    QList<int> curveIdList;
    bool ok;

    for (int i = row; i < row + count; i++)
    {
        int curveId = _model->index(i, ColumnIndex::CurveId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail!").arg("toInt");
        }
        curveIdList.append(curveId);
    }

    if (!_model->removeRows(row, count))
    {
        ERROR << QString("call %1 fail:%2").arg("removeRows").arg(model()->lastError().text());
    }

    submitData();

    foreach (int curveId, curveIdList)
    {
        MapTableManager::mapTableManager(MapTableManager::TableIndex::PLANCURVE)->deleteDesc(curveId);
    }
}

void PlanCurveTableManager::updateRecord(int row, int startAi, int rtAi)
{
    if (!_model->setData(_model->index(row, ColumnIndex::AiStartPoint), startAi))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    if (!_model->setData(_model->index(row, ColumnIndex::RealtimeAI), rtAi))
    {
        ERROR << QString("call %1 fail:%2").arg("setData").arg(model()->lastError().text());
    }

    submitData();
}
