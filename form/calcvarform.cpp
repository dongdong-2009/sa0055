#include "calcvarform.h"
#include "calcvarbindtablemanager.h"
#include "calcvarbindtablemodel.h"
#include "calcvartablemanager.h"
#include "enumtablemanager.h"
#include "formuladeftablemanager.h"
#include "logutil.h"
#include "selectpointcolumndelegate.h"
#include "ui_calcvarform.h"
#include "util.h"
#include <QSqlTableModel>

CalcVarForm::CalcVarForm(QWidget *parent) : QWidget(parent), ui(new Ui::CalcValueForm)
{
    ui->setupUi(this);

    model = new CalcVarBindTableModel(this);
    ui->tableView->setModel(model);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    ui->tableView->setColumnHidden(ColumnIndex::DeviceId, true);
    ui->tableView->setColumnHidden(ColumnIndex::PointType, true);

    ui->tableView->setItemDelegateForColumn(ColumnIndex::PointDesc,
                                            new SelectPointColumnDelegate(this, SelectPoint::Type::AI | SelectPoint::Type::DI,
                                                                          ColumnIndex::DeviceId, ColumnIndex::PointType, false, false));
}

CalcVarForm::~CalcVarForm()
{
    delete ui;
}

void CalcVarForm::setCurrentCalcVarId(int calcVarId)
{
    bool ok;
    CalcVarTableManager::calcVarTableManager()->fetchData();
    int formulaId;
    CalcVarTableManager::calcVarTableManager()->getCalVarInfo(calcVarId, &formulaId);

    int formulaType;
    int formulaVarNum;
    QString formulaExprStr;
    FormulaDefTableManager::formulaDefinitionTableManager()->getFormulaInfo(formulaId, &formulaType, &formulaVarNum, &formulaExprStr);
    QString formulaTyeStr = EnumTableManager::formulaTypeEnumManager()->nameFromId(formulaType);

    ui->formulaExprLineEdit->setText(formulaExprStr);
    ui->formulaTypeLineEdit->setText(formulaTyeStr);

    CalcVarBindTableManager::calcVarBindTableManager()->fetchData(calcVarId);
    QSqlTableModel *bindTableModel = CalcVarBindTableManager::calcVarBindTableManager()->model();

    QList<CalcVarBindPoint> pointList;
    for (int i = 0; i < bindTableModel->rowCount(); i++)
    {
        int varSeqNum = bindTableModel->index(i, CalcVarBindTableManager::ColumnIndex::VarSeqNum).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        int deviceId = bindTableModel->index(i, CalcVarBindTableManager::ColumnIndex::DeviceId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        int pointType = bindTableModel->index(i, CalcVarBindTableManager::ColumnIndex::PointType).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        int pointDescId = bindTableModel->index(i, CalcVarBindTableManager::ColumnIndex::PointDescId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        pointList.append(CalcVarBindPoint(varSeqNum, deviceId, pointType, pointDescId));
    }
    model->updateRecord(formulaVarNum, pointList);
}
