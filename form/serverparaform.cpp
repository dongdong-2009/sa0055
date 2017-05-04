#include "serverparaform.h"
#include "comboboxmapperdelegate.h"
#include "constvalue.h"
#include "datatype.h"
#include "enumtablemanager.h"
#include "logutil.h"
#include "serverparatablemanager.h"
#include "ui_serverparaform.h"
#include "validatormanager.h"
#include <QDataWidgetMapper>
#include <QSqlError>
#include <QSqlTableModel>

ServerParaForm::ServerParaForm(QWidget *parent) : QWidget(parent), ui(new Ui::ServerParaForm)
{
    ui->setupUi(this);

    _createDataWidgetMapper();
    _setUnusedWidgetDisable();
    connect(ui->serverRunModeComboBox, &QComboBox::currentTextChanged, [=]() { _setUnusedWidgetDisable(); });
    connect(ui->networkModeComboBox, &QComboBox::currentTextChanged, [=]() { _setUnusedWidgetDisable(); });
}

ServerParaForm::~ServerParaForm()
{
    delete ui;
}

void ServerParaForm::showEvent(QShowEvent * /*event*/)
{
    mapper->toFirst();
}

void ServerParaForm::hideEvent(QHideEvent * /*event*/)
{
    DEBUG << configValidCheck();
    saveData();
}

void ServerParaForm::saveData()
{
    ServerParaTableManager *mgr = ServerParaTableManager::serverParaTableManager();

    if (!mapper->submit())
    {
        ERROR << QString("call %1 fail, error type:%2, error text: %3")
                     .arg("submit")
                     .arg(SqlErrorTypeStr[mgr->model()->lastError().type()])
                     .arg(mgr->model()->lastError().text());
    }

    mgr->submitData();
}

void ServerParaForm::_setUnusedWidgetDisable()
{
    if (ui->serverRunModeComboBox->currentIndex() == RunMode::Single)
    {
        ui->server2IpALineEdit->setEnabled(false);
        ui->server2IpBLineEdit->setEnabled(false);

        ui->virtualIpALineEdit->setEnabled(false);
        ui->virtualIpBLineEdit->setEnabled(false);

        if (ui->networkModeComboBox->currentIndex() == RunMode::Single)
        {
            ui->server1IpBLineEdit->setEnabled(false);
        }
        else
        {
            ui->server1IpBLineEdit->setEnabled(true);
        }
    }
    else
    {
        ui->server2IpALineEdit->setEnabled(true);
        ui->virtualIpALineEdit->setEnabled(true);

        if (ui->networkModeComboBox->currentIndex() == RunMode::Single)
        {
            ui->server1IpBLineEdit->setEnabled(false);
            ui->server2IpBLineEdit->setEnabled(false);
            ui->virtualIpBLineEdit->setEnabled(false);
        }
        else
        {
            ui->server1IpBLineEdit->setEnabled(true);
            ui->server2IpBLineEdit->setEnabled(true);
            ui->virtualIpBLineEdit->setEnabled(true);
        }
    }
}

void ServerParaForm::_createDataWidgetMapper()
{
    Q_ASSERT(EnumTableManager::serverRunModeEnumManager()->model()->rowCount() == 2);
    Q_ASSERT(EnumTableManager::networkModeEnumManager()->model()->rowCount() == 2);

    DEBUG << ServerParaTableManager::serverParaTableManager()->model()->index(0, ServerParaTableManager::ColumnIndex::ServerRunMode).data();

    ui->serverRunModeComboBox->setModel(EnumTableManager::serverRunModeEnumManager()->model());
    ui->serverRunModeComboBox->setModelColumn(EnumTableManager::serverRunModeEnumManager()->nameColumnIndex());
    ui->networkModeComboBox->setModel(EnumTableManager::networkModeEnumManager()->model());
    ui->networkModeComboBox->setModelColumn(EnumTableManager::networkModeEnumManager()->nameColumnIndex());

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::SubmitPolicy::ManualSubmit);
    mapper->setModel(ServerParaTableManager::serverParaTableManager()->model());
    mapper->setItemDelegate(new ComboBoxMapperDelegate(this));

    mapper->addMapping(ui->serverRunModeComboBox, ServerParaTableManager::ColumnIndex::ServerRunMode);
    mapper->addMapping(ui->networkModeComboBox, ServerParaTableManager::ColumnIndex::NetworkMode);
    mapper->addMapping(ui->server1IpALineEdit, ServerParaTableManager::ColumnIndex::Server1IpA);
    mapper->addMapping(ui->server1IpBLineEdit, ServerParaTableManager::ColumnIndex::Server1IpB);
    mapper->addMapping(ui->server2IpALineEdit, ServerParaTableManager::ColumnIndex::Server2IpA);
    mapper->addMapping(ui->server2IpBLineEdit, ServerParaTableManager::ColumnIndex::Server2IpB);
    mapper->addMapping(ui->virtualIpALineEdit, ServerParaTableManager::ColumnIndex::VirtualIpA);
    mapper->addMapping(ui->virtualIpBLineEdit, ServerParaTableManager::ColumnIndex::VirtualIpB);

    ui->server1IpALineEdit->setValidator(ValidatorManager::ipValidator());
    ui->server1IpBLineEdit->setValidator(ValidatorManager::ipValidator());
    ui->server2IpALineEdit->setValidator(ValidatorManager::ipValidator());
    ui->server2IpBLineEdit->setValidator(ValidatorManager::ipValidator());
    ui->virtualIpALineEdit->setValidator(ValidatorManager::ipValidator());
    ui->virtualIpBLineEdit->setValidator(ValidatorManager::ipValidator());
}

QStringList ServerParaForm::configValidCheck()
{
    QStringList errorList;

    if (!ui->server1IpALineEdit->hasAcceptableInput() || ui->server1IpALineEdit->text() == DEFAULT_IP)
    {
        errorList << QString("服务器1的IP地址A不正确!");
    }

    if (ui->serverRunModeComboBox->currentIndex() == RunMode::Single)
    {
        if (ui->networkModeComboBox->currentIndex() == RunMode::Single)
        {
        }
        else
        {
            if (!ui->server1IpBLineEdit->hasAcceptableInput() || ui->server1IpBLineEdit->text() == DEFAULT_IP)
            {
                errorList << QString("服务器1的IP地址B不正确!");
            }
        }
    }
    else
    {
        if (!ui->server2IpALineEdit->hasAcceptableInput() || ui->server2IpALineEdit->text() == DEFAULT_IP)
        {
            errorList << QString("服务器2的IP地址B不正确!");
        }

        if (!ui->virtualIpALineEdit->hasAcceptableInput() || ui->virtualIpALineEdit->text() == DEFAULT_IP)
        {
            errorList << QString("服务器的虚拟IP地址A不正确!");
        }

        if (ui->networkModeComboBox->currentIndex() == RunMode::Single)
        {
        }
        else
        {
            if (!ui->server1IpBLineEdit->hasAcceptableInput() || ui->server1IpBLineEdit->text() == DEFAULT_IP)
            {
                errorList << QString("服务器1的IP地址B不正确!");
            }

            if (!ui->server2IpBLineEdit->hasAcceptableInput() || ui->server2IpBLineEdit->text() == DEFAULT_IP)
            {
                errorList << QString("服务器2的IP地址B不正确!");
            }

            if (!ui->virtualIpBLineEdit->hasAcceptableInput() || ui->virtualIpBLineEdit->text() == DEFAULT_IP)
            {
                errorList << QString("服务器的虚拟IP地址B不正确!");
            }
        }
    }

    return errorList;
}
