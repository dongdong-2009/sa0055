#ifndef ALARMINFOTABLEMANAGER_H
#define ALARMINFOTABLEMANAGER_H

#include "selectpoint.h"
#include <QObject>
class QSqlTableModel;

class AlarmInfoTableManager
{
public:
    enum ColumnIndex
    {
        SeqNum,
        AlarmRuleId,
        DeviceId,
        PointType,
        PointDescId,
        AiUpperLimit,
        AiLowerLimit,
        AiUpperUpperLimit,
        AiLowerLowerLimit,
        MustConfirmFlag,
        UnconfirmedColor,
        ConfirmedColor,
        SoundFilePath,
        SoundPlayTime,
        PrintAlarmFlag,
        PushPicturePath,

        MaxColumnNum
    };

    static AlarmInfoTableManager *alarmInfoTableManager();
    static QString ColumnName[ColumnIndex::MaxColumnNum];

    AlarmInfoTableManager(QObject *parent = 0);
    QSqlTableModel *model()
    {
        return _model;
    }

    void addNewRecord(int alarmRuleId, QList<SelectPoint> selectPointList);
    void refreshData();
    void fetchData(int alarmRuleId);
    void deleteRecord(int alarmRuleId);
    void deleteRecord(int row, int count);
    void setColumnValue(QList<int> rowList, int column, QVariant value);
    void saveData();

private:
    static AlarmInfoTableManager *_alarmInfoTableManager;
    QSqlTableModel *_model;
};

#endif // ALARMINFOTABLEMANAGER_H
