#ifndef CALCVARBINDTABLEMODEL_H
#define CALCVARBINDTABLEMODEL_H

#include "calcvarbindpoint.h"
#include <QAbstractTableModel>

class CalcVarBindTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CalcVarBindTableModel(QObject *parent = 0);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void updateRecord(int varNum, QList<CalcVarBindPoint> pointList);

private:
    QStringList horizontalHeader;
    QList<CalcVarBindPoint> pointList;
    int formulaVarNum;
};

#endif // CALCVARBINDTABLEMODEL_H
