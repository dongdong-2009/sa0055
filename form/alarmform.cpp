#include "eventalarmform.h"
#include "alarminfotablemanager.h"
#include "alarmruletablemanager.h"
#include "comboboxitemdelegate.h"
#include "comboboxmapperdelegate.h"
#include "doublecolumndelegate.h"
#include "enumtablemanager.h"
#include "enumtablemanager.h"
#include "foreignkeycolumndelegate.h"
#include "integercolumndelegate.h"
#include "maptablemanager.h"
#include "picturefiletablemanager.h"
#include "selectcolorcolumndelegate.h"
#include "selectdevicecolumndelegate.h"
#include "selectdevicedialog.h"
#include "selectpointcolumndelegate.h"
#include "selectpointdialog.h"
#include "soundfiletablemanager.h"
#include "ui_alarmform.h"
#include "util.h"
#include <QDataWidgetMapper>
#include <QMessageBox>

EventAlarmForm::EventAlarmForm(QWidget *parent) : QWidget(parent), ui(new Ui::EventAlarmForm), alarmRuleId(-1)
{
    ui->setupUi(this);
    prepareData();

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(AlarmRuleTableManager::alarmRuleTableManager()->model());

    mapper->setItemDelegate(new ComboBoxMapperDelegate(this));
    mapper->addMapping(ui->alarmCauseComboBox, AlarmRuleTableManager::ColumnIndex::AlarmCauseId);
    mapper->addMapping(ui->alarmTypeComboBox, AlarmRuleTableManager::ColumnIndex::AlarmType);
    mapper->addMapping(ui->cosDisplayComboBox, AlarmRuleTableManager::ColumnIndex::CosDisplay);

    connect(ui->addPushButton, &QPushButton::clicked, this, &EventAlarmForm::addNewBindRecord);
    connect(ui->deletePushButton, &QPushButton::clicked, [=]() {
        QModelIndexList list = ui->tableView->selectionModel()->selectedRows();
        if (list.size() < 1)
        {
            QMessageBox::information(this, "提示", "请选择一条记录");
            return;
        }

        if (QMessageBox::question(this, "提示", "删除选中的记录，是否继续?") == QMessageBox::StandardButton::Yes)
        {
            AlarmInfoTableManager::alarmInfoTableManager()->deleteRecord(list.at(0).row(), list.size());
        }
    });

    connect(ui->subCauseListWidget, &ExListWidget::valueChanged, [=](int value){
        AlarmRuleTableManager::alarmRuleTableManager()->setAlarmSubCause(alarmRuleId, value);
    });
}

