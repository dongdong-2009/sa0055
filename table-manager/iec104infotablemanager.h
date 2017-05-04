#ifndef IEC104INFOTABLEMANAGER_H
#define IEC104INFOTABLEMANAGER_H

#include "datatype.h"
#include <QObject>
#include <infotableimportexport.h>

class QSqlTableModel;

class Iec104InfoTableManager
{
public:
    enum ColumnIndex
    {
        /*for DI/AI/DO/AO/CI*/
        SeqNum = 0,
        DeviceId,
        DescrId,
        PointNum,

        /*for DO*/
        DO_Qualifier = 4,
        DO_Type,
        DO_StepType,

        /*for AI*/
        AI_Multiple = 4,
        AI_Offset,
        AI_Dump_Flag,
        AI_Dump_Interval,
        AI_Trace_Flag,
        AI_Trace_Deadband,
        AI_Source
    };

    QSqlTableModel *model();
    void fetchData(int deviceId);
    void addRecord(int deviceId, int descId);
    void addRecordBatch(int deviceId, QList<int> descIdList, QList<InfoTableImportExport::PointInfo> pointList);
    void deleteRecord(int deviceId, int row, int count);
    void deleteRecord(int deviceId);
    void submitData();

    static Iec104InfoTableManager *infoTableManager(DataType type);
protected:
    Iec104InfoTableManager(DataType type, QObject *parent = 0);
    ~Iec104InfoTableManager();

private:
    QSqlTableModel *_model;
    DataType type;
    void _refreshData();

    static Iec104InfoTableManager *_infoTableManager[DataType::DataTypeNum];
};

#endif // IEC104INFOTABLEMANAGER_H
