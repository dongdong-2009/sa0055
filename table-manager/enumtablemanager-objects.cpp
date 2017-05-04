#include "enumtablemanager.h"

EnumTableManager *EnumTableManager::_doQualifierEnumManager = 0;
EnumTableManager *EnumTableManager::_doStepTypeEnumManager = 0;
EnumTableManager *EnumTableManager::_doTypeEnumManager = 0;
EnumTableManager *EnumTableManager::_deviceRunModeEnumManager = 0;
EnumTableManager *EnumTableManager::_serverRunModeEnumManager = 0;
EnumTableManager *EnumTableManager::_networkModeEnumManager = 0;
EnumTableManager *EnumTableManager::_typeIdEnumManager[DataType::DataTypeNum] = {0, 0, 0, 0, 0};
EnumTableManager *EnumTableManager::_byteNumEnumManager[Iec104ByteNumType::MaxByteNumType] = {0, 0, 0};

EnumTableManager *EnumTableManager::_curveTypeEnumManager = 0;
EnumTableManager *EnumTableManager::_curveCalcMethodEnumManager = 0;
EnumTableManager *EnumTableManager::_curveUnitEnumManager = 0;

EnumTableManager *EnumTableManager::_boolEnumManager = 0;
EnumTableManager *EnumTableManager::_formulaTypeEnumManager = 0;
EnumTableManager *EnumTableManager::_cosDisplayEnumManager = 0;
EnumTableManager *EnumTableManager::_alarmtypeEnumManager = 0;

EnumTableManager *EnumTableManager::_dataTypeEnumManager = 0;

const QString TypeIdTableName[] = {"sa0055_di_frame_type_enum", "sa0055_ai_frame_type_enum", "sa0055_do_frame_type_enum", "sa0055_ao_frame_type_enum",
                                   "sa0055_ci_frame_type_enum"};

EnumTableManager *EnumTableManager::doQualifierEnumManager()
{
    if (!_doQualifierEnumManager)
    {
        _doQualifierEnumManager = new EnumTableManager("sa0055_do_qualifier_enum");
    }

    return _doQualifierEnumManager;
}

EnumTableManager *EnumTableManager::doStepTypeEnumManager()
{
    if (!_doStepTypeEnumManager)
    {
        _doStepTypeEnumManager = new EnumTableManager("sa0055_do_step_type_enum");
    }

    return _doStepTypeEnumManager;
}

EnumTableManager *EnumTableManager::doTypeEnumManager()
{
    if (!_doTypeEnumManager)
    {
        _doTypeEnumManager = new EnumTableManager("sa0055_do_type_enum");
    }

    return _doTypeEnumManager;
}

EnumTableManager *EnumTableManager::deviceRunModeEnumManager()
{
    if (!_deviceRunModeEnumManager)
    {
        _deviceRunModeEnumManager = new EnumTableManager("sa0055_device_run_mode_enum");
    }

    return _deviceRunModeEnumManager;
}

EnumTableManager *EnumTableManager::serverRunModeEnumManager()
{
    if (!_serverRunModeEnumManager)
    {
        _serverRunModeEnumManager = new EnumTableManager("sa0055_server_run_mode_enum");
    }

    return _serverRunModeEnumManager;
}

EnumTableManager *EnumTableManager::networkModeEnumManager()
{
    if (!_networkModeEnumManager)
    {
        _networkModeEnumManager = new EnumTableManager("sa0055_network_mode_enum");
    }

    return _networkModeEnumManager;
}

EnumTableManager *EnumTableManager::typeIdEnumManager(DataType type)
{
    if (!_typeIdEnumManager[type])
    {
        _typeIdEnumManager[type] = new EnumTableManager(TypeIdTableName[type]);
    }

    return _typeIdEnumManager[type];
}

EnumTableManager *EnumTableManager::byteNumEnumManager(Iec104ByteNumType type)
{
    if (!_byteNumEnumManager[type])
    {
        _byteNumEnumManager[type] = new EnumTableManager("sa0055_iec104_byte_num_enum");

        _byteNumEnumManager[type]->model()->setFilter(type != Iec104ByteNumType::IOA ? "id < 3" : "id > 1");
        _byteNumEnumManager[type]->model()->select();
    }

    return _byteNumEnumManager[type];
}

EnumTableManager *EnumTableManager::curveTypeEnumManager()
{
    if (!_curveTypeEnumManager)
    {
        _curveTypeEnumManager = new EnumTableManager("sa0055_curve_type_enum");
    }
    return _curveTypeEnumManager;
}

EnumTableManager *EnumTableManager::curveCalcMethodEnumManager()
{
    if (!_curveCalcMethodEnumManager)
    {
        _curveCalcMethodEnumManager = new EnumTableManager("sa0055_curve_calc_method_enum");
    }
    return _curveCalcMethodEnumManager;
}

EnumTableManager *EnumTableManager::curveUnitEnumManager()
{
    if (!_curveUnitEnumManager)
    {
        _curveUnitEnumManager = new EnumTableManager("sa0055_curve_unit_enum");
    }

    return _curveUnitEnumManager;
}

EnumTableManager *EnumTableManager::boolEnumManager()
{
    if (!_boolEnumManager)
    {
        _boolEnumManager = new EnumTableManager("sa0055_bool_enum");
    }

    return _boolEnumManager;
}

EnumTableManager *EnumTableManager::formulaTypeEnumManager()
{
    if (!_formulaTypeEnumManager)
    {
        _formulaTypeEnumManager = new EnumTableManager("sa0055_formula_type_enum");
    }

    return _formulaTypeEnumManager;
}

EnumTableManager *EnumTableManager::cosDisplayEnumManager()
{
    if (!_cosDisplayEnumManager)
    {
        _cosDisplayEnumManager = new EnumTableManager("sa0055_cos_display_enum");
    }

    return _cosDisplayEnumManager;
}

EnumTableManager *EnumTableManager::alarmTypeEnumManager()
{
    if (!_alarmtypeEnumManager)
    {
        _alarmtypeEnumManager = new EnumTableManager("sa0055_alarm_type_enum");
    }

    return _alarmtypeEnumManager;
}

EnumTableManager *EnumTableManager::dataTypeEnumManager()
{
    if(!_dataTypeEnumManager)
    {
        _dataTypeEnumManager = new EnumTableManager("sa0055_data_type_enum");
    }

    return _dataTypeEnumManager;
}
