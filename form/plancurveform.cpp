#include "plancurveform.h"
#include "constvalue.h"
#include "descriptioncolumndelegate.h"
#include "enumtablemanager.h"
#include "foreignkeycolumndelegate.h"
#include "integercolumndelegate.h"
#include "maptablemanager.h"
#include "plancurvetablemanager.h"
#include "selectdevicecolumndelegate.h"
#include "selectpointcolumndelegate.h"
#include "selectpointdialog.h"
#include "ui_plancurveform.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlTableModel>

PlanCurveForm::PlanCurveForm(QWidget *parent) : QWidget(parent), ui(new Ui::PlanCurveForm)
{
    ui->setupUi(this);

    QSqlTableModel *model = PlanCurveTableManager::planCurveTableManager()->model();

    model->setHeaderData(PlanCurveTableManager::ColumnIndex::CurveId, Qt::Orientation::Horizontal,
                         PlanCurveTableManager::ColumnName[PlanCurveTableManager::ColumnIndex::CurveId]);
    model->setHeaderData(PlanCurveTableManager::ColumnIndex::CurveAddr, Qt::Orientation::Horizontal,
                         PlanCurveTableManager::ColumnName[PlanCurveTableManager::ColumnIndex::CurveAddr]);
    model->setHeaderData(PlanCurveTableManager::ColumnIndex::CurveType, Qt::Orientation::Horizontal,
                         PlanCurveTableManager::ColumnName[PlanCurveTableManager::ColumnIndex::CurveType]);
    model->setHeaderData(PlanCurveTableManager::ColumnIndex::CalcMethod, Qt::Orientation::Horizontal,
                         PlanCurveTableManager::ColumnName[PlanCurveTableManager::ColumnIndex::CalcMethod]);
    model->setHeaderData(PlanCurveTableManager::ColumnIndex::DeviceId, Qt::Orientation::Horizontal,
                         PlanCurveTableManager::ColumnName[PlanCurveTableManager::ColumnIndex::DeviceId]);
    model->setHeaderData(PlanCurveTableManager::ColumnIndex::AiStartPoint, Qt::Orientation::Horizontal,
                         PlanCurveTableManager::ColumnName[PlanCurveTableManager::ColumnIndex::AiStartPoint]);
    model->setHeaderData(PlanCurveTableManager::ColumnIndex::RealtimeAI, Qt::Orientation::Horizontal,
                         PlanCurveTableManager::ColumnName[PlanCurveTableManager::ColumnIndex::RealtimeAI]);
    model->setHeaderData(PlanCurveTableManager::ColumnIndex::Unit, Qt::Orientation::Horizontal,
                         PlanCurveTableManager::ColumnName[PlanCurveTableManager::ColumnIndex::Unit]);

    ui->tableView->setModel(model);

    ui->tableView->setItemDelegateForColumn(
        PlanCurveTableManager::ColumnIndex::CurveId,
        new DescriptionColumnDelegate(MapTableManager::mapTableManager(MapTableManager::TableIndex::PLANCURVE), this));

    ui->tableView->setItemDelegateForColumn(PlanCurveTableManager::ColumnIndex::CurveAddr, new IntegerColumnDelegate(0, MaxIntegerValue, this));

    ui->tableView->setItemDelegateForColumn(PlanCurveTableManager::ColumnIndex::CurveType,
                                            new ForeignKeyColumnDelegate(EnumTableManager::curveTypeEnumManager(), true, this));
    ui->tableView->setItemDelegateForColumn(PlanCurveTableManager::ColumnIndex::CalcMethod,
                                            new ForeignKeyColumnDelegate(EnumTableManager::curveCalcMethodEnumManager(), true, this));
    ui->tableView->setItemDelegateForColumn(PlanCurveTableManager::ColumnIndex::Unit,
                                            new ForeignKeyColumnDelegate(EnumTableManager::curveUnitEnumManager(), true, this));

    SelectDeviceColumnDelegate *selectDevColumnDelegate = new SelectDeviceColumnDelegate(this);
    ui->tableView->setItemDelegateForColumn(PlanCurveTableManager::ColumnIndex::DeviceId, selectDevColumnDelegate);

    ui->tableView->setItemDelegateForColumn(
        PlanCurveTableManager::ColumnIndex::AiStartPoint,
        new SelectPointColumnDelegate(this, SelectPoint::Type::AI, PlanCurveTableManager::ColumnIndex::DeviceId, -1, true));

    ui->tableView->setItemDelegateForColumn(
        PlanCurveTableManager::ColumnIndex::RealtimeAI,
        new SelectPointColumnDelegate(this, SelectPoint::Type::AI, PlanCurveTableManager::ColumnIndex::DeviceId, -1, true));

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);

    ui->tableView->setColumnHidden(PlanCurveTableManager::ColumnIndex::SeqNum, true);

    connect(ui->addPushButton, &QPushButton::clicked, this, &PlanCurveForm::addRecord);
    connect(ui->deletePushButton, &QPushButton::clicked, this, &PlanCurveForm::deleteRecord);

    connect(selectDevColumnDelegate, &SelectDeviceColumnDelegate::deviceIdChanged,
            [=](int row, int /*column*/, int /*deviceId*/) { PlanCurveTableManager::planCurveTableManager()->updateRecord(row, -1, -1); });
}

