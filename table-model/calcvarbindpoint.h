#ifndef CALCVARBINDPOINT_H
#define CALCVARBINDPOINT_H

class CalcVarBindPoint
{
public:
    CalcVarBindPoint(int varSeqNum, int devId = -1, int pointType = -1, int pointDescId = -1)
        : varSeqNum(varSeqNum), deviceId(devId), pointType(pointType), pointDescId(pointDescId)
    {
    }

    int getVarSeqNum() const
    {
        return varSeqNum;
    }

    int getDeviceId() const
    {
        return deviceId;
    }

    int getPointType() const
    {
        return pointType;
    }

    int getPointDescId() const
    {
        return pointDescId;
    }

    void setVarSeqNum(int varSeqNum)
    {
        this->varSeqNum = varSeqNum;
    }

    void setDevicId(int devId)
    {
        this->deviceId = devId;
    }

    void setPointType(int pointType)
    {
        this->pointType = pointType;
    }

    void setPointDescId(int pointDescId)
    {
        this->pointDescId = pointDescId;
    }

private:
    int varSeqNum;
    int deviceId;
    int pointType;
    int pointDescId;
};

#endif // CALCVARBINDPOINT_H
