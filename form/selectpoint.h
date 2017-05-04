#ifndef SELECTPOINT_H
#define SELECTPOINT_H

#include "constvalue.h"
#include "datatype.h"
#include <QObject>

class SelectPoint
{
public:
    enum Mode
    {
        Single,  /*单选*/
        Multiple /*多选*/
    };

    enum Source
    {
        Device,  /*装置*/
        CalcVar, /*计算量*/
        All      /*装置或计算量*/
    };

    enum Type
    {
        INVALID = 0,
        DI = 0x01,
        AI = 0x02,
        DO = 0x04,
        AO = 0x08,
        CI = 0x10
    };
    Q_DECLARE_FLAGS(Types, Type)

    SelectPoint(DataType type = DataType::AI, int deviceId = -1, int pointDescId = -1) : _type(type), _deviceId(deviceId), _pointDescId(pointDescId)
    {
    }

    DataType getType() const { return _type; }

    int getDeviceId() const { return _deviceId; }

    int getPointDescId() const { return _pointDescId; }

    static QString getDescInfo(int deviceId, int pointType, int pointDescId, QString *pDeviceDesc = 0, QString *pPointDesc = 0);

private:
    DataType _type;
    int _deviceId;
    int _pointDescId;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SelectPoint::Types)
#endif // SELECTPOINT_H
