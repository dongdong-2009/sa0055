#include "deviceparaform.h"
#include "comboboxmapperdelegate.h"
#include "constvalue.h"
#include "deviceparatablemanager.h"
#include "enumtablemanager.h"
#include "logutil.h"
#include "maptablemanager.h"
#include "ui_deviceparaform.h"
#include "validatormanager.h"
#include <QDataWidgetMapper>
#include <QSqlError>
#include <QSqlTableModel>

void DeviceParaForm::initDevicePara()
{
    createDataWidgetMapper4Device();

    ui->deviceNameLineEdit->setEnabled(false);

    _setUnusedWidgetDisable();
    connect(ui->deviceRunModeComboBox, &QComboBox::currentTextChanged, [=]() { _setUnusedWidgetDisable(); });
    connect(ui->networkModeComboBox, &QComboBox::currentTextChanged, [=]() { _setUnusedWidgetDisable(); });
}

void DeviceParaForm::refreshDevicePara()
{
    deviceParaMapper->setCurrentIndex(DeviceParaTableManager::deviceParaTableManager()->rowCountFromId(deviceId));
    ui->deviceNameLineEdit->setText(MapTableManager::mapTableManager(MapTableManager::TableIndex::DEV)->nameFromId(deviceId));
}

void DeviceParaForm::saveDevicePara()
{
    DeviceParaTableManager *mgr = DeviceParaTableManager::deviceParaTableManager();

    if (!deviceParaMapper->submit() && mgr->model()->lastError().type() != QSqlError::ErrorType::NoError)
    {
        ERROR << QString("call %1 fail, error type:%2, errorString: %3")
                     .arg("submit")
                     .arg(SqlErrorTypeStr[mgr->model()->lastError().type()])
                     .arg(mgr->model()->lastError().text());
    }

    mgr->submitData();
}

void DeviceParaForm::_setUnusedWidgetDisable()
{
    if (ui->deviceRunModeComboBox->currentIndex() == RunMode::Single)
    {
        ui->device2IpALineEdit->setEnabled(false);
        ui->device2IpBLineEdit->setEnabled(false);

        if (ui->networkModeComboBox->currentIndex() == RunMode::Single)
        {
            ui->device1IpBLineEdit->setEnabled(false);
        }
        else
        {
            ui->device1IpBLineEdit->setEnabled(true);
        }
    }
    else
    {
        ui->device2IpALineEdit->setEnabled(true);

        if (ui->networkModeComboBox->currentIndex() == RunMode::Single)
        {
            ui->device1IpBLineEdit->setEnabled(false);
            ui->device2IpBLineEdit->setEnabled(false);
        }
        else
        {
            ui->device1IpBLineEdit->setEnabled(true);
            ui->device2IpBLineEdit->setEnabled(true);
        }
    }
}

void DeviceParaForm::createDataWidgetMapper4Device()
{
    ui->deviceRunModeComboBox->setModel(EnumTableManager::deviceRunModeEnumManager()->model());
    ui->deviceRunModeComboBox->setModelColumn(EnumTableManager::deviceRunModeEnumManager()->nameColumnIndex());
    ui->networkModeComboBox->setModel(EnumTableManager::networkModeEnumManager()->model());
    ui->networkModeComboBox->setModelColumn(EnumTableManager::networkModeEnumManager()->nameColumnIndex());

    deviceParaMapper = new QDataWidgetMapper(this);
    deviceParaMapper->setSubmitPolicy(QDataWidgetMapper::SubmitPolicy::ManualSubmit);
    deviceParaMapper->setModel(DeviceParaTableManager::deviceParaTableManager()->model());
    deviceParaMapper->setItemDelegate(new ComboBoxMapperDelegate(this));
    deviceParaMapper->addMapping(ui->device1IpALineEdit, DeviceParaTableManager::ColumnIndex::Dev1IpA);
    deviceParaMapper->addMapping(ui->device1IpBLineEdit, DeviceParaTableManager::ColumnIndex::Dev1IpB);
    deviceParaMapper->addMapping(ui->device2IpALineEdit, DeviceParaTableManager::ColumnIndex::Dev2IpA);
    deviceParaMapper->addMapping(ui->device2IpBLineEdit, DeviceParaTableManager::ColumnIndex::Dev2IpB);
    deviceParaMapper->addMapping(ui->deviceRunModeComboBox, DeviceParaTableManager::ColumnIndex::DeviceRunMode);
    deviceParaMapper->addMapping(ui->networkModeComboBox, DeviceParaTableManager::ColumnIndex::NetworkMode);

    ui->device1IpALineEdit->setValidator(ValidatorManager::ipValidator());
    ui->device1IpBLineEdit->setValidator(ValidatorManager::ipValidator());
    ui->device2IpALineEdit->setValidator(ValidatorManager::ipValidator());
    ui->device2IpBLineEdit->setValidator(ValidatorManager::ipValidator());
}

QStringList DeviceParaForm::deviceConfigValidCheck()
{
    QStringList errorList;

    if (!ui->device1IpALineEdit->hasAcceptableInput() || ui->device1IpALineEdit->text() == DEFAULT_IP)
    {
        errorList << QString("装置的IP地址A不正确!");
    }

    if (ui->deviceRunModeComboBox->currentIndex() == RunMode::Single)
    {
        if (ui->networkModeComboBox->currentIndex() == RunMode::Single)
        {
        }
        else
        {
            if (!ui->device1IpBLineEdit->hasAcceptableInput() || ui->device1IpBLineEdit->text() == DEFAULT_IP)
            {
                errorList << QString("装置的IP地址B不正确!");
            }
        }
    }
    else
    {
        if (!ui->device2IpALineEdit->hasAcceptableInput() || ui->device2IpALineEdit->text() == DEFAULT_IP)
        {
            errorList << QString("冗余装置的IP地址A不正确!");
        }

        if (ui->networkModeComboBox->currentIndex() == RunMode::Single)
        {
        }
        else
        {
            if (!ui->device1IpBLineEdit->hasAcceptableInput() || ui->device1IpBLineEdit->text() == DEFAULT_IP)
            {
                errorList << QString("装置的IP地址B不正确!");
            }

            if (!ui->device2IpBLineEdit->hasAcceptableInput() || ui->device2IpBLineEdit->text() == DEFAULT_IP)
            {
                errorList << QString("冗余装置的IP地址B不正确!");
            }
        }
    }

    return errorList;
}
