#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calcvarform.h"
#include "datadumpform.h"
#include "datatraceform.h"
#include "deviceparaform.h"
#include "eventalarmform.h"
#include "formulaform.h"
#include "globalparaform.h"
#include "integralformulaform.h"
#include "plancurveform.h"
#include "serverparaform.h"

#include "treemodel.h"
#include "projecttreenode.h"

void MainWindow::initStackedWidgets()
{
    ui->stackedWidget->addWidget(stackedWidget[WidgetIndex::ServerPara] = new ServerParaForm(this));
    ui->stackedWidget->addWidget(stackedWidget[WidgetIndex::DevicePara] = new DeviceParaForm(this));
    ui->stackedWidget->addWidget(stackedWidget[WidgetIndex::PlanCurve] = new PlanCurveForm(this));
    ui->stackedWidget->addWidget(stackedWidget[WidgetIndex::IntegralFormula] = new IntegralFormulaForm(this));
    ui->stackedWidget->addWidget(stackedWidget[WidgetIndex::CommonFormula] = new FormulaForm(this));
    ui->stackedWidget->addWidget(stackedWidget[WidgetIndex::CalcVar] = new CalcVarForm(this));
    ui->stackedWidget->addWidget(stackedWidget[WidgetIndex::AlarmRule] = new EventAlarmForm(this));
    ui->stackedWidget->addWidget(stackedWidget[WidgetIndex::AIDump] = new DataDumpForm(this));
    ui->stackedWidget->addWidget(stackedWidget[WidgetIndex::AITrace] = new DataTraceForm(this));
    ui->stackedWidget->addWidget(stackedWidget[WidgetIndex::GlobalPara] = new GlobalParaForm(this));
}

void MainWindow::processLeftButtonClicked(const QModelIndex &index)
{
    TreeNode *item1 = static_cast<TreeNode *>(index.internalPointer());
    ProjectTreeNode *item = static_cast<ProjectTreeNode *>(item1);
    int widgetIndex = item->widgetIndex();

    if(widgetIndex == -1)
    {
        return;
    }

    int deviceId = item->deviceId();
    if(deviceId != -1)
    {
        static_cast<DeviceParaForm*>(stackedWidget[WidgetIndex::DevicePara])->setCurrentDeviceId(deviceId);
    }

    int formulaId = item->formulaId();
    if(formulaId != -1)
    {
        static_cast<FormulaForm*>(stackedWidget[WidgetIndex::CommonFormula])->setCurrentFormulaId(formulaId);
    }

    int calcVarId = item->calcVarId();
    if(calcVarId != -1)
    {
        static_cast<CalcVarForm*>(stackedWidget[WidgetIndex::CalcVar])->setCurrentCalcVarId(calcVarId);
    }

    int alarmRuleId = item->alarmRuleId();
    if(alarmRuleId != -1)
    {
        static_cast<EventAlarmForm*>(stackedWidget[WidgetIndex::AlarmRule])->setCurrentAlarmRuleId(alarmRuleId);
    }

    ui->stackedWidget->setCurrentIndex(widgetIndex);
}

void MainWindow::processRightButtonClicked(const QModelIndex &index)
{
    TreeNode *item1 = static_cast<TreeNode *>(index.internalPointer());
    ProjectTreeNode *item = static_cast<ProjectTreeNode *>(item1);

    int type = item->nodeType();
    if(type == -1)
    {
        return;
    }

    popupMenu.clear();

    switch (type) {
    case ProjectTreeNode::TreeNodeType::DeviceList:
        popupMenu.addAction(ui->addDeviceAction);
        break;
    case ProjectTreeNode::TreeNodeType::FormulaList:
        popupMenu.addAction(ui->addFormulaAction);
        break;
    case ProjectTreeNode::TreeNodeType::AlarmRuleList:
        popupMenu.addAction(ui->addAlarmRuleAction);
        break;
    case ProjectTreeNode::TreeNodeType::DeviceNode:
        popupMenu.addAction(ui->deleteDeviceAction);
        ui->deleteDeviceAction->setData(item->deviceId());
        break;
    case ProjectTreeNode::TreeNodeType::FormulaNode:
        popupMenu.addAction(ui->deleteFormulaAction);
        ui->deleteFormulaAction->setData(item->formulaId());
        break;
    case ProjectTreeNode::TreeNodeType::AlarmRuleNode:
        popupMenu.addAction(ui->deleteAlarmRuleAction);
        ui->deleteAlarmRuleAction->setData(item->alarmRuleId());
        break;
    default:
        return;
        break;
    }

    popupMenu.exec(QCursor::pos());
}

void MainWindow::returnToFirstNode()
{
    ui->treeView->setCurrentIndex(model->index(0, 0));
}

void MainWindow::initTreeView()
{
    model = new TreeModel(this);
    model->init2ProjectTree();
    ui->treeView->setModel(model);

    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::processLeftButtonClicked);

    ui->treeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, [=](const QPoint &pos){

        QModelIndex index = ui->treeView->indexAt(pos);
        if(!index.isValid())
        {
            return;
        }

        processRightButtonClicked(index);
    });
}
