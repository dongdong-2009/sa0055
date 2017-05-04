#ifndef AITRACETABLEMANAGER_H
#define AITRACETABLEMANAGER_H
#include <QObject>

class QSqlTableModel;
class SelectPoint;

class AiTraceTableManager
{
protected:
    AiTraceTableManager(QObject *parent = 0);

public:
    static AiTraceTableManager *aiTraceTableManager();
    QSqlTableModel *model()
    {
        return _model;
    }
    void submitData();
    void fetchData(bool isAll = false);

    void addRecord(QList<SelectPoint> selectPointList);
    void deleteRecord(QList<int> aiDescIdList);
    void setDeadband(QList<int> rowList, double deadband);

private:
    static AiTraceTableManager *_aiTraceTableManager;
    QSqlTableModel *_model;
    void _refreshData();
};

#endif // AITRACETABLEMANAGER_H
