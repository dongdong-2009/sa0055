#ifndef CALCVARTABLEMANAGER_H
#define CALCVARTABLEMANAGER_H
#include "datatype.h"
#include <QObject>
class QSqlTableModel;

class CalcVarTableManager
{
protected:
    CalcVarTableManager(QObject *parent = 0);

public:
    enum ColumnIndex
    {
        SeqNum,
        CalcVarId,
        FormulaId,
        FormulaType
    };

    QSqlTableModel *model() const
    {
        return _model;
    }

    static CalcVarTableManager *calcVarTableManager();
    void addRecord(int formulaId, int calcVarId, int formulaType);
    void deleteRecord(int calcVarId);

    void fetchData(int formulaId = -1);
    void fetchData(DataType type);
    void getCalVarInfo(int calcVarId, int *formulaId = 0, int *formulaType = 0);

    void refreshData();
    void submitData();
    QList<int> getCalVarIdList(int formulaId = -1);
private:
    QSqlTableModel *_model;
    static CalcVarTableManager *_calcVarTableManager;
};

#endif // CALCVARTABLEMANAGER_H
