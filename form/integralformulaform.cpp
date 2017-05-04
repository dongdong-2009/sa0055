#include "integralformulaform.h"
#include "descriptioncolumndelegate.h"
#include "enumtablemanager.h"
#include "foreignkeycolumndelegate.h"
#include "integercolumndelegate.h"
#include "integraldefinitiontablemanager.h"
#include "maptablemanager.h"
#include "selectpointcolumndelegate.h"
#include "ui_integralformulaform.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlTableModel>

IntegralFormulaForm::IntegralFormulaForm(QWidget *parent) : QWidget(parent), ui(new Ui::IntegralFormulaForm)
{
    ui->setupUi(this);

    QSqlTableModel *model = IntegralDefinitionTableManager::integralDefTableManager()->model();
    model->setHeaderData(IntegralDefinitionTableManager::ColumnIndex::IntegralId, Qt::Orientation::Horizontal, "积分量名称");
    model->setHeaderData(IntegralDefinitionTableManager::ColumnIndex::AiId, Qt::Orientation::Horizontal, "遥测量");
    model->setHeaderData(IntegralDefinitionTableManager::ColumnIndex::SamplingInterval, Qt::Orientation::Horizontal, "采样间隔(秒)");
    model->setHeaderData(IntegralDefinitionTableManager::ColumnIndex::WriteDbInterval, Qt::Orientation::Horizontal, "写库间隔(秒)");
    model->setHeaderData(IntegralDefinitionTableManager::ColumnIndex::ClearFlag, Qt::Orientation::Horizontal, "是否清零");

    ui->tableView->setModel(model);

    ui->tableView->setItemDelegateForColumn(
        IntegralDefinitionTableManager::ColumnIndex::IntegralId,
        new DescriptionColumnDelegate(MapTableManager::mapTableManager(MapTableManager::TableIndex::INTEGRAL), this));

    ui->tableView->setItemDelegateForColumn(IntegralDefinitionTableManager::ColumnIndex::SamplingInterval, new IntegerColumnDelegate(0, 65535, this));

    ui->tableView->setItemDelegateForColumn(IntegralDefinitionTableManager::ColumnIndex::WriteDbInterval,
                                            new IntegerColumnDelegate(0, 99999999, this));

    ui->tableView->setItemDelegateForColumn(
        IntegralDefinitionTableManager::ColumnIndex::AiId,
        new SelectPointColumnDelegate(this, SelectPoint::Type::AI, IntegralDefinitionTableManager::ColumnIndex::DeviceId));
    ui->tableView->setItemDelegateForColumn(IntegralDefinitionTableManager::ColumnIndex::ClearFlag,
                                            new ForeignKeyColumnDelegate(EnumTableManager::boolEnumManager(), true, this));

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);

    ui->tableView->setColumnHidden(IntegralDefinitionTableManager::ColumnIndex::SeqNum, true);
    ui->tableView->setColumnHidden(IntegralDefinitionTableManager::ColumnIndex::DeviceId, true);

    connect(ui->addPushButton, &QPushButton::clicked, this, &IntegralFormulaForm::addIntegralRecord);
    connect(ui->deletePushButton, &QPushButton::clicked, this, &IntegralFormulaForm::deleteIntegralRecord);
}

IntegralFormulaForm::~IntegralFormulaForm()
{
    delete ui;
}

void IntegralFormulaForm::hideEvent(QHideEvent */*event*/)
{
    DEBUG << configValidCheck();
}

void IntegralFormulaForm::addIntegralRecord()
{
    forever
    {
        bool ok;
        int num = IntegralDefinitionTableManager::integralDefTableManager()->model()->rowCount() + 1;
        QString defaultName = QString("%1#%2").arg("积分量").arg(num);
        QString IntegralVarName = QInputDialog::getText(this, "集分量", "请输入积分量名称:", QLineEdit::EchoMode::Normal, defaultName, &ok);
        if (!ok)
        {
            break;
        }

        if (IntegralVarName.isEmpty())
        {
            QMessageBox::warning(this, "提示", "输入的集分量名称不能为空!");
            continue;
        }

        int integralId = MapTableManager::mapTableManager(MapTableManager::TableIndex::INTEGRAL)->addDesc(IntegralVarName);
        if (integralId == MapTableManager::Error::AlreadyExist)
        {
            QMessageBox::warning(this, "提示", "输入的集分量名称已经被使用!");
        }
        else
        {
            IntegralDefinitionTableManager::integralDefTableManager()->addNewRecord(integralId);
            break;
        }
    }
}

void IntegralFormulaForm::deleteIntegralRecord()
{
    QModelIndexList list = ui->tableView->selectionModel()->selectedRows();
    if (list.size() < 1)
    {
        QMessageBox::information(this, "提示", "请选择一条记录");
        return;
    }

    IntegralDefinitionTableManager::integralDefTableManager()->deleteRecord(list.at(0).row(), list.size());
}

QStringList IntegralFormulaForm::configValidCheck()
{
    QStringList errorList;

    bool ok;
    QSqlTableModel *model = IntegralDefinitionTableManager::integralDefTableManager()->model();
    for (int i = 0; i < model->rowCount(); i++)
    {
        int deviceId = model->index(i, IntegralDefinitionTableManager::ColumnIndex::DeviceId).data().toInt(&ok);
        int aiId = model->index(i, IntegralDefinitionTableManager::ColumnIndex::AiId).data().toInt(&ok);

        if (MapTableManager::mapTableManager(MapTableManager::TableIndex::DEV)->nameFromId(deviceId).isEmpty() ||
            MapTableManager::mapTableManager(MapTableManager::TableIndex::AI)->nameFromId(aiId).isEmpty())
        {
            errorList << QString("第%1条积分量未配置遥测量!").arg(i+1);
        }
    }

    return errorList;
}