void EventAlarmForm::addNewBindRecord()
{
    if (selectDeviceFlag)
    {
        SelectDeviceDialog dialog(this, true);
        connect(&dialog, &SelectDeviceDialog::selectedOk, [=](QList<int> deviceIdList) {
            QSqlTableModel *model = AlarmInfoTableManager::alarmInfoTableManager()->model();
            bool ok;
            QList<SelectPoint> selectPointList;

            foreach (int deviceId, deviceIdList)
            {
                bool found = false;
                for (int i = 0; i < model->rowCount(); i++)
                {
                    int devId = model->index(i, AlarmInfoTableManager::ColumnIndex::DeviceId).data().toInt(&ok);
                    if (!ok)
                    {
                        ERROR << QString("call %1 fail").arg("toInt");
                    }

                    if (devId == deviceId)
                    {
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    selectPointList << SelectPoint(DataType::DataTypeNum, deviceId, -1);
                }
            }

            AlarmInfoTableManager::alarmInfoTableManager()->addNewRecord(alarmRuleId, selectPointList);
        });

        dialog.exec();
        return;
    }
    else if (selectPointType != SelectPoint::Type::INVALID)
    {

        SelectPointDialog dialog(this, selectPointType, SelectPoint::Mode::Multiple);
        connect(&dialog, &SelectPointDialog::selectedOk, [=](QList<SelectPoint> list) {
            QSqlTableModel *model = AlarmInfoTableManager::alarmInfoTableManager()->model();
            bool ok;
            QList<SelectPoint> selectPointList;

            foreach (SelectPoint p, list)
            {
                bool found = false;
                for (int i = 0; i < model->rowCount(); i++)
                {
                    int devId = model->index(i, AlarmInfoTableManager::ColumnIndex::DeviceId).data().toInt(&ok);
                    if (!ok)
                    {
                        ERROR << QString("call %1 fail").arg("toInt");
                    }

                    int pointDescId = model->index(i, AlarmInfoTableManager::ColumnIndex::PointDescId).data().toInt(&ok);
                    if (!ok)
                    {
                        ERROR << QString("call %1 fail").arg("toInt");
                    }

                    if (devId == p.getDeviceId() && pointDescId == p.getPointDescId())
                    {
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    selectPointList << SelectPoint(p.getType(), p.getDeviceId(), p.getPointDescId());
                }
            }

            AlarmInfoTableManager::alarmInfoTableManager()->addNewRecord(alarmRuleId, selectPointList);
        });

        dialog.exec();
    }
}

void EventAlarmForm::prepareData()
{
    ui->alarmCauseComboBox->setModel(AlarmCauseTableManager::alarmCauseTableManager()->model());
    ui->alarmCauseComboBox->setModelColumn(AlarmCauseTableManager::alarmCauseTableManager()->nameColumnIndex());

    ui->alarmTypeComboBox->setModel(EnumTableManager::alarmTypeEnumManager()->model());
    ui->alarmTypeComboBox->setModelColumn(EnumTableManager::alarmTypeEnumManager()->nameColumnIndex());

    ui->cosDisplayComboBox->setModel(EnumTableManager::cosDisplayEnumManager()->model());
    ui->cosDisplayComboBox->setModelColumn(EnumTableManager::cosDisplayEnumManager()->nameColumnIndex());

    QSqlTableModel *model = AlarmInfoTableManager::alarmInfoTableManager()->model();
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::DeviceId, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::DeviceId]);
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::PointDescId, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::PointDescId]);
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::AiUpperLimit, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::AiUpperLimit]);
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::AiLowerLimit, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::AiLowerLimit]);
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::AiUpperUpperLimit, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::AiUpperUpperLimit]);
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::AiLowerLowerLimit, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::AiLowerLowerLimit]);
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::MustConfirmFlag, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::MustConfirmFlag]);
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::UnconfirmedColor, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::UnconfirmedColor]);
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::ConfirmedColor, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::ConfirmedColor]);
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::SoundFilePath, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::SoundFilePath]);
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::SoundPlayTime, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::SoundPlayTime]);
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::PrintAlarmFlag, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::PrintAlarmFlag]);
    model->setHeaderData(AlarmInfoTableManager::ColumnIndex::PushPicturePath, Qt::Orientation::Horizontal,
                         AlarmInfoTableManager::ColumnName[AlarmInfoTableManager::ColumnIndex::PushPicturePath]);

    ui->tableView->setModel(model);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);

    /*Column Delegate*/
    ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::DeviceId, new SelectDeviceColumnDelegate(this));

    ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::AiUpperLimit, new DoubleColumnDelegate(-32768, 32767, 6, this));
    ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::AiLowerLimit, new DoubleColumnDelegate(-32768, 32767, 6, this));
    ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::AiUpperUpperLimit,
                                            new DoubleColumnDelegate(-99999999, 99999999, 6, this));
    ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::AiLowerLowerLimit,
                                            new DoubleColumnDelegate(-99999999, 99999999, 6, this));
    ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::MustConfirmFlag,
                                            new ForeignKeyColumnDelegate(EnumTableManager::boolEnumManager(), true, this));

    ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::SoundFilePath,
                                            new ComboBoxItemDelegate(SoundFileTableManager::soundFileTableManager()->model(), this));

    ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::PushPicturePath,
                                            new ComboBoxItemDelegate(PictureFileTableManager::pictureFileTableManager()->model(), this));

    ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::SoundPlayTime, new IntegerColumnDelegate(0, 100, this));
    ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::PrintAlarmFlag,
                                            new ForeignKeyColumnDelegate(EnumTableManager::boolEnumManager(), true, this));
    ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::UnconfirmedColor, new SelectColorColumnDelegate(this));
    ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::ConfirmedColor, new SelectColorColumnDelegate(this));

    ui->tableView->setColumnHidden(AlarmInfoTableManager::ColumnIndex::SeqNum, true);
    ui->tableView->setColumnHidden(AlarmInfoTableManager::ColumnIndex::AlarmRuleId, true);
    ui->tableView->setColumnHidden(AlarmInfoTableManager::ColumnIndex::PointType, true);

    selectPointDelegate[DataType::DI] =
        new SelectPointColumnDelegate(this, SelectPoint::Type::DI, AlarmInfoTableManager::DeviceId, AlarmInfoTableManager::PointType, false, false);
    selectPointDelegate[DataType::AI] =
        new SelectPointColumnDelegate(this, SelectPoint::Type::AI, AlarmInfoTableManager::DeviceId, AlarmInfoTableManager::PointType, false, false);
    selectPointDelegate[DataType::DO] =
        new SelectPointColumnDelegate(this, SelectPoint::Type::DO, AlarmInfoTableManager::DeviceId, AlarmInfoTableManager::PointType, false, false);
    selectPointDelegate[DataType::AO] =
        new SelectPointColumnDelegate(this, SelectPoint::Type::AO, AlarmInfoTableManager::DeviceId, AlarmInfoTableManager::PointType, false, false);

    selectDeviceDelegate = new SelectDeviceColumnDelegate(this);

    connect(ui->tableView, &QAbstractItemView::pressed, this, &EventAlarmForm::batchSetValue);
}

EventAlarmForm::~EventAlarmForm()
{
    delete ui;
}

