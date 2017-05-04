#ifndef CALCVARTABLEMODEL_H
#define CALCVARTABLEMODEL_H

#include "calcvarbindpoint.h"
#include <QAbstractTableModel>

class TempCalcVarTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    static int DeviceIdColumn(int varSeqNum /*0-based*/) { return 3 * (varSeqNum + 1) - 2; }

    static int DataTypeColumn(int varSeqNum /*0-based*/) { return 3 * (varSeqNum + 1) - 1; }

    static int PointDescIdColumn(int varSeqNum /*0-based*/) { return 3 * (varSeqNum + 1); }

    static int VarSeqNum(int column) { return (column - 1) / 3; }

    explicit TempCalcVarTableModel(QObject *parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void addNewRecord(int calcVarId, QList<CalcVarBindPoint> bindPointList = QList<CalcVarBindPoint>());
    void deleteRecord(QList<int> delRowList);
    void updateAllRecord(QList<int> calcVarIdList, QList<QList<CalcVarBindPoint>> bindList);

    const QList<int> &getCalcVarIdList() const { return calcVarIdList; }

    const QList<QList<CalcVarBindPoint>> &getBindPointListList() { return bindPointListList; }

private:
    QList<int> calcVarIdList;
    QList<QList<CalcVarBindPoint>> bindPointListList;
    QStringList headerStr;
    void _addNewRecord(int calcVarId, QList<CalcVarBindPoint> realBindPointList);
};

#endif // CALCVARTABLEMODEL_H
