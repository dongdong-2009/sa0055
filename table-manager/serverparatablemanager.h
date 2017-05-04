#ifndef SERVERPARATABLEMANAGER_H
#define SERVERPARATABLEMANAGER_H

#include <QObject>
class QSqlTableModel;

class ServerParaTableManager
{
protected:
    ServerParaTableManager(QObject *parent = 0);

public:
    enum ColumnIndex
    {
        ServerRunMode,
        NetworkMode,
        Server1IpA,
        Server1IpB,
        Server2IpA,
        Server2IpB,
        VirtualIpA,
        VirtualIpB
    };

    QSqlTableModel *model()
    {
        return _model;
    }

    static ServerParaTableManager *serverParaTableManager();

    void submitData();
private:
    QSqlTableModel *_model;
    static ServerParaTableManager *_serverParaTableManager;
};

#endif // SERVERPARATABLEMANAGER_H