void EventAlarmForm::setCurrentAlarmRuleId(int alarmRuleId)
{
    if (this->alarmRuleId != -1 && this->alarmRuleId != alarmRuleId)
    {
        saveData();
    }

    this->alarmRuleId = alarmRuleId;
    ui->alarmRuleNameLineEdit->setText(MapTableManager::mapTableManager(MapTableManager::TableIndex::ALARMRULE)->nameFromId(alarmRuleId));

    int rowNum;
    int alarmCauseId;
    int alarmSubCauseValue;

    AlarmRuleTableManager::alarmRuleTableManager()->getAlarmRuleInfo(alarmRuleId, &alarmCauseId, &alarmSubCauseValue, &rowNum);
    mapper->setCurrentIndex(rowNum);

    QString alarmSubCauseStrList;
    AlarmCauseTableManager::BindPointType bindPointType;

    AlarmCauseTableManager::alarmCauseTableManager()->getAlarmCauseInfo(alarmCauseId, &alarmSubCauseStrList, &bindPointType);
    ui->subCauseListWidget->initializeListItem(alarmSubCauseStrList);
    ui->subCauseListWidget->setValue(alarmSubCauseValue);

    if (alarmCauseId == AlarmRuleTableManager::AlarmCause::COS)
    {
        ui->cosDisplayComboBox->setVisible(true);
        ui->cosDisplayLabel->setVisible(true);
    }
    else
    {
        ui->cosDisplayComboBox->setVisible(false);
        ui->cosDisplayLabel->setVisible(false);
    }

    updateBindTable(bindPointType, alarmCauseId);
    AlarmInfoTableManager::alarmInfoTableManager()->fetchData(alarmRuleId);
}

void EventAlarmForm::hideEvent(QHideEvent * /*event*/)
{
    saveData();
}

void EventAlarmForm::saveData()
{
    mapper->submit();
    AlarmInfoTableManager::alarmInfoTableManager()->model()->submitAll();
}

void EventAlarmForm::updateBindTable(AlarmCauseTableManager::BindPointType bindPointType, int alarmCauseId)
{
    ui->groupBox->setEnabled(true);
    ui->tableView->setColumnHidden(AlarmInfoTableManager::DeviceId, true);
    ui->tableView->setColumnHidden(AlarmInfoTableManager::PointDescId, true);

    ui->tableView->setColumnHidden(AlarmInfoTableManager::AiUpperLimit, true);
    ui->tableView->setColumnHidden(AlarmInfoTableManager::AiLowerLimit, true);
    ui->tableView->setColumnHidden(AlarmInfoTableManager::AiUpperUpperLimit, true);
    ui->tableView->setColumnHidden(AlarmInfoTableManager::AiLowerLowerLimit, true);

    selectDeviceFlag = false;
    selectPointType = SelectPoint::Type::INVALID;

    switch (bindPointType)
    {
    case AlarmCauseTableManager::BindPointType::DI:
        selectPointType = SelectPoint::Type::DI;
        ui->tableView->setColumnHidden(AlarmInfoTableManager::PointDescId, false);
        break;
    case AlarmCauseTableManager::BindPointType::AI:
        selectPointType = SelectPoint::Type::AI;
        ui->tableView->setColumnHidden(AlarmInfoTableManager::PointDescId, false);

        if (alarmCauseId == AlarmRuleTableManager::AlarmCause::AIOutOfLimit)
        {
            ui->tableView->setColumnHidden(AlarmInfoTableManager::AiUpperLimit, false);
            ui->tableView->setColumnHidden(AlarmInfoTableManager::AiLowerLimit, false);
            ui->tableView->setColumnHidden(AlarmInfoTableManager::AiUpperUpperLimit, false);
            ui->tableView->setColumnHidden(AlarmInfoTableManager::AiLowerLowerLimit, false);
        }

        break;
    case AlarmCauseTableManager::BindPointType::DO:
        selectPointType = SelectPoint::Type::DO;
        ui->tableView->setColumnHidden(AlarmInfoTableManager::PointDescId, false);
        break;
    case AlarmCauseTableManager::BindPointType::AO:
        selectPointType = SelectPoint::Type::AO;
        ui->tableView->setColumnHidden(AlarmInfoTableManager::PointDescId, false);
        break;
    case AlarmCauseTableManager::BindPointType::CI:
        Q_UNREACHABLE();
        break;
    case AlarmCauseTableManager::BindPointType::DEV:
        ui->tableView->setColumnHidden(AlarmInfoTableManager::DeviceId, false);
        selectDeviceFlag = true;
        break;
    case AlarmCauseTableManager::BindPointType::None:
        ui->groupBox->setEnabled(false);
        break;
    }

    if (selectDeviceFlag)
    {
        ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::DeviceId, selectDeviceDelegate);
    }
    else if (selectPointType != SelectPoint::Type::INVALID)
    {
        ui->tableView->setItemDelegateForColumn(AlarmInfoTableManager::ColumnIndex::PointDescId,
                                                selectPointDelegate[Util::selectPointType_2_dataType(selectPointType)]);
    }
}
