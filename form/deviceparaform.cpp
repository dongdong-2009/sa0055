#include "deviceparaform.h"
#include "comboboxmapperdelegate.h"
#include "deviceparatablemanager.h"
#include "logutil.h"
#include "maptablemanager.h"
#include "ui_deviceparaform.h"
#include "validatormanager.h"
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QSqlTableModel>

DeviceParaForm::~DeviceParaForm()
{
    delete ui;
}

DeviceParaForm::DeviceParaForm(QWidget *parent) : QWidget(parent), ui(new Ui::DeviceParaForm), deviceId(-1)
{
    ui->setupUi(this);

    initDevicePara();
    initInfoTable();
    initIec104Para();
}

void DeviceParaForm::setCurrentDeviceId(int deviceId)
{
    if (this->deviceId != deviceId)
    {

        QString deviceName = MapTableManager::mapTableManager(MapTableManager::DEV)->nameFromId(this->deviceId);
        ui->deviceNameLineEdit->setText(deviceName);

        if (this->deviceId != -1)
        {
            saveData();
        }

        this->deviceId = deviceId;
        refreshData();
    }
}

void DeviceParaForm::hideEvent(QHideEvent * /*event*/)
{
    DEBUG << configValidCheck();
    saveData();
}

void DeviceParaForm::showEvent(QShowEvent * /*event*/)
{
    refreshData();
}

void DeviceParaForm::saveData()
{
    saveDevicePara();
    saveInfoTable();
    saveIec104Para();
}

void DeviceParaForm::refreshData()
{
    refreshDevicePara();
    refreshInfoTable();
    refreshIec104Para();
}

QStringList DeviceParaForm::configValidCheck()
{
    QStringList errorList;
    errorList << deviceConfigValidCheck() << iec104ParaConfigValidCheck() << infoTableConfigValidCheck();

    return errorList;
}
