#ifndef EXPRESSON_H
#define EXPRESSON_H
#include <QMap>
#include <QSet>
#include <QStack>
#include <QString>
#include <QVariant>

class Operand;
class Operator;

#define VARIABLE_ID_START 5000
#define CONSTANT_ID_START 10000
#define TEMPORARY_ID_START 15000
#define OPERATOR_ID_START 30000

class Expression
{
public:
    enum CharType
    {
        OPERATOR,
        OPERAND,
        UNKOWN
    };

    enum Error
    {
        None,
        UnsupportChar,
        MismatchBracket,
        CommaError,
        LackOperand,
        UnsupportOperator,
        VariablNotAssignValue,
        UnknownError,
        OperandError
    };

    Expression(const QString &expr, int varNum);
    ~Expression();
    bool isOK();
    QString lastError();
    Operand eval(const QList<Operand> &values);

private:
    QString expr;
    QString rnpExpr;
    Error error;

    ushort constOperandId;
    ushort tempOperandId;
    QMap<QChar, Operand> constantOperands;
    QMap<QChar, Operand> defaultVariableOperands;
    QMap<QChar, Operand> operands;

    void _toRnpString();
    CharType _getCharType(QChar ch);

    void processMinus();
    void replaceConstantValue();
    void replaceOperator();
    void replaceVariableValue();

    static QMap<QChar, Operator> operators;
    static void initOperator();

    Operand _eval();

    int varNum;
    QString printableExpr(QString rawExpr);
};

#endif // EXPRESSON_H
