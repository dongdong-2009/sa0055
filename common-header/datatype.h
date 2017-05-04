#ifndef DATATYPE_H
#define DATATYPE_H

#include <QString>

enum DataType
{
    DI,
    AI,
    DO,
    AO,
    CI,
    DataTypeNum
};

extern const QString DataTypeStr[];

enum RunMode
{
    Single,
    Standby
};

enum FormulaType
{
    Boolean,
    Numeric
};

#endif // DATATYPE_H
