#ifndef PROXYMODELDESCRIPTIONFILTERPROXYMODEL_H
#define PROXYMODELDESCRIPTIONFILTERPROXYMODEL_H

#include "abstractmaptablemanager.h"
#include <QObject>
#include <QSortFilterProxyModel>

class SelectPointFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    SelectPointFilterProxyModel(QObject *parent = 0);

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

};

#endif // PROXYMODELDESCRIPTIONFILTERPROXYMODEL_H
