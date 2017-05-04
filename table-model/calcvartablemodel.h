#ifndef CALCVARTABLEMODEL_H
#define CALCVARTABLEMODEL_H

#include <QAbstractTableModel>
#include "selectpoint.h"

class CalcVarTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CalcVarTableModel(QList<int> &calcVarIdList, QList<QList<SelectPoint> > &pointList, QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private:
    QList<int> &calcVarIdList;
    QList<QList<SelectPoint> > &pointList;
};

#endif // CALCVARTABLEMODEL_H
