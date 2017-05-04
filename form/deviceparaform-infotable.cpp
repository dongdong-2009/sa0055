#include "deviceparaform.h"
#include "constvalue.h"
#include "datatype.h"
#include "descriptioncolumndelegate.h"
#include "doublecolumndelegate.h"
#include "enumtablemanager.h"
#include "foreignkeycolumndelegate.h"
#include "iec104infotablemanager.h"
#include "maptablemanager.h"
#include "ui_deviceparaform.h"
#include "util.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlTableModel>

const QString DataTypeStr[] = {"遥信(DI)", "遥测(AI)", "遥控(DO)", "遥调(AO)", "累计量(CI)"};

void DeviceParaForm::initInfoTable()
{
    tableView[DataType::DI] = ui->tableViewDI;
    tableView[DataType::AI] = ui->tableViewAI;
    tableView[DataType::DO] = ui->tableViewDO;
    tableView[DataType::AO] = ui->tableViewAO;
    tableView[DataType::CI] = ui->tableViewCI;

    for (int i = DataType::DI; i < DataType::DataTypeNum; i++)
    {
        initInfoView(Util::integer_2_dataType(i));
    }

    connect(ui->addButton, &QPushButton::pressed, this, &DeviceParaForm::addInfoRecord);
    connect(ui->delButton, &QPushButton::pressed, this, &DeviceParaForm::deleteInfoRecord);
    connect(ui->clearButton, &QPushButton::pressed, this, &DeviceParaForm::clearInfoRecord);
    connect(ui->exportButton, &QPushButton::pressed, this, &DeviceParaForm::exportInfoRecord);
    connect(ui->importButton, &QPushButton::pressed, this, &DeviceParaForm::importInfoRecord);
}

void DeviceParaForm::initInfoView(DataType dataType)
{
    QSqlTableModel *infoModel = Iec104InfoTableManager::infoTableManager(dataType)->model();
    QTableView *infoView = tableView[dataType];

    infoView->setModel(infoModel);
    infoView->setAlternatingRowColors(true);
    infoView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    infoView->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);
    infoView->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);

    //列表头
    infoModel->setHeaderData(Iec104InfoTableManager::ColumnIndex::DescrId, Qt::Orientation::Horizontal, "描述");
    if (dataType == DataType::AI)
    {
        infoModel->setHeaderData(Iec104InfoTableManager::ColumnIndex::AI_Multiple, Qt::Orientation::Horizontal, "乘法因子");
        infoModel->setHeaderData(Iec104InfoTableManager::ColumnIndex::AI_Offset, Qt::Orientation::Horizontal, "偏移量");
    }
    else if (dataType == DataType::DO)
    {
        infoModel->setHeaderData(Iec104InfoTableManager::ColumnIndex::DO_Qualifier, Qt::Orientation::Horizontal, "限定词");
        infoModel->setHeaderData(Iec104InfoTableManager::ColumnIndex::DO_Type, Qt::Orientation::Horizontal, "类型");
        infoModel->setHeaderData(Iec104InfoTableManager::ColumnIndex::DO_StepType, Qt::Orientation::Horizontal, "步调节");
    }

    MapTableManager::TableIndex tblIndex = Util::dataType_2_mapTableIndex(dataType);
    infoView->setItemDelegateForColumn(Iec104InfoTableManager::ColumnIndex::DescrId,
                                       new DescriptionColumnDelegate(MapTableManager::mapTableManager(tblIndex), this));
    if (dataType == DataType::AI)
    {
        infoView->setItemDelegateForColumn(Iec104InfoTableManager::ColumnIndex::AI_Multiple,
                                           new DoubleColumnDelegate(MinDoubleValue, MaxDoubleValue, MaxDecimals, this));
        infoView->setItemDelegateForColumn(Iec104InfoTableManager::ColumnIndex::AI_Offset,
                                           new DoubleColumnDelegate(MinDoubleValue, MaxDoubleValue, MaxDecimals, this));
    }
    else if (dataType == DataType::DO)
    {
        infoView->setItemDelegateForColumn(Iec104InfoTableManager::ColumnIndex::DO_Qualifier,
                                           new ForeignKeyColumnDelegate(EnumTableManager::doQualifierEnumManager(), true, this));
        infoView->setItemDelegateForColumn(Iec104InfoTableManager::ColumnIndex::DO_Type,
                                           new ForeignKeyColumnDelegate(EnumTableManager::doTypeEnumManager(), true, this));
        infoView->setItemDelegateForColumn(Iec104InfoTableManager::ColumnIndex::DO_StepType,
                                           new ForeignKeyColumnDelegate(EnumTableManager::doStepTypeEnumManager(), true, this));
    }

    //隐藏列
    for (int i = 0; i < infoModel->columnCount(); i++)
    {
        if (dataType == DataType::AI)
        {
            if (i != Iec104InfoTableManager::ColumnIndex::DescrId && i != Iec104InfoTableManager::ColumnIndex::AI_Multiple &&
                i != Iec104InfoTableManager::ColumnIndex::AI_Offset)
            {
                infoView->setColumnHidden(i, true);
            }
        }
        else if (dataType == DataType::DO)
        {
            if (i != Iec104InfoTableManager::ColumnIndex::DescrId && i != Iec104InfoTableManager::ColumnIndex::DO_Qualifier &&
                i != Iec104InfoTableManager::ColumnIndex::DO_Type && i != Iec104InfoTableManager::ColumnIndex::DO_StepType)
            {
                infoView->setColumnHidden(i, true);
            }
        }
        else
        {
            if (i != Iec104InfoTableManager::ColumnIndex::DescrId)
            {
                infoView->setColumnHidden(i, true);
            }
        }
    }
}

