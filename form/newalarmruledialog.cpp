#include "newalarmruledialog.h"
#include "alarmcausetablemanager.h"
#include "constvalue.h"
#include "logutil.h"
#include "maptablemanager.h"
#include "ui_newalarmruledialog.h"
#include <QMessageBox>

NewAlarmRuleDialog::NewAlarmRuleDialog(QWidget *parent) : QDialog(parent), ui(new Ui::NewAlarmRuleDialog)
{
    ui->setupUi(this);

    ui->alarmCauseComboBox->setModel(AlarmCauseTableManager::alarmCauseTableManager()->model());
    ui->alarmCauseComboBox->setModelColumn(AlarmCauseTableManager::alarmCauseTableManager()->nameColumnIndex());
    ui->alarmCauseComboBox->setCurrentIndex(0);

    connect(ui->okPushButton, &QPushButton::clicked, this, &NewAlarmRuleDialog::okButtonClicked);
    connect(ui->cancelPushButton, &QPushButton::clicked, [=]() { reject(); });

    int num = MapTableManager::mapTableManager(MapTableManager::TableIndex::ALARMRULE)->currentMaxId() - Desc_ID_Start + 1;
    QString defaultAlarmRuleName = QString("告警规则#%1").arg(num);
    ui->AlarmRuleLineEdit->setText(defaultAlarmRuleName);
}

NewAlarmRuleDialog::~NewAlarmRuleDialog()
{
    delete ui;
}

void NewAlarmRuleDialog::okButtonClicked()
{
    QString ruleName = ui->AlarmRuleLineEdit->text();
    if (ruleName.isEmpty())
    {
        QMessageBox::warning(this, "提示", "公式名称为空!");
        return;
    }

    int id = MapTableManager::mapTableManager(MapTableManager::TableIndex::ALARMRULE)->addDesc(ruleName);
    if (id == MapTableManager::Error::AlreadyExist)
    {
        QMessageBox::warning(this, "提示", "公式名称已经被使用!");
        return;
    }

    bool ok;
    int causeId = AlarmCauseTableManager::alarmCauseTableManager()
                      ->model()
                      ->index(ui->alarmCauseComboBox->currentIndex(), AlarmCauseTableManager::alarmCauseTableManager()->idColumnIndex())
                      .data()
                      .toInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }

    emit createAlarmRuleOk(id, causeId);
    accept();
}
