#ifndef ENUMTABLEMANAGER_H
#define ENUMTABLEMANAGER_H

#include "abstractmaptablemanager.h"
#include "datatype.h"

#include <QObject>
class QSqlTableModel;

class EnumTableManager : public AbstractMapTableManager
{
public:
    EnumTableManager(QString mapTable, QString mapId = "id", QString mapName = "name", QObject *parent = 0);

    /*静态函数*/
    static EnumTableManager *doQualifierEnumManager();
    static EnumTableManager *doTypeEnumManager();
    static EnumTableManager *doStepTypeEnumManager();
    static EnumTableManager *deviceRunModeEnumManager();
    static EnumTableManager *serverRunModeEnumManager();
    static EnumTableManager *networkModeEnumManager();
    static EnumTableManager *typeIdEnumManager(DataType type);

    enum Iec104ByteNumType
    {
        COT,
        CommonAddr,
        IOA,
        MaxByteNumType
    };
    static EnumTableManager *byteNumEnumManager(Iec104ByteNumType type);

    static EnumTableManager *curveTypeEnumManager();
    static EnumTableManager *curveCalcMethodEnumManager();
    static EnumTableManager *curveUnitEnumManager();

    static EnumTableManager *boolEnumManager();
    static EnumTableManager *formulaTypeEnumManager();

    static EnumTableManager *cosDisplayEnumManager();

    static EnumTableManager *alarmTypeEnumManager();

    static EnumTableManager *dataTypeEnumManager();

private:
    static EnumTableManager *_doQualifierEnumManager;
    static EnumTableManager *_doTypeEnumManager;
    static EnumTableManager *_doStepTypeEnumManager;
    static EnumTableManager *_deviceRunModeEnumManager;
    static EnumTableManager *_serverRunModeEnumManager;
    static EnumTableManager *_networkModeEnumManager;
    static EnumTableManager *_typeIdEnumManager[DataType::DataTypeNum];
    static EnumTableManager *_byteNumEnumManager[Iec104ByteNumType::MaxByteNumType];

    static EnumTableManager *_curveTypeEnumManager;
    static EnumTableManager *_curveCalcMethodEnumManager;
    static EnumTableManager *_curveUnitEnumManager;

    static EnumTableManager *_boolEnumManager;
    static EnumTableManager *_formulaTypeEnumManager;

    static EnumTableManager *_cosDisplayEnumManager;
    static EnumTableManager *_alarmtypeEnumManager;

    static EnumTableManager *_dataTypeEnumManager;
};

#endif // ENUMTABLEMANAGER_H
