#ifndef SELECTDEVICECOLUMNDELEGATE_H
#define SELECTDEVICECOLUMNDELEGATE_H

#include <QItemDelegate>
#include <QObject>
class MapTableManager;

class SelectDeviceColumnDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    SelectDeviceColumnDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void deviceIdChanged(int row, int column, int newDeviceId);

private:
    MapTableManager *deviceMgr;
    QWidget *parentWidget;
};

#endif // SELECTDEVICECOLUMNDELEGATE_H
