#include "newdevicedialog.h"
#include "constvalue.h"
#include "maptablemanager.h"
#include "ui_newdevicedialog.h"
#include <QMessageBox>

NewDeviceDialog::NewDeviceDialog(QWidget *parent) : QDialog(parent), ui(new Ui::NewDeviceDialog)
{
    ui->setupUi(this);

    int num = MapTableManager::mapTableManager(MapTableManager::TableIndex::DEV)->currentMaxId() - Desc_ID_Start + 1;
    QString defaultDeviceName = QString("装置#%1").arg(num);
    ui->deviceNameLineEdit->setText(defaultDeviceName);

    connect(ui->okPushButton, &QPushButton::clicked, [=]() {

        int deviceId = MapTableManager::mapTableManager(MapTableManager::TableIndex::DEV)->addDesc(ui->deviceNameLineEdit->text());
        if (deviceId == MapTableManager::Error::EmptyDescr)
        {
            QMessageBox::warning(this, "提示", "输入的装置名称不能为空!");
            return;
        }
        else if (deviceId == MapTableManager::Error::AlreadyExist)
        {
            QMessageBox::warning(this, "提示", "输入的装置名称已经被使用!");
            return;
        }
        else
        {
            emit createDeviceOk(deviceId);
            accept();
        }
    });

    connect(ui->cancelPushButton, &QPushButton::clicked, [=]() { reject(); });
}

NewDeviceDialog::~NewDeviceDialog()
{
    delete ui;
}
