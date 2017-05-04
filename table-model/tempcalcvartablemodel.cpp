#include "tempcalcvartablemodel.h"
#include "constvalue.h"
#include "logutil.h"
#include "maptablemanager.h"
#include "util.h"

TempCalcVarTableModel::TempCalcVarTableModel(QObject *parent) : QAbstractTableModel(parent)
{
    headerStr << "计算量名称";
    for (int i = 1; i <= FormulaVarMaxNum; i++)
    {
        headerStr << QString("装置名称%1").arg(i) << QString("数据类型%1").arg(i) << Util::formulaVarName(i);
    }
}

QVariant TempCalcVarTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::ItemDataRole::DisplayRole || orientation != Qt::Orientation::Horizontal)
    {
        return QVariant();
    }

    return headerStr.at(section);
}

int TempCalcVarTableModel::rowCount(const QModelIndex & /*parent*/) const
{
    return calcVarIdList.size();
}

int TempCalcVarTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return headerStr.size();
}

QVariant TempCalcVarTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int column = index.column();

    if (!index.isValid() || (role != Qt::ItemDataRole::DisplayRole && role != Qt::ItemDataRole::EditRole))
    {
        return QVariant();
    }

    if (row >= bindPointListList.size())
    {
        return QVariant();
    }

    if (column == 0)
    {
        return calcVarIdList.at(row);
    }
    else
    {
        int varSeqNum = VarSeqNum(column);//(column + 2) / 3 - 1;
        int num = column % 3;

        if (varSeqNum >= bindPointListList.at(row).size())
        {
            return -1;
        }

        if (num == 0)
        {
            return bindPointListList.at(row).at(varSeqNum).getPointDescId();
        }
        else if (num == 1)
        {
            return bindPointListList.at(row).at(varSeqNum).getDeviceId();
        }
        else
        {
            return bindPointListList.at(row).at(varSeqNum).getPointType();
        }
    }
}

bool TempCalcVarTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::ItemDataRole::EditRole)
    {
        return false;
    }

    int row = index.row();
    if (row >= bindPointListList.size())
    {
        return false;
    }

    int column = index.column();
    bool ok;
    int val = value.toInt(&ok);
    if (!ok)
    {
        return false;
    }

    if (column == 0)
    {
        calcVarIdList[row] = val;
    }
    else
    {
        int varSeqNum = VarSeqNum(column);
        int num = column % 3;

        if (varSeqNum >= bindPointListList.at(row).size())
        {
            return false;
        }

        if (num == 0)
        {
            bindPointListList[row][varSeqNum].setPointDescId(val);
        }
        else if (num == 1)
        {
            bindPointListList[row][varSeqNum].setDevicId(val);
        }
        else
        {
            bindPointListList[row][varSeqNum].setPointType(val);
        }
    }

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags TempCalcVarTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QAbstractTableModel::flags(index);
    }

    return Qt::ItemFlag::ItemIsEditable | QAbstractTableModel::flags(index);
}

void TempCalcVarTableModel::updateAllRecord(QList<int> calcVarIdList, QList<QList<CalcVarBindPoint>> bindList)
{
    Q_ASSERT(calcVarIdList.size() == bindList.size());

    beginResetModel();
    this->calcVarIdList.clear();
    this->bindPointListList.clear();
    for (int i = 0; i < calcVarIdList.size(); i++)
    {
        _addNewRecord(calcVarIdList.at(i), bindList.at(i));
    }

    endResetModel();
}

void TempCalcVarTableModel::addNewRecord(int calcVarId, QList<CalcVarBindPoint> realBindPointList)
{
    beginResetModel();
    _addNewRecord(calcVarId, realBindPointList);
    endResetModel();
}

void TempCalcVarTableModel::_addNewRecord(int calcVarId, QList<CalcVarBindPoint> realBindPointList)
{
    QList<CalcVarBindPoint> rowBindPointList;
    for (int i = 0; i < FormulaVarMaxNum; i++)
    {
        rowBindPointList.append(CalcVarBindPoint(i));
    }

    foreach (CalcVarBindPoint p, realBindPointList)
    {
        Q_ASSERT(p.getVarSeqNum() < FormulaVarMaxNum);

        rowBindPointList[p.getVarSeqNum()].setDevicId(p.getDeviceId());
        rowBindPointList[p.getVarSeqNum()].setPointType(p.getPointType());
        rowBindPointList[p.getVarSeqNum()].setPointDescId(p.getPointDescId());
    }

    calcVarIdList.append(calcVarId);
    bindPointListList.append(rowBindPointList);
}

void TempCalcVarTableModel::deleteRecord(QList<int> delRowList)
{
    Q_ASSERT(delRowList.size() > 0);
    Q_ASSERT(calcVarIdList.size() == bindPointListList.size());

    beginResetModel();
    for (int i = delRowList.size() - 1; i >= 0; i--)
    {
        calcVarIdList.removeAt(delRowList.at(i));
        bindPointListList.removeAt(delRowList.at(i));
    }
    endResetModel();
}
