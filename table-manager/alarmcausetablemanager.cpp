#include "alarmcausetablemanager.h"

AlarmCauseTableManager *AlarmCauseTableManager::_alarmCauseTableManager = 0;

AlarmCauseTableManager *AlarmCauseTableManager::alarmCauseTableManager()
{
    if (!_alarmCauseTableManager)
    {
        _alarmCauseTableManager = new AlarmCauseTableManager();
    }

    return _alarmCauseTableManager;
}

AlarmCauseTableManager::AlarmCauseTableManager(QObject *parent) : AbstractMapTableManager("sa0055_alarm_cause_enum", "id", "name", parent)
{
}

void AlarmCauseTableManager::getAlarmCauseInfo(int causeId, QString *subCauseStrList, BindPointType *dataType)
{
    bool ok;
    for (int i = 0; i < model()->rowCount(); i++)
    {
        int id = model()->index(i, ColumnIndex::Id).data().toInt(&ok);
        Q_ASSERT(ok);

        if (id == causeId)
        {
            if (subCauseStrList)
            {
                *subCauseStrList = model()->index(i, ColumnIndex::SubCauseList).data().toString();
            }

            if (dataType)
            {
                *dataType = BindPointType(model()->index(i, ColumnIndex::DataType).data().toInt(&ok));
                Q_ASSERT(ok);
            }

            return;
        }
    }

    Q_UNREACHABLE();
}
