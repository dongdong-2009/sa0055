#ifndef CALCVARBINDTABLEMANAGER_H
#define CALCVARBINDTABLEMANAGER_H
#include "datatype.h"
#include <QObject>
class QSqlTableModel;

class CalcVarBindTableManager
{
protected:
    CalcVarBindTableManager(QObject *parent = 0);

public:
    enum ColumnIndex
    {
        CalcVarId,
        VarSeqNum,
        DeviceId,
        PointType,
        PointDescId
    };

    QSqlTableModel *model() const
    {
        return _model;
    }
    void addNewRecord(int calcVarId, int varSeqNum, int deviceId, int pointType, int pointDescId);
    void deleteRecord(int calcVarid);

    static CalcVarBindTableManager *calcVarBindTableManager();

    void updateRecord(int calcVarId, int varSeqNum, int deviceId, int pointType, int pointDescId);
    void fetchData(int calcVarId);

    void refreshData();
    void submitData();
private:
    QSqlTableModel *_model;
    static CalcVarBindTableManager *_calcVarBindTableManager;
};

#endif // CALCVARBINDTABLEMANAGER_H
