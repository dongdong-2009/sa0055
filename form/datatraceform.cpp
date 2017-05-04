#include "datatraceform.h"
#include "aitracetablemanager.h"
#include "doublecolumndelegate.h"
#include "iec104infotablemanager.h"
#include "logutil.h"
#include "maptablemanager.h"
#include "selectpointcolumndelegate.h"
#include "ui_datatraceform.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QtSql>

DataTraceForm::DataTraceForm(QWidget *parent) : QWidget(parent), ui(new Ui::DataTraceForm)
{
    ui->setupUi(this);

    QSqlTableModel *model = AiTraceTableManager::aiTraceTableManager()->model();

    model->setHeaderData(Iec104InfoTableManager::ColumnIndex::DescrId, Qt::Orientation::Horizontal, "遥测(AI)");
    model->setHeaderData(Iec104InfoTableManager::ColumnIndex::AI_Trace_Deadband, Qt::Orientation::Horizontal, "死区");

    ui->tableView->setModel(model);

    ui->tableView->setItemDelegateForColumn(
        Iec104InfoTableManager::ColumnIndex::DescrId,
        new SelectPointColumnDelegate(this, SelectPoint::Type::AI, Iec104InfoTableManager::ColumnIndex::DeviceId, -1, false, false));
    ui->tableView->setItemDelegateForColumn(Iec104InfoTableManager::ColumnIndex::AI_Trace_Deadband, new DoubleColumnDelegate(0, 99999999, 6, this));

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);

    for (int i = 0; i <= Iec104InfoTableManager::ColumnIndex::AI_Source; i++)
    {
        if (i != Iec104InfoTableManager::ColumnIndex::DescrId && i != Iec104InfoTableManager::ColumnIndex::AI_Trace_Deadband)
        {
            ui->tableView->setColumnHidden(i, true);
        }
    }

    connect(ui->addButton, &QPushButton::clicked, [=]() {
        SelectPointDialog dialog(this, SelectPoint::Type::AI, SelectPoint::Mode::Multiple);
        connect(&dialog, &SelectPointDialog::selectedOk, this, &DataTraceForm::addRecord);
        dialog.exec();
    });

    connect(ui->deleteButton, &QPushButton::clicked, this, &DataTraceForm::deleteRecord);

    connect(ui->tableView, &QAbstractItemView::pressed, this, &DataTraceForm::setDeadband);
}

DataTraceForm::~DataTraceForm()
{
    delete ui;
}

void DataTraceForm::showEvent(QShowEvent * /*event*/)
{
    AiTraceTableManager::aiTraceTableManager()->fetchData();
}

void DataTraceForm::hideEvent(QHideEvent * /*event*/)
{
    AiTraceTableManager::aiTraceTableManager()->submitData();
}

void DataTraceForm::addRecord(QList<SelectPoint> selectedList)
{
    AiTraceTableManager::aiTraceTableManager()->addRecord(selectedList);
}

void DataTraceForm::deleteRecord()
{
    bool ok;
    QList<int> descIdList;
    foreach (QModelIndex index, ui->tableView->selectionModel()->selectedRows(Iec104InfoTableManager::ColumnIndex::DescrId))
    {
        descIdList << index.data().toInt(&ok);
        Q_ASSERT(ok);
    }

    if (descIdList.size() > 0)
    {
        AiTraceTableManager::aiTraceTableManager()->deleteRecord(descIdList);
    }
}

void DataTraceForm::setDeadband(const QModelIndex &index)
{
    if (!(index.column() == Iec104InfoTableManager::ColumnIndex::AI_Trace_Deadband && QApplication::mouseButtons() == Qt::MouseButton::RightButton))
    {
        return;
    }

    QModelIndexList modelIndexList = ui->tableView->selectionModel()->selectedRows(Iec104InfoTableManager::ColumnIndex::AI_Trace_Deadband);
    if (modelIndexList.size() < 2)
    {
        return;
    }

    double initValue = modelIndexList.at(0).data(Qt::ItemDataRole::EditRole).toDouble();

    bool ok;
    double deadband = QInputDialog::getDouble(this, "设置", "将所选记录的死区设置为:", initValue, 0, 99999999, 6, &ok);
    if (!ok)
    {
        return;
    }

    QList<int> aiDescIdList;
    foreach (QModelIndex idx, modelIndexList)
    {
        int descId = idx.model()->index(idx.row(), Iec104InfoTableManager::ColumnIndex::DescrId).data().toInt(&ok);
        aiDescIdList << descId;
    }
    AiTraceTableManager::aiTraceTableManager()->setDeadband(aiDescIdList, deadband);
}