void DeviceParaForm::addInfoRecord()
{
    Q_ASSERT(ui->infoTableTabWidget->count() == DataType::DataTypeNum);

    DataType dataType = Util::integer_2_dataType(ui->infoTableTabWidget->currentIndex());
    MapTableManager::TableIndex tblIndex = Util::dataType_2_mapTableIndex(dataType);

    int num = MapTableManager::mapTableManager(tblIndex)->currentMaxId() - Desc_ID_Start + 1;
    QString defaultName = QString("%1#%2").arg(DataTypeStr[dataType]).arg(num);

    forever
    {
        bool ok;
        QString desc = QInputDialog::getText(this, "描述", tr("请输入新增%1记录的描述:").arg(DataTypeStr[dataType]), QLineEdit::EchoMode::Normal,
                                             defaultName, &ok);
        if (!ok)
        {
            break;
        }

        if (desc.isEmpty())
        {
            QMessageBox::warning(this, "警告", "描述不能为空!");
            continue;
        }

        int descId = MapTableManager::mapTableManager(tblIndex)->addDesc(desc);
        if (descId == MapTableManager::AlreadyExist)
        {
            QMessageBox::warning(this, "警告", "描述已经被使用，不能添加!");
            continue;
        }
        else
        {
            Iec104InfoTableManager::infoTableManager(dataType)->addRecord(deviceId, descId);
            break;
        }
    }
}

void DeviceParaForm::deleteInfoRecord()
{
    DataType dataType = Util::integer_2_dataType(ui->infoTableTabWidget->currentIndex());
    QTableView *infoView = tableView[dataType];

    QModelIndexList list = infoView->selectionModel()->selectedRows(0);
    if (list.size() < 1)
    {
        QMessageBox::warning(this, "警告", tr("请选择需要删除的%1记录:").arg(DataTypeStr[dataType]));
        return;
    }

    if (QMessageBox::question(this, "提示", tr("删除选定的%1记录，是否继续?").arg(DataTypeStr[dataType])) == QMessageBox::StandardButton::No)
    {
        return;
    }

    Iec104InfoTableManager::infoTableManager(dataType)->deleteRecord(deviceId, list.at(0).row(), list.size());
}

void DeviceParaForm::_clearInfoRecord(DataType dataType)
{
    Iec104InfoTableManager *infoMgr = Iec104InfoTableManager::infoTableManager(dataType);
    if (infoMgr->model()->rowCount() > 0)
    {
        infoMgr->deleteRecord(deviceId);
    }
}

void DeviceParaForm::clearInfoRecord()
{
    int rowCountNum = 0;
    for (int i = DataType::DI; i < DataType::DataTypeNum; i++)
    {
        DataType dataType = Util::integer_2_dataType(i);
        rowCountNum += Iec104InfoTableManager::infoTableManager(dataType)->model()->rowCount();
    }

    if (rowCountNum < 1)
    {
        return;
    }

    if (QMessageBox::question(this, "提示", QString("清空四遥信息表, 是否继续?")) == QMessageBox::StandardButton::No)
    {
        return;
    }

    for (int i = DataType::DI; i < DataType::DataTypeNum; i++)
    {
        DataType dataType = Util::integer_2_dataType(i);
        _clearInfoRecord(dataType);
    }
}

void DeviceParaForm::refreshInfoTable()
{
    for (int i = DataType::DI; i < DataType::DataTypeNum; i++)
    {
        Iec104InfoTableManager::infoTableManager(Util::integer_2_dataType(i))->fetchData(deviceId);
    }
}

void DeviceParaForm::saveInfoTable()
{
    for (int i = DataType::DI; i < DataType::DataTypeNum; i++)
    {
        DataType dataType = Util::integer_2_dataType(i);
        Iec104InfoTableManager::infoTableManager(dataType)->submitData();
    }
}

QStringList DeviceParaForm::infoTableConfigValidCheck()
{
    QStringList errorString;

    int rowCountNum = Iec104InfoTableManager::infoTableManager(DataType::DI)->model()->rowCount();
    rowCountNum += Iec104InfoTableManager::infoTableManager(DataType::AI)->model()->rowCount();

    if (rowCountNum < 1)
    {
        errorString << QString("遥测/遥信信息表为空!");
    }

    return errorString;
}
