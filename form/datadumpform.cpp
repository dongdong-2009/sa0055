#include "datadumpform.h"
#include "aidumptablemanager.h"
#include "iec104infotablemanager.h"
#include "integercolumndelegate.h"
#include "logutil.h"
#include "maptablemanager.h"
#include "selectpointcolumndelegate.h"
#include "ui_datadumpform.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QtSql>

DataDumpForm::DataDumpForm(QWidget *parent) : QWidget(parent), ui(new Ui::DataDumpForm)
{
    ui->setupUi(this);

    model = AiDumpTableManager::aiDumpTableManager()->model();

    model->setHeaderData(Iec104InfoTableManager::ColumnIndex::DescrId, Qt::Orientation::Horizontal, "遥测(AI)");
    model->setHeaderData(Iec104InfoTableManager::ColumnIndex::AI_Dump_Interval, Qt::Orientation::Horizontal, "转存间隔(分钟)");

    ui->tableView->setModel(model);

    ui->tableView->setItemDelegateForColumn(
        Iec104InfoTableManager::ColumnIndex::DescrId,
        new SelectPointColumnDelegate(this, SelectPoint::Type::AI, Iec104InfoTableManager::ColumnIndex::DeviceId, -1, false, false));
    ui->tableView->setItemDelegateForColumn(Iec104InfoTableManager::ColumnIndex::AI_Dump_Interval, new IntegerColumnDelegate(1, 65535, this));

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);

    for (int i = 0; i <= Iec104InfoTableManager::ColumnIndex::AI_Source; i++)
    {
        if (i != Iec104InfoTableManager::ColumnIndex::DescrId && i != Iec104InfoTableManager::ColumnIndex::AI_Dump_Interval)
        {
            ui->tableView->setColumnHidden(i, true);
        }
    }

    connect(ui->addButton, &QPushButton::clicked, [=]() {
        SelectPointDialog dialog(this, SelectPoint::Type::AI, SelectPoint::Mode::Multiple);
        connect(&dialog, &SelectPointDialog::selectedOk, this, &DataDumpForm::addRecord);
        dialog.exec();
    });

    connect(ui->deleteButton, &QPushButton::clicked, this, &DataDumpForm::deleteRecord);

    connect(ui->tableView, &QAbstractItemView::pressed, this, &DataDumpForm::setInterval);
}

void DataDumpForm::setInterval(const QModelIndex &index)
{
    if (!(index.column() == Iec104InfoTableManager::ColumnIndex::AI_Dump_Interval && QApplication::mouseButtons() == Qt::MouseButton::RightButton))
    {
        return;
    }

    QModelIndexList modelIndexList = ui->tableView->selectionModel()->selectedRows(Iec104InfoTableManager::ColumnIndex::AI_Dump_Interval);
    if (modelIndexList.size() < 2)
    {
        return;
    }

    int initValue = modelIndexList.at(0).data(Qt::ItemDataRole::EditRole).toInt();
    bool ok;
    int interval = QInputDialog::getInt(this, "设置", "将所选记录的转存间隔(分钟)设置为:", initValue, 1, 60, 1, &ok);
    if (!ok)
    {
        return;
    }

    QList<int> aiDescIdList;
    foreach (QModelIndex idx, modelIndexList)
    {
        int descId = idx.model()->index(idx.row(), Iec104InfoTableManager::ColumnIndex::DescrId).data().toInt(&ok);
        Q_ASSERT(ok);

        aiDescIdList << descId;
    }

    AiDumpTableManager::aiDumpTableManager()->setInterval(aiDescIdList, interval);
}

void DataDumpForm::deleteRecord()
{
    bool ok;
    QList<int> aiDescIdList;
    foreach (QModelIndex index, ui->tableView->selectionModel()->selectedRows(Iec104InfoTableManager::ColumnIndex::DescrId))
    {
        aiDescIdList << index.data().toInt(&ok);
        Q_ASSERT(ok);
    }

    if (aiDescIdList.size() > 0)
    {
        AiDumpTableManager::aiDumpTableManager()->deleteRecord(aiDescIdList);
    }
}

void DataDumpForm::addRecord(QList<SelectPoint> selectedList)
{
    AiDumpTableManager::aiDumpTableManager()->addRecord(selectedList);
}

DataDumpForm::~DataDumpForm()
{
    delete ui;
}

void DataDumpForm::showEvent(QShowEvent * /*event*/)
{
    fetchData();
}

void DataDumpForm::hideEvent(QHideEvent * /*event*/)
{
    saveData();
}

void DataDumpForm::fetchData()
{
    AiDumpTableManager::aiDumpTableManager()->fetchData();
}

void DataDumpForm::saveData()
{
    AiDumpTableManager::aiDumpTableManager()->submitData();
}
