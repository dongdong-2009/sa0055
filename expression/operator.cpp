#include "expression.h"
#include "logutil.h"
#include <QtMath>

#include "operand.h"
#include "operator.h"

#define epsion 1e-6

Operand &ADD(Operand &result, const Operand &n1, const Operand &n2)
{
    if (n1.type == Operand::Number && n2.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = n1.numberValue + n2.numberValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &SUB(Operand &result, const Operand &n1, const Operand &n2)
{
    if (n1.type == Operand::Number && n2.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = n1.numberValue - n2.numberValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &MINUS(Operand &result, const Operand &n1)
{
    if (n1.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = -n1.numberValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &MUL(Operand &result, const Operand &n1, const Operand &n2)
{
    if (n1.type == Operand::Number && n2.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = n1.numberValue * n2.numberValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &DIV(Operand &result, const Operand &n1, const Operand &n2)
{
    if (n1.type == Operand::Number && n2.type == Operand::Number)
    {
        if (qAbs(n2.numberValue) < epsion)
        {
            result.type = Operand::DivideZeroError;
        }
        else
        {
            result.type = Operand::Number;
            result.numberValue = n1.numberValue / n2.numberValue;
        }
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &GT(Operand &result, const Operand &n1, const Operand &n2)
{
    if (n1.type == Operand::Number && n2.type == Operand::Number)
    {
        result.type = Operand::Boolean;
        result.boolValue = n1.numberValue > n2.numberValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &GE(Operand &result, const Operand &n1, const Operand &n2)
{
    if (n1.type == Operand::Number && n2.type == Operand::Number)
    {
        result.type = Operand::Boolean;
        result.boolValue = n1.numberValue >= n2.numberValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &LT(Operand &result, const Operand &n1, const Operand &n2)
{
    if (n1.type == Operand::Number && n2.type == Operand::Number)
    {
        result.type = Operand::Boolean;
        result.boolValue = n1.numberValue < n2.numberValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &LE(Operand &result, const Operand &n1, const Operand &n2)
{
    if (n1.type == Operand::Number && n2.type == Operand::Number)
    {
        result.type = Operand::Boolean;
        result.boolValue = n1.numberValue <= n2.numberValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &EQ(Operand &result, const Operand &n1, const Operand &n2)
{
    if (n1.type == Operand::Number && n2.type == Operand::Number)
    {
        result.type = Operand::Boolean;
        result.boolValue = qAbs(n1.numberValue - n2.numberValue) < epsion;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &NE(Operand &result, const Operand &n1, const Operand &n2)
{
    EQ(result, n1, n2);
    if (result.type == Operand::Boolean)
    {
        result.boolValue = !result.boolValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }
    return result;
}

Operand &AND(Operand &result, const Operand &b1, const Operand &b2)
{
    if (b1.type == Operand::Boolean && b2.type == Operand::Boolean)
    {
        result.type = Operand::Boolean;
        result.boolValue = b1.boolValue && b2.boolValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &OR(Operand &result, const Operand &b1, const Operand &b2)
{
    if (b1.type == Operand::Boolean && b2.type == Operand::Boolean)
    {
        result.type = Operand::Boolean;
        result.boolValue = b1.boolValue || b2.boolValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &NOT(Operand &result, const Operand &b1)
{
    if (b1.type == Operand::Boolean)
    {
        result.type = Operand::Boolean;
        result.boolValue = !b1.boolValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &ABS(Operand &result, const Operand &n1)
{
    if (n1.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = qAbs(n1.numberValue);
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &MAX(Operand &result, const Operand &n1, const Operand &n2)
{
    if (n1.type == Operand::Number && n2.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = n1.numberValue >= n2.numberValue ? n1.numberValue : n2.numberValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &MIN(Operand &result, const Operand &n1, const Operand &n2)
{
    if (n1.type == Operand::Number && n2.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = n1.numberValue <= n2.numberValue ? n1.numberValue : n2.numberValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &SQR(Operand &result, const Operand &n1)
{
    if (n1.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = n1.numberValue * n1.numberValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &CUBE(Operand &result, const Operand &n1)
{
    if (n1.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = n1.numberValue * n1.numberValue * n1.numberValue;
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &SQRT(Operand &result, const Operand &n1)
{
    if (n1.type == Operand::Number)
    {
        if (n1.numberValue >= 0)
        {
            result.type = Operand::Number;
            result.numberValue = sqrt(n1.numberValue);
        }
        else
        {
            result.type = Operand::SqrtDomainError;
        }
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &CBRT(Operand &result, const Operand &n1)
{
    if (n1.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = cbrt(n1.numberValue);
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &LOG(Operand &result, const Operand &n1)
{
    if (n1.type == Operand::Number)
    {
        if (n1.numberValue > 0)
        {
            result.type = Operand::Number;
            result.numberValue = log(n1.numberValue);
        }
        else
        {
            result.type = Operand::LogDomainError;
        }
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &IF(Operand &result, const Operand &n1, const Operand &n2, const Operand &n3)
{
    if (n1.type == Operand::Boolean)
    {
        if (n2.type == Operand::Number && n3.type == Operand::Number)
        {
            result.type = Operand::Number;
            result.numberValue = n1.boolValue ? n2.numberValue : n3.numberValue;
        }
        else if (n2.type == Operand::Boolean && n3.type == Operand::Boolean)
        {
            result.type = Operand::Boolean;
            result.boolValue = n1.boolValue ? n2.boolValue : n3.boolValue;
        }
        else
        {
            result.type = Operand::IfStatementError;
        }
    }
    else
    {
        result.type = Operand::IfStatementError;
    }

    return result;
}

Operand &SIN(Operand &result, const Operand &n1)
{
    if (n1.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = sin(n1.numberValue);
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &COS(Operand &result, const Operand &n1)
{
    if (n1.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = cos(n1.numberValue);
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &TAN(Operand &result, const Operand &n1)
{
    if (n1.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = tan(n1.numberValue);
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

Operand &COT(Operand &result, const Operand &n1)
{
    if (n1.type == Operand::Number)
    {
        result.type = Operand::Number;
        result.numberValue = 1 / tan(n1.numberValue);
    }
    else
    {
        result.type = Operand::InvalidType;
    }

    return result;
}

void insertOperatorMap(QMap<QChar, Operator> &operators, const Operator &op)
{
    operators.insert(op.id, op);
}

void Expression::initOperator()
{
    insertOperatorMap(operators, Operator(Operator::ID(), "(", 0));
    insertOperatorMap(operators, Operator(Operator::ID(), ",", 1));

    insertOperatorMap(operators, Operator(Operator::ID(), "||", 800, &OR));
    insertOperatorMap(operators, Operator(Operator::ID(), "&&", 801, &AND));
    insertOperatorMap(operators, Operator(Operator::ID(), "!", 802, &NOT));

    insertOperatorMap(operators, Operator(Operator::ID(), ">", 900, &GT));
    insertOperatorMap(operators, Operator(Operator::ID(), ">=", 900, &GE));
    insertOperatorMap(operators, Operator(Operator::ID(), "<", 900, &LT));
    insertOperatorMap(operators, Operator(Operator::ID(), "<=", 900, &LE));
    insertOperatorMap(operators, Operator(Operator::ID(), "==", 900, &EQ));
    insertOperatorMap(operators, Operator(Operator::ID(), "!=", 900, &NE));

    insertOperatorMap(operators, Operator(Operator::ID(), "+", 1000, &ADD));
    insertOperatorMap(operators, Operator(Operator::ID(), "-", 1000, &SUB));
    insertOperatorMap(operators, Operator(Operator::ID(), "*", 1001, &MUL));
    insertOperatorMap(operators, Operator(Operator::ID(), "/", 1001, &DIV));

    insertOperatorMap(operators, Operator(Operator::ID(), "~", 1100, &MINUS));

    insertOperatorMap(operators, Operator(Operator::ID(), "if", 1700, &IF));
    insertOperatorMap(operators, Operator(Operator::ID(), "abs", 1700, &ABS));
    insertOperatorMap(operators, Operator(Operator::ID(), "max", 1700, &MAX));
    insertOperatorMap(operators, Operator(Operator::ID(), "min", 1700, &MIN));
    insertOperatorMap(operators, Operator(Operator::ID(), "sqr", 1700, &SQR));
    insertOperatorMap(operators, Operator(Operator::ID(), "cube", 1700, &CUBE));
    insertOperatorMap(operators, Operator(Operator::ID(), "sqrt", 1700, &SQRT));
    insertOperatorMap(operators, Operator(Operator::ID(), "cbrt", 1700, &CBRT));
    insertOperatorMap(operators, Operator(Operator::ID(), "log", 1700, &LOG));
    insertOperatorMap(operators, Operator(Operator::ID(), "sin", 1700, &SIN));
    insertOperatorMap(operators, Operator(Operator::ID(), "cos", 1700, &COS));
    insertOperatorMap(operators, Operator(Operator::ID(), "tan", 1700, &TAN));
    insertOperatorMap(operators, Operator(Operator::ID(), "cot", 1700, &COT));

    insertOperatorMap(operators, Operator(Operator::ID(), ")", 5000));
}

QDebug &operator<<(QDebug &out, const Operator &s)
{
    out << QString("(id=%1,name=%2,priority=%3,opNumber=%4)").arg(s.id).arg(s.desc).arg(s.priority).arg(s.operandNumber);
    return out;
}
