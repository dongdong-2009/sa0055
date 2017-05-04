#ifndef SELECTCOLORCOLUMNDELEGATE_H
#define SELECTCOLORCOLUMNDELEGATE_H

#include <QItemDelegate>

class SelectColorColumnDelegate : public QItemDelegate
{
public:
    SelectColorColumnDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    QWidget *parentWidget;
};

#endif // SELECTCOLORCOLUMNDELEGATE_H
