#ifndef COMBOBOXITEMDELEGATE_H
#define COMBOBOXITEMDELEGATE_H

#include <QItemDelegate>

class ComboBoxItemDelegate : public QItemDelegate
{
public:
    ComboBoxItemDelegate(QAbstractItemModel *model, QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QAbstractItemModel *model;
    void commitAndClose();
};

#endif // COMBOBOXITEMDELEGATE_H
