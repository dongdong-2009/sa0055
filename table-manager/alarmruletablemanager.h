#ifndef ALARMRULETABLEMANAGER_H
#define ALARMRULETABLEMANAGER_H

#include <QObject>
class QSqlTableModel;

class AlarmRuleTableManager
{
public:
    enum ColumnIndex
    {
        SeqNum,
        AlarmRuleId,
        AlarmCauseId,
        AlarmSubCauseValue,
        AlarmType,
        CosDisplay
    };

    enum AlarmCause
    {
        COS,
        SOE,
        AIOutOfLimit,
        DIForceValue,
        AIForceValue,
        DIHangTag,
        AIHangTag,
        DeviceHangTag,
        DOOperation,
        AOOperation,
        SystemLog
    };

    enum AlarmType
    {
        Accident,
        MalFunction,
        StatusChange,
        OutOfLimit,
        Inform
    };

    static AlarmRuleTableManager *alarmRuleTableManager();
    AlarmRuleTableManager(QObject *parent = 0);
    void addNewRecord(int alarmRuleId, int alarmCauseId);
    void deleteRecord(int alarmRuleId);
    QSqlTableModel *model()
    {
        return _model;
    }

    void setAlarmSubCause(int alarmRuleId, int alarmSubCause);

    void refreshData();
    void submitData();
    void getAlarmRuleInfo(int alarmRuleId, int *alarmCauseId, int *alarmSubCauseValue, int *row);

    QList<int> getAlarmRuleIdList();
private:
    static AlarmRuleTableManager *_alarmRuleTableManager;
    QSqlTableModel *_model;
};

#endif // ALARMRULETABLEMANAGER_H
