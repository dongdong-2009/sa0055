#ifndef PROTOCOLPARAITEMDELEGATE_H
#define PROTOCOLPARAITEMDELEGATE_H

#include <QItemDelegate>

class ComboBoxMapperDelegate : public QItemDelegate
{
public:
    ComboBoxMapperDelegate(QObject *parent = 0);

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // PROTOCOLPARAITEMDELEGATE_H
