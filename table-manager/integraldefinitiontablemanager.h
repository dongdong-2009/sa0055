#ifndef INTEGRALDEFINITIONTABLEMANAGER_H
#define INTEGRALDEFINITIONTABLEMANAGER_H

#include <QObject>

class QSqlTableModel;

class IntegralDefinitionTableManager
{
public:
    enum ColumnIndex
    {
        SeqNum,
        IntegralId,
        SamplingInterval,
        WriteDbInterval,
        ClearFlag,
        DeviceId,
        AiId
    };

    IntegralDefinitionTableManager(QObject *parent = 0);
    QSqlTableModel *model()
    {
        return _model;
    }

    void addNewRecord(int integralId);
    void deleteRecord(int row, int count);

    static IntegralDefinitionTableManager *integralDefTableManager();

    void refreshData();
    void submitData();
private:
    QSqlTableModel *_model;
    static IntegralDefinitionTableManager *_integralDefTableManager;
};

#endif // INTEGRALDEFINITIONTABLEMANAGER_H
