#ifndef SELECTPOINTCOLUMNDELEGATE_H
#define SELECTPOINTCOLUMNDELEGATE_H

#include "datatype.h"
#include "selectpointdialog.h"
#include <QItemDelegate>
class MapTableManager;

class SelectPointColumnDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    SelectPointColumnDelegate(QObject *parent = 0, SelectPoint::Types pointTypes = SelectPoint::Type::AI,
                              int deviceColumnIndex = -1, int typeColumnIndex = -1, bool specifiedDeviceFlag = false, bool canModify = true);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void selectPointChanged(int row, int column, int deviceId, int pointType, int descId);

private:
    SelectPoint::Types pointTypes;
    int deviceColumnIndex;
    int dataTypeColumnIndex;
    bool specifiedDeviceFlag;
    QWidget *parentWidget;
    DataType pointDataType;
    bool canModify;
};

#endif // SELECTPOINTCOLUMNDELEGATE_H
