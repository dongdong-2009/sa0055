#include "expression.h"
#include "constvalue.h"
#include "logutil.h"
#include "operand.h"
#include "operator.h"
#include "util.h"
#include <QtMath>

QMap<QChar, Operator> Expression::operators;

bool lengthGreaterThan(const QString &s1, const QString &s2)
{
    return s1.length() > s2.length();
}

void Expression::replaceOperator()
{
    static QMap<QString, QChar> operatorReplaceMap;
    static QList<QString> replaceKeyList;

    if (operatorReplaceMap.size() == 0)
    {
        foreach (Operator op, operators)
        {
            operatorReplaceMap.insert(op.desc, op.id);
        }

        replaceKeyList = operatorReplaceMap.keys();
        qSort(replaceKeyList.begin(), replaceKeyList.end(), lengthGreaterThan);
    }

    foreach (QString key, replaceKeyList)
    {
        expr.replace(key, operatorReplaceMap.value(key));
    }
}

void Expression::replaceVariableValue()
{
    for (int i = 0; i < varNum; i++)
    {
        QString varStr = Util::formulaVarName(i + 1);
        Operand data(i + 1, Operand::ID(), varStr);
        defaultVariableOperands.insert(data.id, data);
        expr.replace(varStr, QChar(data.id));
    }
}

void Expression::replaceConstantValue()
{
    QSet<QString> numSet;
    QRegExp re("(\\d+(?:\\.\\d+)?)");

    int pos = 0;
    while ((pos = re.indexIn(expr, pos)) != -1)
    {
        numSet.insert(re.cap(1));
        pos += re.matchedLength();
    }

    foreach (QString s, numSet)
    {
        Operand data(s.toDouble(), constOperandId++, s);
        constantOperands.insert(data.id, data);
        expr.replace(s, QChar(data.id));
    }
}

void Expression::processMinus()
{
    QList<int> rmList;
    QList<int> replaceList;

    //去掉空格
    expr = expr.replace(' ', "");

    //处理正负号
    for (int i = expr.size() - 1; i >= 0; i--)
    {
        if (i == 0 || (i > 0 && (expr.at(i - 1) == '(' || expr.at(i - 1) == ',')))
        {
            if (expr.at(i) == '+')
            {
                rmList << i;
            }
            else if (expr.at(i) == '-')
            {
                replaceList << i;
            }
        }
    }

    //替换负号
    foreach (int index, replaceList)
    {
        expr.replace(index, 1, '~');
    }

    //去掉正号
    foreach (int index, rmList)
    {
        expr.remove(index, 1);
    }

    // DEBUG << QString("去掉正号且替换负号后表达式为:%1").arg(expr);
}

Expression::Expression(const QString &rowExpr, int varNum)
    : expr(rowExpr), constOperandId(CONSTANT_ID_START), tempOperandId(TEMPORARY_ID_START), varNum(varNum)
{
    Q_ASSERT(!rowExpr.isEmpty() && varNum >= 0);

    if (operators.size() == 0)
    {
        initOperator();
    }

    processMinus();
    replaceVariableValue();
    replaceConstantValue();
    replaceOperator();

    foreach (Operand o, defaultVariableOperands)
    {
        operands.insert(o.id, o);
    }

    foreach (Operand o, constantOperands)
    {
        operands.insert(o.id, o);
    }

    DEBUG << QString("before convert to RNP: [%1]").arg(printableExpr(expr));
    _toRnpString();
    DEBUG << QString("after convert to RNP:[%1]").arg(printableExpr(rnpExpr));
}

Expression::~Expression()
{
}

bool Expression::isOK()
{
    if (error != Error::None)
    {
        return false;
    }

    operands.clear();
    foreach (Operand o, defaultVariableOperands)
    {
        operands.insert(o.id, o);
    }

    foreach (Operand o, constantOperands)
    {
        operands.insert(o.id, o);
    }

    Operand result = _eval();
    if (result.type != Operand::Boolean && result.type != Operand::Number)
    {
        error = Error::OperandError;
    }
    else
    {
        if (result.type == Operand::Boolean)
        {
            DEBUG << QString("result:%1").arg(result.boolValue);
        }
        else
        {
            DEBUG << QString("result:%1").arg(result.numberValue);
        }
    }

    return error == Error::None;
}

QString Expression::lastError()
{
    QString str;
    switch (error)
    {
    case None:
        str = "N/A";
        break;
    case UnsupportChar:
        str = "不支持的字符";
        break;
    case MismatchBracket:
        str = "括号不匹配";
        break;
    case CommaError:
        str = "逗号错误";
        break;
    case LackOperand:
        str = "缺少操作数";
        break;
    case UnsupportOperator:
        str = "不支持的运算符";
        break;
    case VariablNotAssignValue:
        str = "变量未赋值";
        break;
    case UnknownError:
        str = "未知错误";
        break;
    case OperandError:
        str = "操作数错误";
        break;
    }

    return str;
}

