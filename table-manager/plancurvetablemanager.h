#ifndef PLANCURVETABLEMANAGER_H
#define PLANCURVETABLEMANAGER_H

#include <QObject>
class QSqlTableModel;

class PlanCurveTableManager
{
protected:
    PlanCurveTableManager(QObject *parent = 0);

public:
    enum ColumnIndex
    {
        SeqNum,
        CurveId,
        CurveAddr,
        CurveType,
        CalcMethod,
        DeviceId,
        AiStartPoint,
        RealtimeAI,
        Unit
    };

    static QString ColumnName[];

    static PlanCurveTableManager *planCurveTableManager();
    QSqlTableModel *model()
    {
        return _model;
    }
    void addNewRecord(int curveId);
    void deleteRecord(int row, int count);
    void updateRecord(int row, int startAi, int rtAi);

    void refreshData();
    void submitData();
private:
    static PlanCurveTableManager *_planCurveTableManager;
    QSqlTableModel *_model;
};

#endif // PLANCURVETABLEMANAGER_H
