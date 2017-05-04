#include "formulaform.h"
#include "constvalue.h"
#include "enumtablemanager.h"
#include "formuladeftablemanager.h"
#include "logutil.h"
#include "maptablemanager.h"
#include "maptablemanager.h"
#include "selectpointdialog.h"
#include "tempcalcvartablemodel.h"
#include "ui_formulaform.h"
#include "util.h"
#include <QDataWidgetMapper>
#include <QSqlError>

FormulaForm::FormulaForm(QWidget *parent) : QWidget(parent), ui(new Ui::FormulaForm)
{
    ui->setupUi(this);

    ui->formulaExprLineEdit->setMaxLength(FormulaExprMaxLength);

    initComboBoxGroup();

    initCalcVarTable();

    connect(ui->addCalcVariablePushButton, &QPushButton::clicked, this, &FormulaForm::addNewCalcVar);
    connect(ui->deleteCalcVariablePushButton, &QPushButton::clicked, this, &FormulaForm::deleteCalcVar);
    connect(ui->formulaValidatePushButton, &QPushButton::clicked, this, &FormulaForm::validateFormula);

    connect(ui->formulaExprLineEdit, &QLineEdit::textChanged, [=]() { ui->formulaValidatePushButton->setEnabled(true); });
}

FormulaForm::~FormulaForm() { delete ui; }

void FormulaForm::hideEvent(QHideEvent * /*event*/)
{
    FormulaDefTableManager::formulaDefinitionTableManager()->submitData();

    DEBUG << configValidCheck();
}

void FormulaForm::setCurrentFormulaId(int formulaId)
{
    this->formulaId = formulaId;

    int currentIndex;
    QString formulaExpr;
    FormulaDefTableManager::formulaDefinitionTableManager()->getFormulaInfo(formulaId, &formulaType, &formulaVarNum, &formulaExpr, &currentIndex);

    updateCalcVarTable(formulaId);
    hideUnusedVar(formulaVarNum);

    ui->formulaNameLineEdit->setText(MapTableManager::mapTableManager(MapTableManager::TableIndex::FORMULA)->nameFromId(formulaId));
    ui->formulaTypeLineEdit->setText(EnumTableManager::formulaTypeEnumManager()->nameFromId(formulaType));
    ui->formulaExprLineEdit->setText(formulaExpr);
}

QStringList FormulaForm::configValidCheck()
{
    QStringList errorList;

    if (!validateFormula())
    {
        errorList << QString("公式不正确!");
    }

    QList<QList<CalcVarBindPoint>> bindPointListList = tempCalcVarTableModel->getBindPointListList();
    QList<int> calcVarIdList = tempCalcVarTableModel->getCalcVarIdList();

    for (int i = 0; i < bindPointListList.size(); i++)
    {
        int calcVarId = calcVarIdList.at(i);

        for (int j = 0; j < bindPointListList.at(i).size(); j++)
        {
            CalcVarBindPoint point = bindPointListList.at(i).at(j);

            int varSeqNum = point.getVarSeqNum();
            if (varSeqNum >= formulaVarNum)
            {
                break;
            }

            if(calcVarId == point.getPointDescId())
            {
                errorList << QString("第%1个计算量中变量V%2绑定的数据点为该计算量本身!").arg(i + 1).arg(varSeqNum + 1, 2, 10, QChar('0'));
            }

            QString desc = SelectPoint::getDescInfo(point.getDeviceId(), point.getPointType(), point.getPointDescId());
            if (desc.isEmpty())
            {
                errorList << QString("第%1个计算量中变量V%2未绑定数据点!").arg(i + 1).arg(varSeqNum + 1, 2, 10, QChar('0'));
            }
        }
    }

    return errorList;
}
