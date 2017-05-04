#ifndef DESCRIPTIONCOLUMNDELEGATE_H
#define DESCRIPTIONCOLUMNDELEGATE_H

#include <QItemDelegate>

class MapTableManager;

class DescriptionColumnDelegate : public QItemDelegate
{
public:
    DescriptionColumnDelegate(MapTableManager *mapMgr, QObject *parent = Q_NULLPTR);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    MapTableManager *mapMgr;
};

#endif // DESCRIPTIONCOLUMNDELEGATE_H
