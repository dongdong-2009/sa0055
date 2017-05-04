#include "selectdevicedialog.h"
#include "datatype.h"
#include "deviceparatablemanager.h"
#include "foreignkeycolumndelegate.h"
#include "maptablemanager.h"
#include "ui_selectdevicedialog.h"
#include <QMessageBox>

SelectDeviceDialog::SelectDeviceDialog(QWidget *parent, bool isMultiSelection) : QDialog(parent), ui(new Ui::SelectDeviceDialog)
{
    ui->setupUi(this);

    ui->listView->setModel(DeviceParaTableManager::deviceParaTableManager()->model());
    ui->listView->setModelColumn(DeviceParaTableManager::ColumnIndex::DeviceId);
    ui->listView->setItemDelegateForColumn(
        DeviceParaTableManager::ColumnIndex::DeviceId,
        new ForeignKeyColumnDelegate(MapTableManager::mapTableManager(MapTableManager::TableIndex::DEV), false, this));
    ui->listView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->listView->setSelectionMode(isMultiSelection ? QAbstractItemView::SelectionMode::MultiSelection
                                                    : QAbstractItemView::SelectionMode::SingleSelection);

    connect(ui->okPushButton, &QPushButton::clicked, this, &SelectDeviceDialog::_checkResult);
    connect(ui->cancelPushButton, &QPushButton::clicked, [=] { reject(); });
}

void SelectDeviceDialog::_checkResult()
{
    QModelIndexList indexList = ui->listView->selectionModel()->selectedRows(DeviceParaTableManager::ColumnIndex::DeviceId);
    if (indexList.size() < 1)
    {
        QMessageBox::warning(this, "提示", "未选择装置!");
        return;
    }

    QList<int> list;
    bool ok;
    foreach (QModelIndex idx, indexList)
    {
        int deviceId = idx.data().toInt(&ok);
        Q_ASSERT(ok);
        list << deviceId;
    }

    emit selectedOk(list);
    accept();
}

SelectDeviceDialog::~SelectDeviceDialog()
{
    delete ui;
}
