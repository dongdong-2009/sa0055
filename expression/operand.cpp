#include "operand.h"
#include "logutil.h"

QDebug &operator<<(QDebug &out, const Operand &s)
{
    if (s.type == Operand::Type::Boolean)
    {
        out << QString("(id=%1,value=%2)").arg(s.id).arg(s.boolValue ? "true" : "false");
    }
    else if (s.type == Operand::Type::Number)
    {
        out << QString("(id=%1,value=%2)").arg(s.id).arg(s.numberValue);
    }
    else
    {
        out << QString("(id=%1,value=****)").arg(s.id);
    }

    return out;
}
