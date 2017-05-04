#include "mainwindow.h"
#include "aboutdialog.h"
#include "alarminfotablemanager.h"
#include "alarmruletablemanager.h"
#include "calcvarbindtablemanager.h"
#include "calcvarform.h"
#include "calcvartablemanager.h"
#include "dbtools.h"
#include "deviceparaform.h"
#include "deviceparatablemanager.h"
#include "eventalarmform.h"
#include "formuladeftablemanager.h"
#include "formulaform.h"
#include "iec104paratablemanager.h"
#include "maptablemanager.h"
#include "newalarmruledialog.h"
#include "newdevicedialog.h"
#include "newformuladialog.h"
#include "newprojectdialog.h"
#include "projecttreenode.h"
#include "ui_mainwindow.h"
#include "util.h"
#include <QMessageBox>

void MainWindow::initActions()
{
    connect(ui->newProjectAction, &QAction::triggered, [=]() {
        NewProjectDialog form;
        form.exec();
    });

    connect(ui->saveProjectAction, &QAction::triggered, [=]() {
        MapTableManager::CopyMap2Model();
        DbTools::dumpMemoryToFile();
    });

    connect(ui->aboutAction, &QAction::triggered, [=]() { AboutDialog(this).exec(); });

    connect(ui->addDeviceAction, &QAction::triggered, [=]() {
        NewDeviceDialog dialog(this);
        connect(&dialog, &NewDeviceDialog::createDeviceOk, [=](int deviceId) {
            ProjectTreeNode::addNewDeviceNode(deviceId);
            DeviceParaTableManager::deviceParaTableManager()->addNewDevice(deviceId);
            Iec104ParaTableManager::iec104ParaTableManager()->addNewIec104ParaRecord(deviceId);
        });
        dialog.exec();
    });

    connect(ui->deleteDeviceAction, &QAction::triggered, [=]() {
        bool ok;
        int deviceId = ui->deleteDeviceAction->data().toInt(&ok);
        Q_ASSERT(ok);

        QString deviceName = MapTableManager::mapTableManager(MapTableManager::TableIndex::DEV)->nameFromId(deviceId);
        if (QMessageBox::question(this, "提示", QString("将删除装置:%1。是否继续?").arg(deviceName)) == QMessageBox::No)
        {
            return;
        }

        DeviceParaTableManager::deviceParaTableManager()->deleteDevice(deviceId);
        Iec104ParaTableManager::iec104ParaTableManager()->deleteIec104ParaRecord(deviceId);
        for (int i = 0; i < DataType::DataTypeNum; i++)
        {
            Iec104InfoTableManager::infoTableManager(static_cast<DataType>(i))->deleteRecord(deviceId);
        }

        ProjectTreeNode::deleteDeviceNode(deviceId);
        returnToFirstNode();
    });

    connect(ui->addFormulaAction, &QAction::triggered, [=]() {
        NewFormulaDialog dialog(this);
        connect(&dialog, &NewFormulaDialog::createFormulaOk, [=](int formulaId, int formulaType) {
            ProjectTreeNode::addNewFormulaNode(formulaId);
            FormulaDefTableManager::formulaDefinitionTableManager()->addNewRecord(formulaId, formulaType);
        });
        dialog.exec();
    });

    connect(ui->deleteFormulaAction, &QAction::triggered, [=]() {
        bool ok;
        int formulaId = ui->deleteFormulaAction->data().toInt(&ok);
        Q_ASSERT(ok);

        QString formulaName = MapTableManager::mapTableManager(MapTableManager::TableIndex::FORMULA)->nameFromId(formulaId);
        if (QMessageBox::question(this, "提示", QString("将删除公式:%1。是否继续?").arg(formulaName)) == QMessageBox::No)
        {
            return;
        }

        QList<int> calcVarIdList = CalcVarTableManager::calcVarTableManager()->getCalVarIdList(formulaId);
        foreach (int calcVarId, calcVarIdList)
        {
            CalcVarBindTableManager::calcVarBindTableManager()->deleteRecord(calcVarId);
            CalcVarTableManager::calcVarTableManager()->deleteRecord(calcVarId);
            ProjectTreeNode::deleteCalcVarNode(calcVarId);
        }

        FormulaDefTableManager::formulaDefinitionTableManager()->deleteRecordById(formulaId);
        ProjectTreeNode::deleteFormulaNode(formulaId);

        returnToFirstNode();
    });

    connect(ui->addAlarmRuleAction, &QAction::triggered, [=]() {
        NewAlarmRuleDialog dialog(this);
        connect(&dialog, &NewAlarmRuleDialog::createAlarmRuleOk, [=](int alarmRuleId, int alarmCauseId) {
            AlarmRuleTableManager::alarmRuleTableManager()->addNewRecord(alarmRuleId, alarmCauseId);
            ProjectTreeNode::addNewAlarmRuleNode(alarmRuleId);
        });
        dialog.exec();
    });

    connect(ui->deleteAlarmRuleAction, &QAction::triggered, [=]() {
        bool ok;
        int alarmRuleId = ui->deleteAlarmRuleAction->data().toInt(&ok);
        Q_ASSERT(ok);

        QString alarmRuleName = MapTableManager::mapTableManager(MapTableManager::TableIndex::ALARMRULE)->nameFromId(alarmRuleId);
        if (QMessageBox::question(this, "提示", QString("将删除告警规则:%1。是否继续?").arg(alarmRuleName)) == QMessageBox::No)
        {
            return;
        }

        AlarmInfoTableManager::alarmInfoTableManager()->deleteRecord(alarmRuleId);
        AlarmRuleTableManager::alarmRuleTableManager()->deleteRecord(alarmRuleId);
        ProjectTreeNode::deleteAlarmRuleNode(alarmRuleId);

        returnToFirstNode();
    });
}
