#ifndef SELECTFILEPATHCOLUMNDELEGATE_H
#define SELECTFILEPATHCOLUMNDELEGATE_H

#include <QItemDelegate>

class SelectFilePathColumnDelegate : public QItemDelegate
{
public:
    SelectFilePathColumnDelegate(QObject *parent = 0, QString filter = "");
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    QWidget *parentWidget;
    QString fileFilter;
};

#endif // SELECTFILEPATHCOLUMNDELEGATE_H
