#include "calcvarbindtablemodel.h"
#include "util.h"

CalcVarBindTableModel::CalcVarBindTableModel(QObject *parent) : QAbstractTableModel(parent)
{
    horizontalHeader << "变量"
                     << "装置"
                     << "类型"
                     << "绑定的数据点";
    formulaVarNum = 0;
}

QVariant CalcVarBindTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::ItemDataRole::DisplayRole || orientation != Qt::Orientation::Horizontal)
    {
        return QVariant();
    }

    if (section < 0 || section >= horizontalHeader.size())
    {
        return QVariant();
    }

    return horizontalHeader.at(section);
}

int CalcVarBindTableModel::rowCount(const QModelIndex & /*parent*/) const
{
    return formulaVarNum;
}

int CalcVarBindTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return horizontalHeader.size();
}

QVariant CalcVarBindTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::ItemDataRole::DisplayRole || !index.isValid())
    {
        return QVariant();
    }

    if (index.column() == 0)
    {
        return Util::formulaVarName(index.row() + 1);
    }
    else
    {
        if (index.column() == 1)
        {
            return pointList.at(index.row()).getDeviceId();
        }
        else if (index.column() == 2)
        {
            return pointList.at(index.row()).getPointType();
        }
        else
        {
            return pointList.at(index.row()).getPointDescId();
        }
    }
}

void CalcVarBindTableModel::updateRecord(int varNum, QList<CalcVarBindPoint> realBindPointList)
{
    beginResetModel();
    formulaVarNum = varNum;

    pointList.clear();
    for (int i = 0; i < varNum; i++)
    {
        pointList.append(CalcVarBindPoint(i));
    }

    foreach (CalcVarBindPoint p, realBindPointList)
    {
        if (p.getVarSeqNum() < varNum)
        {
            pointList[p.getVarSeqNum()].setDevicId(p.getDeviceId());
            pointList[p.getVarSeqNum()].setPointType(p.getPointType());
            pointList[p.getVarSeqNum()].setPointDescId(p.getPointDescId());
        }
    }

    endResetModel();
}
