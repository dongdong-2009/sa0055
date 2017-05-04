#ifndef INTEGERCOLUMNDELEGATE_H
#define INTEGERCOLUMNDELEGATE_H

#include <QItemDelegate>

class IntegerColumnDelegate : public QItemDelegate
{
public:
    IntegerColumnDelegate(int lowLimit = 0, int highLimit = 99, QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *refModel, const QModelIndex &index) const;

private:
    int lowLimit;
    int highLimit;
    void commitAndClose();
};

#endif // INTEGERCOLUMNDELEGATE_H
