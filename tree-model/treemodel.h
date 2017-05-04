#ifndef SELECTPOINTTREEMODEL_H
#define SELECTPOINTTREEMODEL_H

#include "selectpoint.h"
#include "treenode.h"
#include <QAbstractItemModel>

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    TreeModel(QObject *parent = 0);
    ~TreeModel();

    void init2SelectPoint(SelectPoint::Types types = SelectPoint::Type::AI, SelectPoint::Source pointSource = SelectPoint::Source::All,
                                bool specifiedDeviceFlag = false, int specifiedDeviceId = -1);
    void init2ProjectTree();

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    TreeNode *_rootItem;
};

#endif // SELECTPOINTTREEMODEL_H
