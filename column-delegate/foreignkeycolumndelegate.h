#ifndef FOREIGNKEYCOLUMNDELEGATE_H
#define FOREIGNKEYCOLUMNDELEGATE_H

#include <QItemDelegate>
class QSqlTableModel;
class AbstractMapTableManager;

class ForeignKeyColumnDelegate : public QItemDelegate
{
public:
    ForeignKeyColumnDelegate(AbstractMapTableManager *manager, bool canModify = true, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    AbstractMapTableManager *mgr;
    bool canModify;
    void commitAndClose();
};

#endif // FOREIGNKEYCOLUMNDELEGATE_H