PlanCurveForm::~PlanCurveForm()
{
    delete ui;
}

void PlanCurveForm::hideEvent(QHideEvent * /*event*/)
{
    PlanCurveTableManager::planCurveTableManager()->submitData();

    DEBUG << configValidCheck();
}

void PlanCurveForm::addRecord()
{
    forever
    {
        bool ok;
        int num = PlanCurveTableManager::planCurveTableManager()->model()->rowCount() + 1;
        QString defaultName = QString("%1#%2").arg("计划曲线").arg(num);
        QString curveName = QInputDialog::getText(this, "计划曲线", "请输入计划曲线名称:", QLineEdit::EchoMode::Normal, defaultName, &ok);
        if (!ok)
        {
            break;
        }

        if (curveName.isEmpty())
        {
            QMessageBox::warning(this, "提示", "输入的计划曲线名称不能为空!");
            continue;
        }

        int curveId = MapTableManager::mapTableManager(MapTableManager::TableIndex::PLANCURVE)->addDesc(curveName);
        if (curveId == MapTableManager::Error::AlreadyExist)
        {
            QMessageBox::warning(this, "提示", "输入的计划曲线名称已经存在!");
        }
        else
        {
            PlanCurveTableManager::planCurveTableManager()->addNewRecord(curveId);
            break;
        }
    }
}

void PlanCurveForm::deleteRecord()
{
    QModelIndexList list = ui->tableView->selectionModel()->selectedRows();
    if (list.size() < 1)
    {
        QMessageBox::information(this, "提示", "请选择一条记录");
        return;
    }

    if (QMessageBox::question(this, "提示", "删除选中记录，是否继续?") == QMessageBox::StandardButton::No)
    {
        return;
    }

    PlanCurveTableManager::planCurveTableManager()->deleteRecord(list.at(0).row(), list.size());
}

QStringList PlanCurveForm::configValidCheck()
{
    QStringList errorList;
    QSqlTableModel *model = PlanCurveTableManager::planCurveTableManager()->model();
    bool ok;

    for (int i = 0; i < model->rowCount(); i++)
    {
        int deviceId = model->index(i, PlanCurveTableManager::ColumnIndex::DeviceId).data().toInt(&ok);
        Q_ASSERT(ok);

        int aiStart = model->index(i, PlanCurveTableManager::ColumnIndex::AiStartPoint).data().toInt(&ok);
        Q_ASSERT(ok);

        int rtAi = model->index(i, PlanCurveTableManager::ColumnIndex::RealtimeAI).data().toInt(&ok);
        Q_ASSERT(ok);

        if (MapTableManager::mapTableManager(MapTableManager::TableIndex::DEV)->nameFromId(deviceId).isEmpty())
        {
            errorList << QString("第%1条计算曲线未选择装置!").arg(i + 1);
        }

        if (MapTableManager::mapTableManager(MapTableManager::TableIndex::AI)->nameFromId(aiStart).isEmpty())
        {
            errorList << QString("第%1条计算曲线未选择曲线起始遥测(AI)!").arg(i + 1);
        }

        if (MapTableManager::mapTableManager(MapTableManager::TableIndex::AI)->nameFromId(rtAi).isEmpty())
        {
            errorList << QString("第%1条计算曲线未选择实时遥测(AI)!").arg(i + 1);
        }
    }

    return errorList;
}