/*
Operand Expression::eval(const QList<Operand> &values)
{
    QMap<QChar, Operand> operands;
    foreach (Operand o, constantOperands)
    {
        operands.insert(o.id, o);
    }

    foreach (Operand o, values)
    {
        operands.insert(o.id, o);
    }

    return _eval(operands);
}
*/

Operand Expression::_eval()
{
    QStack<Operand> stack;
    Operand data1, data2, data3, result;

    for (int i = 0; error == Error::None && i < rnpExpr.length(); i++)
    {
        QChar id = rnpExpr.at(i);
        CharType type = _getCharType(id);

        if (type == CharType::OPERATOR)
        {
            Operator op = operators[id];

            if (stack.size() < op.operandNumber)
            {
                error = Error::LackOperand;
                break;
            }

            switch (op.operandNumber)
            {
            case 1:
                data1 = stack.pop();
                result = op.funcPtr1(result, data1);
                break;
            case 2:
                data2 = stack.pop();
                data1 = stack.pop();
                result = op.funcPtr2(result, data1, data2);
                break;
            case 3:
                data3 = stack.pop();
                data2 = stack.pop();
                data1 = stack.pop();
                result = op.funcPtr3(result, data1, data2, data3);
                break;
            default:
                error = Error::UnsupportOperator;
                break;
            }

            result.id = tempOperandId++;
            stack.push(result);

            operands.insert(result.id, result);
        }
        else
        {
            if (operands.contains(id))
            {
                stack.push(operands[id]);
            }
            else
            {
                error = Error::VariablNotAssignValue;
            }
        }
    }

    if (error == Error::None && stack.size() == 1)
    {
        return stack.pop();
    }
    else if (stack.size() != 1)
    {
        error = Error::UnknownError;
    }

    return Operand();
}

Expression::CharType Expression::_getCharType(QChar id)
{
    if (id >= OPERATOR_ID_START)
    {
        return CharType::OPERATOR;
    }
    else if (id >= VARIABLE_ID_START)
    {
        return CharType::OPERAND;
    }
    else
    {
        return CharType::UNKOWN;
    }
}

QString Expression::printableExpr(QString rawExpr)
{
    QString result = "";

    for (int i = 0; i < rawExpr.length(); i++)
    {
        QChar id = rawExpr.at(i);
        CharType type = _getCharType(id);

        if (type == CharType::OPERATOR)
        {
            result.append(operators[id].desc + " ");
        }
        else
        {
            result.append(operands[id].desc + " ");
        }
    }

    return result;
}

void Expression::_toRnpString()
{
    QStack<Operator> operatorStack;
    error = Error::None;

    for (int i = 0; error == Error::None && i < expr.length(); i++)
    {
        QChar id = expr.at(i);
        CharType type = _getCharType(id);
        QString opDesc;

        if (type == CharType::OPERATOR)
        {
            opDesc = operators[id].desc;
        }

        if (opDesc == "(")
        {
            operatorStack.push(operators[id]);
        }
        else if (opDesc == ")")
        {
            do
            {
                if (operatorStack.isEmpty())
                {
                    error = Error::MismatchBracket;
                    break;
                }

                Operator topOp = operatorStack.pop();
                if (topOp.desc == "(")
                {
                    break;
                }
                else
                {
                    rnpExpr.append(topOp.id);
                }

            } while (true);
        }
        else if (opDesc == ",")
        {
            do
            {
                if (operatorStack.isEmpty())
                {
                    error = Error::CommaError;
                    break;
                }

                Operator topOp = operatorStack.top();
                if (topOp.desc == "(")
                {
                    break;
                }
                else
                {
                    topOp = operatorStack.pop();
                    rnpExpr.append(topOp.id);
                }

            } while (true);
        }
        else if (type == CharType::OPERATOR)
        {
            Operator op = operators[id];
            do
            {
                if (operatorStack.isEmpty())
                {
                    operatorStack.push(op);
                    break;
                }

                if (op > operatorStack.top())
                {
                    operatorStack.push(op);
                    break;
                }
                else
                {
                    Operator topCh = operatorStack.pop();
                    rnpExpr.append(topCh.id);
                }
            } while (true);
        }
        else if (type == CharType::OPERAND)
        {
            rnpExpr.append(id);
        }
        else
        {
            error = Error::UnsupportChar;
            break;
        }
    }

    while (error == Error::None && !operatorStack.isEmpty())
    {
        Operator topOp = operatorStack.pop();
        rnpExpr.append(topOp.id);
    }
}
