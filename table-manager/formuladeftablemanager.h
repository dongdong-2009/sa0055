#ifndef FORMULADEFINITIONTABLEMANAGER_H
#define FORMULADEFINITIONTABLEMANAGER_H

#include <QObject>
class QSqlTableModel;

class FormulaDefTableManager
{
public:
    enum ColumnIndex
    {
        SeqNum,
        FormulaId,
        FormulaType,
        FormulaExpr,
        VariableNum
    };

    FormulaDefTableManager(QObject *parent = 0);

    QSqlTableModel *model()
    {
        return _model;
    }

    void addNewRecord(int formulaId, int formulaType);
    void deleteRecord(int row, int count);
    static FormulaDefTableManager *formulaDefinitionTableManager();

    void getFormulaInfo(int formulaId, int* formulaType, int* varNum = 0, QString* formulaExpr = 0, int *rowCount = 0);
    void setFormulaInfo(int formulaId, int *formulaVarNum, QString *formulaExpr = 0);

    void refreshData();
    void submitData();

    QList<int> getFormulaIdList();
    void deleteRecordById(int formulaId);
private:
    QSqlTableModel *_model;
    static FormulaDefTableManager *_formulaDefinitionTableManager;
};

#endif // FORMULADEFINITIONTABLEMANAGER_H
