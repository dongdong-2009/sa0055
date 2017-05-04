#ifndef ABSTRACTMAPTABLEMANAGER_H
#define ABSTRACTMAPTABLEMANAGER_H

#include "logutil.h"
#include <QObject>
#include <QSqlTableModel>

class AbstractMapTableManager
{
protected:
    AbstractMapTableManager(QString mapTable, QString mapId, QString mapName, QObject *parent) : _tableName(mapTable)
    {
        mapModel = new QSqlTableModel(parent);
        mapModel->setTable(mapTable);

        mapIdColumnIndex = mapModel->fieldIndex(mapId);
        mapNameColumnIndex = mapModel->fieldIndex(mapName);

        refresh();
    }

public:
    QSqlTableModel *model() const
    {
        return mapModel;
    }

    virtual int idFromName(QString name) const
    {
        if (!name.isEmpty())
        {
            for (int i = 0; i < model()->rowCount(); i++)
            {
                if (model()->index(i, nameColumnIndex()).data().toString() == name)
                {
                    bool ok;
                    int id = model()->index(i, idColumnIndex()).data().toInt(&ok);
                    if (!ok)
                    {
                        ERROR << QString("call %1 fail").arg("toInt");
                    }

                    return id;
                }
            }
        }
        return -1;
    }

    virtual QString nameFromId(int id) const
    {
        if (id >= 0)
        {
            bool ok;
            for (int i = 0; i < model()->rowCount(); i++)
            {
                int tempId = model()->index(i, idColumnIndex()).data().toInt(&ok);
                if (!ok)
                {
                    ERROR << QString("call %1 fail").arg("toInt");
                }

                if (tempId == id)
                {
                    return model()->index(i, nameColumnIndex()).data().toString();
                }
            }
        }

        return "";
    }

    int idColumnIndex() const
    {
        return mapIdColumnIndex;
    }

    int nameColumnIndex() const
    {
        return mapNameColumnIndex;
    }

    QString tableName() const
    {
        return _tableName;
    }

    void refresh()
    {
        if (!model()->select())
        {
            ERROR << QString("call %1 fail").arg("select");
        }

        while (model()->canFetchMore())
        {
            model()->fetchMore();
        }
    }

private:
    QSqlTableModel *mapModel;
    int mapIdColumnIndex;
    int mapNameColumnIndex;
    QString _tableName;
};

#endif // ABSTRACTMAPTABLEMANAGER_H
