#ifndef MAPTABLEMANAGER_H
#define MAPTABLEMANAGER_H
#include "abstractmaptablemanager.h"

#include <QObject>
class QSqlTableModel;

class MapTableManager : public AbstractMapTableManager
{
protected:
    MapTableManager(QString mapTable, QString mapId = "id", QString mapName = "name", QString mapValid = "valid", QObject *parent = Q_NULLPTR);

public:
    enum Error
    {
        AlreadyExist = -10,
        EmptyDescr
    };

    enum TableIndex
    {
        DI,
        AI,
        DO,
        AO,
        CI,
        DEV,
        FORMULA,
        INTEGRAL,
        CALCVAR,
        PLANCURVE,
        ALARMRULE,
        MAX
    };

    /*自有函数*/
    int validColumnIndex() const
    {
        return mapValidColumnIndex;
    }

    virtual int idFromName(QString name) const;
    virtual QString nameFromId(int id) const;

    int modifyDesc(int oldId, QString oldDesc, QString newDesc);
    int addDesc(QString newDesc);
    void deleteDesc(int oldId);
    void deleteDesc(QList<int> oldIdList);
    int currentMaxId();
    void copyMap2Model();
    void copyModel2Map();

    /*静态函数*/
    static MapTableManager *mapTableManager(TableIndex index);
    static void CopyMap2Model();
    static void CopyModel2Map();
private:
    class MapRecord
    {
    public:
        MapRecord()
        {
            _id = -1;
            _name = "";
            _valid = false;
        }

        MapRecord(int id, QString name, bool valid) : _id(id), _name(name), _valid(valid)
        {
        }

        int id() const
        {
            return _id;
        }

        void setId(int id)
        {
            _id = id;
        }

        QString name() const
        {
            return _name;
        }

        void setName(const QString &name)
        {
            _name = name;
        }

        bool valid() const
        {
            return _valid;
        }

        void setValid(bool valid)
        {
            _valid = valid;
        }

    private:
        int _id;
        QString _name;
        bool _valid;
    };

    QHash<int, MapRecord*> id4MapRecord;
    QHash<QString, MapRecord*> name4MapRecord;
    int _currentMaxId;

    int mapValidColumnIndex;
    /*静态变量*/
    static MapTableManager *_mapTableManager[TableIndex::MAX];
};

#endif // MAPTABLEMANAGER_H
