#ifndef DEVICEPARATABLEMANAGER_H
#define DEVICEPARATABLEMANAGER_H

#include <QList>

class QObject;
class QSqlTableModel;

class DeviceParaTableManager
{
protected:
    DeviceParaTableManager(QObject *parent = 0);

public:
    enum ColumnIndex
    {
        SeqNum,
        DeviceId,
        DeviceRunMode,
        NetworkMode,
        Dev1IpA,
        Dev1IpB,
        Dev2IpA,
        Dev2IpB
    };
    QSqlTableModel *model();
    int rowCountFromId(int deviceId);
    void addNewDevice(int deviceId);
    void deleteDevice(int deviceId);

    static DeviceParaTableManager *deviceParaTableManager();

    void submitData();
    void refreshData();
    QList<int> getDeviceIdList();
private:
    QSqlTableModel *_model;

    static DeviceParaTableManager *_deviceParaTableManager;
};

#endif // DEVICEPARATABLEMANAGER_H
