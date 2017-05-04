#include "formulaform.h"
#include "ui_formulaform.h"

void FormulaForm::initComboBoxGroup()
{
    QStringList variableList;
    variableList << "变量"
                 << "V01"
                 << "V02"
                 << "V03"
                 << "V04"
                 << "V05"
                 << "V06"
                 << "V07"
                 << "V08"
                 << "V09"
                 << "V10";
    QStringList arithmeticList;
    arithmeticList << "算术运算"
                   << "+"
                   << "-"
                   << "*"
                   << "/";

    QStringList compareList;
    compareList << "比较运算"
                << ">"
                << ">="
                << "<"
                << "<="
                << "=="
                << "!=";
    QStringList logicalList;
    logicalList << "逻辑运算"
                << "&&"
                << "||"
                << "!";

    QStringList basicFuncList;
    basicFuncList << "基本函数"
                  << "abs(x)"
                  << "max(x,y)"
                  << "min(x,y)"
                  << "sqr(x)"
                  << "cube(x)"
                  << "sqrt(x)"
                  << "cbrt(x)"
                  << "log(x)";

    QStringList trigonometricFuncList;
    trigonometricFuncList << "三角函数"
                          << "sin(x)"
                          << "cos(x)"
                          << "tan(x)"
                          << "cot(x)";
    QStringList conditionJudeList;
    conditionJudeList << "条件判断"
                      << "if(condition,expr1,expr2)";

    ui->variableComboBox->addItems(variableList);
    ui->arithmeticOpComboBox->addItems(arithmeticList);
    ui->compareOpComboBox->addItems(compareList);
    ui->logicalOpComboBox->addItems(logicalList);
    ui->basicFunctionComboBox->addItems(basicFuncList);
    ui->trigonometricFunctionComboBox->addItems(trigonometricFuncList);
    ui->conditionJudgeComboBox->addItems(conditionJudeList);

    connect(ui->variableComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &FormulaForm::comboBoxIndexChanged);

    connect(ui->arithmeticOpComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &FormulaForm::comboBoxIndexChanged);

    connect(ui->compareOpComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &FormulaForm::comboBoxIndexChanged);

    connect(ui->logicalOpComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &FormulaForm::comboBoxIndexChanged);

    connect(ui->basicFunctionComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &FormulaForm::comboBoxIndexChanged);

    connect(ui->trigonometricFunctionComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &FormulaForm::comboBoxIndexChanged);

    connect(ui->conditionJudgeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &FormulaForm::comboBoxIndexChanged);
}

void FormulaForm::comboBoxIndexChanged(int currentIndex)
{
    if (currentIndex == 0)
    {
        return;
    }

    QComboBox *comboBox = static_cast<QComboBox *>(sender());
    ui->formulaExprLineEdit->setText(ui->formulaExprLineEdit->text() + comboBox->currentText());
    comboBox->setCurrentIndex(0);
}
