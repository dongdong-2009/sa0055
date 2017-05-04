#include "formulaform.h"
#include "calcvartablemanager.h"
#include "constvalue.h"
#include "descriptioncolumndelegate.h"
#include "formuladeftablemanager.h"
#include "logutil.h"
#include "maptablemanager.h"
#include "selectpointcolumndelegate.h"
#include "tempcalcvartablemodel.h"
#include "calcvarbindtablemanager.h"
#include "ui_formulaform.h"
#include "util.h"
#include "projecttreenode.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlTableModel>

/**
 * Column:0-calcVarId, 1-deviceId, 2-type, 3-pointId, [4-deviceId, 5-type, 6-pointId,] [7-deviceId, 8-type, 9-pointId]
 */
void FormulaForm::initCalcVarTable()
{
    tempCalcVarTableModel = new TempCalcVarTableModel(this);
    ui->tableView->setModel(tempCalcVarTableModel);

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);

    ui->tableView->setItemDelegateForColumn(0, new DescriptionColumnDelegate(MapTableManager::mapTableManager(MapTableManager::CALCVAR), this));
    for (int i = 0; i < FormulaVarMaxNum; i++)
    {
        ui->tableView->setColumnHidden(TempCalcVarTableModel::DeviceIdColumn(i), true);
        ui->tableView->setColumnHidden(TempCalcVarTableModel::DataTypeColumn(i), true);
        SelectPointColumnDelegate *selectPointDelagete =
            new SelectPointColumnDelegate(this, SelectPoint::Type::AI | SelectPoint::Type::DI,
                                          TempCalcVarTableModel::DeviceIdColumn(i), TempCalcVarTableModel::DataTypeColumn(i));
        ui->tableView->setItemDelegateForColumn(TempCalcVarTableModel::PointDescIdColumn(i), selectPointDelagete);

        connect(selectPointDelagete, &SelectPointColumnDelegate::selectPointChanged,
                [=](int row, int column, int deviceId, int pointType, int descId) {
                    bool ok;
                    int calcVarId = tempCalcVarTableModel->index(row, 0).data().toInt(&ok);
                    if (!ok)
                    {
                        ERROR << QString("call %1 fail").arg("toInt");
                    }

                    int varSeqNum = TempCalcVarTableModel::VarSeqNum(column);
                    CalcVarBindTableManager::calcVarBindTableManager()->updateRecord(calcVarId, varSeqNum, deviceId, pointType, descId);
                });
    }
}

void FormulaForm::hideUnusedVar(int formulaVarNum)
{
    for (int i = 0; i < FormulaVarMaxNum; i++)
    {
        if (i < formulaVarNum)
        {
            ui->tableView->setColumnHidden(TempCalcVarTableModel::PointDescIdColumn(i), false);
        }
        else
        {
            ui->tableView->setColumnHidden(TempCalcVarTableModel::PointDescIdColumn(i), true);
        }
    }
}

void FormulaForm::setFormulaValid(bool valid)
{
    ui->addCalcVariablePushButton->setEnabled(valid);
    ui->deleteCalcVariablePushButton->setEnabled(valid);
    ui->tableView->setEnabled(valid);

    ui->formulaValidatePushButton->setEnabled(false);
}

void FormulaForm::updateCalcVarTable(int formulaId)
{
    bool ok;
    QList<int> calcVarIdList;
    QList<QList<CalcVarBindPoint>> bindList;

    CalcVarTableManager *calcVarMgr = CalcVarTableManager::calcVarTableManager();
    calcVarMgr->fetchData(formulaId);
    QSqlTableModel *calcVarModel = calcVarMgr->model();
    for (int i = 0; i < calcVarModel->rowCount(); i++)
    {
        int calcVarId = calcVarModel->index(i, CalcVarTableManager::ColumnIndex::CalcVarId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        CalcVarBindTableManager *binVarMgr = CalcVarBindTableManager::calcVarBindTableManager();
        binVarMgr->fetchData(calcVarId);

        calcVarIdList.append(calcVarId);

        QList<CalcVarBindPoint> bindPoint;
        QSqlTableModel *bindModel = binVarMgr->model();
        for (int j = 0; j < bindModel->rowCount(); j++)
        {
            int varSeqNum = bindModel->index(j, CalcVarBindTableManager::ColumnIndex::VarSeqNum).data().toInt(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            int deviceId = bindModel->index(j, CalcVarBindTableManager::ColumnIndex::DeviceId).data().toInt(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            int pointType = bindModel->index(j, CalcVarBindTableManager::ColumnIndex::PointType).data().toInt(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            int pointId = bindModel->index(j, CalcVarBindTableManager::ColumnIndex::PointDescId).data().toInt(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            bindPoint.append(CalcVarBindPoint(varSeqNum, deviceId, pointType, pointId));
        }
        bindList.append(bindPoint);
    }

    tempCalcVarTableModel->updateAllRecord(calcVarIdList, bindList);
}

void FormulaForm::addNewCalcVar()
{
    int num = MapTableManager::mapTableManager(MapTableManager::TableIndex::CALCVAR)->currentMaxId() - Desc_ID_Start + 1;
    QString defaultCaclVarName = QString("计算量#%1").arg(num);

    forever
    {
        bool ok;
        QString calcVarName = QInputDialog::getText(this, "计算量", "请输入计算量名称:", QLineEdit::EchoMode::Normal, defaultCaclVarName, &ok);
        if (!ok)
        {
            break;
        }

        if (calcVarName.isEmpty())
        {
            QMessageBox::warning(this, "提示", "输入计算量名称不能为空!");
            continue;
        }

        int calcVarId = MapTableManager::mapTableManager(MapTableManager::TableIndex::CALCVAR)->addDesc(calcVarName);
        if (calcVarId == MapTableManager::Error::AlreadyExist)
        {
            QMessageBox::warning(this, "提示", "输入计算量名称已经被使用!");
            continue;
        }
        else
        {
            CalcVarTableManager::calcVarTableManager()->addRecord(formulaId, calcVarId, formulaType);
            tempCalcVarTableModel->addNewRecord(calcVarId);
            ProjectTreeNode::addNewCalcVarNode(calcVarId);
            break;
        }
    }
}

void FormulaForm::deleteCalcVar()
{
    QModelIndexList list = ui->tableView->selectionModel()->selectedRows();
    if (list.size() < 1)
    {
        QMessageBox::information(this, "提示", "请选择一条记录");
        return;
    }

    if (QMessageBox::question(this, "提示", "删除选择记录，是否继续?") == QMessageBox::StandardButton::No)
    {
        return;
    }

    bool ok;
    QList<int> rowList;
    foreach (QModelIndex index, list)
    {
        int calcVarId = index.data(Qt::ItemDataRole::EditRole).toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        CalcVarTableManager::calcVarTableManager()->deleteRecord(calcVarId);
        //delete variable bind
        CalcVarBindTableManager::calcVarBindTableManager()->deleteRecord(calcVarId);

        ProjectTreeNode::deleteCalcVarNode(calcVarId);

        rowList.append(index.row());
    }
    tempCalcVarTableModel->deleteRecord(rowList);
}
