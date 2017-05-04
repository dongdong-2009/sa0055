#include "formulaform.h"
#include "expression.h"
#include "formuladeftablemanager.h"
#include "logutil.h"
#include "ui_formulaform.h"
#include <QMessageBox>

int FormulaForm::checkFormulaVar(QString expr)
{
    QRegExp regexp("V(\\d{2,})");

    QSet<int> varSeqNumset;
    int pos = 0;
    while ((pos = regexp.indexIn(expr, pos)) != -1)
    {
        pos += regexp.matchedLength();
        varSeqNumset << regexp.cap(1).toInt();
    }

    if (varSeqNumset.size() < 1)
    {
        return 0;
    }

    QList<int> varSeqNumlist = varSeqNumset.toList();
    qSort(varSeqNumlist);

    if (varSeqNumlist.first() == 1 && varSeqNumlist.last() == varSeqNumlist.size())
    {
        return varSeqNumlist.size();
    }
    else
    {
        return -1;
    }
}

bool FormulaForm::validateFormula(bool silent)
{
    QString formulaStr = ui->formulaExprLineEdit->text().simplified();
    ui->formulaExprLineEdit->setText(formulaStr);

    bool result = false;
    if (formulaStr.isEmpty())
    {
        if (!silent)
        {
            QMessageBox::warning(this, "提示", "公式不能为空！");
        }
    }
    else
    {
        int varNum = checkFormulaVar(formulaStr);
        if (varNum <= 0)
        {
            if (!silent)
            {
                QMessageBox::warning(this, "提示", "变量使用错误！\n"
                                                   "1.变量必须以字母V开始，后跟2位序号。例如：V01,V02,...V99\n"
                                                   "2.变量序号必须连续，不能跳跃。");
            }
        }
        else
        {
            Expression expr(formulaStr, varNum);
            if (!expr.isOK())
            {
                if (!silent)
                {
                    QMessageBox::warning(this, "提示", QString("公式错误,请重新输入!\n"));
                }
                DEBUG << QString("formula [%1] error: %2").arg(formulaStr).arg(expr.lastError());
            }
            else
            {
                result = true;
                int formulaVarNum = varNum;
                hideUnusedVar(formulaVarNum);
                FormulaDefTableManager::formulaDefinitionTableManager()->setFormulaInfo(formulaId, &formulaVarNum, &formulaStr);
            }
        }
    }

    setFormulaValid(result);
    return result;
}
