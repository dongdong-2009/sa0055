#include "calcvartablemodel.h"
#include "constvalue.h"

CalcVarTableModel::CalcVarTableModel(QList<int> &calcVarIdList, QList<QList<SelectPoint> > &pointList, QObject *parent)
    : QAbstractTableModel(parent), calcVarIdList(calcVarIdList), pointList(pointList)
{
}

QVariant CalcVarTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::ItemDataRole::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Orientation::Vertical)
    {
        if(section > 0)
        {
            return section;
        }
        else
        {
            return QVariant();
        }
    }
    else
    {
        if(section == 0)
        {
            return "计算量名称";
        }
        else if(section >= FormulaVarMaxNum * 3 + 1)
        {
            return QVariant();
        }
        else
        {
            if(section % 3 == 0)
            {
                return QString("V%1").arg(section, 2, 10, QChar('0'));
            }
            else
            {
                return QVariant();
            }
        }
    }
}

int CalcVarTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return calcVarIdList.size();
}

int CalcVarTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return FormulaVarMaxNum * 3 + 1;
}

QVariant CalcVarTableModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::ItemDataRole::DisplayRole)
    {
        return QVariant();
    }

    if (!index.isValid())
    {
        return QVariant();
    }

    int row = index.row();
    int column = index.column();
    if(row >= calcVarIdList.size() || row >= pointList.size())
    {
        return QVariant();
    }


    if(column == 0)
    {
        return calcVarIdList.at(row);
    }
    else
    {
        int num1 = column / 3 - 1;
        int num2 = column % 3;

        if(num1 >= pointList.at(row).size())
        {
            return QVariant();
        }

        SelectPoint p = pointList.at(row).at(num1);
        if(num2 == 0)
        {
            return p.getPointDescId();
        }
        else if(num2 == 1)
        {
            return p.getDeviceId();
        }
        else
        {
            return p.getType();
        }
    }
}

bool CalcVarTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

}
