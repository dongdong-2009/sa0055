#include "deviceparaform.h"
#include "comboboxmapperdelegate.h"
#include "datatype.h"
#include "enumtablemanager.h"
#include "iec104paratablemanager.h"
#include "logutil.h"
#include "ui_deviceparaform.h"
#include "util.h"
#include <QDataWidgetMapper>
#include <QSqlError>
#include <QSqlTableModel>

void DeviceParaForm::initIec104Para()
{
    typeIdComboBox[DataType::DI] = ui->diTypeIdComboBox;
    typeIdComboBox[DataType::AI] = ui->aiTypeIdComboBox;
    typeIdComboBox[DataType::DO] = ui->doTypeIdComboBox;
    typeIdComboBox[DataType::AO] = ui->aoTypeIdComboBox;
    typeIdComboBox[DataType::CI] = ui->ciTypeIdComboBox;

    createDataWidgetMapper4Iec104();
}

void DeviceParaForm::createDataWidgetMapper4Iec104()
{
    for (int i = 0; i < DataType::DataTypeNum; i++)
    {
        typeIdComboBox[i]->setModel(EnumTableManager::typeIdEnumManager(Util::integer_2_dataType(i))->model());
        typeIdComboBox[i]->setModelColumn(EnumTableManager::typeIdEnumManager(Util::integer_2_dataType(i))->nameColumnIndex());
    }

    ui->cotByteNumComboBox->setModel(EnumTableManager::byteNumEnumManager(EnumTableManager::Iec104ByteNumType::COT)->model());
    ui->cotByteNumComboBox->setModelColumn(EnumTableManager::byteNumEnumManager(EnumTableManager::Iec104ByteNumType::COT)->nameColumnIndex());

    ui->asduByteNumComboBox->setModel(EnumTableManager::byteNumEnumManager(EnumTableManager::Iec104ByteNumType::CommonAddr)->model());
    ui->asduByteNumComboBox->setModelColumn(EnumTableManager::byteNumEnumManager(EnumTableManager::Iec104ByteNumType::CommonAddr)->nameColumnIndex());

    ui->ioaByteNumComboBox->setModel(EnumTableManager::byteNumEnumManager(EnumTableManager::Iec104ByteNumType::IOA)->model());
    ui->ioaByteNumComboBox->setModelColumn(EnumTableManager::byteNumEnumManager(EnumTableManager::Iec104ByteNumType::IOA)->nameColumnIndex());

    iec104ParaMapper = new QDataWidgetMapper(this);
    iec104ParaMapper->setSubmitPolicy(QDataWidgetMapper::SubmitPolicy::ManualSubmit);
    iec104ParaMapper->setModel(Iec104ParaTableManager::iec104ParaTableManager()->model());
    iec104ParaMapper->setItemDelegate(new ComboBoxMapperDelegate(this));

    iec104ParaMapper->addMapping(ui->asduAddrSpinBox, Iec104ParaTableManager::ColumnIndex::AsduAddr);
    iec104ParaMapper->addMapping(ui->cotByteNumComboBox, Iec104ParaTableManager::ColumnIndex::CotSize);
    iec104ParaMapper->addMapping(ui->asduByteNumComboBox, Iec104ParaTableManager::ColumnIndex::AsduSize);
    iec104ParaMapper->addMapping(ui->ioaByteNumComboBox, Iec104ParaTableManager::ColumnIndex::IoaSize);
    iec104ParaMapper->addMapping(ui->diStartAddrSpinBox, Iec104ParaTableManager::ColumnIndex::DiIoaStart);
    iec104ParaMapper->addMapping(ui->aiStartAddrSpinBox, Iec104ParaTableManager::ColumnIndex::AiIoaStart);
    iec104ParaMapper->addMapping(ui->doStartAddrSpinBox, Iec104ParaTableManager::ColumnIndex::DoIoaStart);
    iec104ParaMapper->addMapping(ui->aoStartAddrSpinBox, Iec104ParaTableManager::ColumnIndex::AoIoaStart);
    iec104ParaMapper->addMapping(ui->ciStartAddrSpinBox, Iec104ParaTableManager::ColumnIndex::CiIoaStart);

    iec104ParaMapper->addMapping(ui->diTypeIdComboBox, Iec104ParaTableManager::ColumnIndex::DiTypeId);
    iec104ParaMapper->addMapping(ui->aiTypeIdComboBox, Iec104ParaTableManager::ColumnIndex::AiTypeId);
    iec104ParaMapper->addMapping(ui->doTypeIdComboBox, Iec104ParaTableManager::ColumnIndex::DoTypeId);
    iec104ParaMapper->addMapping(ui->aoTypeIdComboBox, Iec104ParaTableManager::ColumnIndex::AoTypeId);
    iec104ParaMapper->addMapping(ui->ciTypeIdComboBox, Iec104ParaTableManager::ColumnIndex::CiTypeId);
}

void DeviceParaForm::refreshIec104Para()
{
    iec104ParaMapper->setCurrentIndex(Iec104ParaTableManager::iec104ParaTableManager()->rowCountFromId(deviceId));
}

void DeviceParaForm::saveIec104Para()
{
    Iec104ParaTableManager *mgr = Iec104ParaTableManager::iec104ParaTableManager();
    if (!iec104ParaMapper->submit() && mgr->model()->lastError().type() != QSqlError::ErrorType::NoError)
    {
        ERROR << QString("call %1 fail, error type: %2, error String:%3")
                     .arg("submit")
                     .arg(SqlErrorTypeStr[mgr->model()->lastError().type()])
                     .arg(mgr->model()->lastError().text());
    }

    mgr->submitData();
}

QStringList DeviceParaForm::iec104ParaConfigValidCheck()
{
    QStringList errorList;

    return errorList;
}
