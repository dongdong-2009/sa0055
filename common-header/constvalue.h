#ifndef CONSTVALUE_H
#define CONSTVALUE_H

#include <QString>

#define DevicePointDescFormat QString("[%1]:%2")

const int FormulaVarMaxNum = 99;

const int FormulaExprMaxLength = 128;
const int DescriptionMaxLength = 64;

const int Desc_ID_Start = 10000;
const int CalcVarPseudoDeviceId = Desc_ID_Start - 1000;
const QString CalcVarStr = "计算量";

const QString DEFAULT_IP = "0.0.0.0";

const int MaxDecimals = 10;
const double MaxDoubleValue = 2147483647;
const double MinDoubleValue = -2147483648;

const int MaxIntegerValue = 2147483647;
const int MinIntegerValue = -2147483648;
#endif // CONSTVALUE_H
