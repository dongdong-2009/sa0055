#ifndef DOUBLECOLUMNDELEGATE_H
#define DOUBLECOLUMNDELEGATE_H

#include <QItemDelegate>

class DoubleColumnDelegate : public QItemDelegate
{
public:
    DoubleColumnDelegate(double lowLimit, double highLimit, int decimals, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    double lowLimit;
    double highLimit;
    int decimals;
    void commitAndClose();
};

#endif // DOUBLECOLUMNDELEGATE_H
