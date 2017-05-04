#ifndef OPERAND_H
#define OPERAND_H

#include "expression.h"
#include <QChar>
#include <QString>

#define Invalid_ID 65535

class Operand
{
public:
    enum Type
    {
        Boolean,
        Number,
        InvalidType,
        DivideZeroError,
        SqrtDomainError,
        LogDomainError,
        IfStatementError
    };

    Operand(ushort id = Invalid_ID) : id(id), type(Type::InvalidType), boolValue(false), numberValue(0.0)
    {
    }

    Operand(bool value, ushort id = Invalid_ID) : id(id), type(Type::Boolean), boolValue(value), numberValue(0.0)
    {
    }

    Operand(double value, ushort id = Invalid_ID, QString desc = "") : id(id), type(Type::Number), boolValue(false), numberValue(value), desc(desc)
    {
    }

    ushort id;
    Type type;
    bool boolValue;
    double numberValue;
    QString desc;

    static ushort ID()
    {
        static unsigned short _ID = VARIABLE_ID_START;
        return _ID++;
    }
};

QDebug &operator<<(QDebug &out, const Operand &s);

#endif // OPERAND_H
