#ifndef ALARMCAUSETABLEMANAGER_H
#define ALARMCAUSETABLEMANAGER_H

#include "abstractmaptablemanager.h"

class AlarmCauseTableManager : public AbstractMapTableManager
{
public:
    enum ColumnIndex
    {
        Id,
        Name,
        SubCauseList,
        DataType
    };

    enum BindPointType
    {
        None = -1,
        DI,
        AI,
        DO,
        AO,
        CI,
        DEV
    };

    AlarmCauseTableManager(QObject *parent = 0);
    void getAlarmCauseInfo(int causeId, QString *subCauseStrList = 0, BindPointType *dataType = 0);

    static AlarmCauseTableManager *alarmCauseTableManager();

private:
    static AlarmCauseTableManager *_alarmCauseTableManager;
};

#endif // ALARMCAUSETABLEMANAGER_H
