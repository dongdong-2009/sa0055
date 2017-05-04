#ifndef AIDUMPTABLEMANAGER_H
#define AIDUMPTABLEMANAGER_H

#include <QObject>
#include <selectpoint.h>

class QSqlTableModel;

class AiDumpTableManager
{
protected:
    AiDumpTableManager(QObject *parent = 0);

public:
    static AiDumpTableManager *aiDumpTableManager();
    QSqlTableModel *model()
    {
        return _model;
    }

    void deleteRecord(QList<int> aiDescIdList);
    void addRecord(QList<SelectPoint> selectedList);
    void submitData();
    void fetchData(bool isAll = false);
    void setInterval(QList<int> rowList, int interval);

private:
    static AiDumpTableManager *_aiDumpTableManager;
    QSqlTableModel *_model;
    void _refreshData();
};

#endif // AIDUMPTABLEMANAGER_H
