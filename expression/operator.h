#ifndef OPERATOR_H
#define OPERATOR_H

#include "expression.h"
#include <QChar>
#include <QString>

class Operand;

class Operator
{
public:
    typedef Operand &(*FuncPtr1)(Operand &result, const Operand &a);
    typedef Operand &(*FuncPtr2)(Operand &result, const Operand &a, const Operand &b);
    typedef Operand &(*FuncPtr3)(Operand &result, const Operand &a, const Operand &b, const Operand &c);

    ushort id;
    QString desc;
    int priority;

    int operandNumber;
    FuncPtr1 funcPtr1;
    FuncPtr2 funcPtr2;
    FuncPtr3 funcPtr3;

    explicit Operator(ushort id = (ushort)-1) : id(id), priority(-1), operandNumber(-1)
    {
    }

    explicit Operator(ushort id, QString desc, int priority) : id(id), desc(desc), priority(priority), operandNumber(0)
    {
    }

    explicit Operator(ushort id, QString desc, int priority, FuncPtr1 ptr) : id(id), desc(desc), priority(priority), operandNumber(1), funcPtr1(ptr)
    {
    }

    explicit Operator(ushort id, QString desc, int priority, FuncPtr2 ptr) : id(id), desc(desc), priority(priority), operandNumber(2), funcPtr2(ptr)
    {
    }

    explicit Operator(ushort id, QString desc, int priority, FuncPtr3 ptr) : id(id), desc(desc), priority(priority), operandNumber(3), funcPtr3(ptr)
    {
    }

    bool operator>(const Operator &op2) const
    {
        return priority > op2.priority;
    }

    static ushort ID()
    {
        static unsigned short _ID = OPERATOR_ID_START;
        return _ID++;
    }
};

#endif // OPERATOR_H
