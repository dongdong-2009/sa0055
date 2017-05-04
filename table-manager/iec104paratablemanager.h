#ifndef IEC104PARATABLEMANAGER_H
#define IEC104PARATABLEMANAGER_H

#include <QObject>
class QSqlTableModel;

class Iec104ParaTableManager
{
protected:
    Iec104ParaTableManager(QObject *parent = 0);

public:
    enum ColumnIndex
    {
        SeqNum,
        DeviceId,
        AsduAddr,
        CotSize,
        AsduSize,
        IoaSize,
        DiIoaStart,
        AiIoaStart,
        DoIoaStart,
        AoIoaStart,
        CiIoaStart,
        DiTypeId,
        AiTypeId,
        DoTypeId,
        AoTypeId,
        CiTypeId
    };

    QSqlTableModel *model()
    {
        return _model;
    }

    static Iec104ParaTableManager *iec104ParaTableManager();
    int rowCountFromId(int deviceId);
    void addNewIec104ParaRecord(int deviceId);
    void deleteIec104ParaRecord(int deviceId);
    void refreshData();

    void submitData();
private:
    QSqlTableModel *_model;
    static Iec104ParaTableManager *_iec104ParaTableManager;
};

#endif // IEC104PARATABLEMANAGER_H
