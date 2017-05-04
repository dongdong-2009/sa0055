#include "selectpointcolumndelegate.h"
#include "constvalue.h"
#include "maptablemanager.h"
#include "selectpointdialog.h"
#include "util.h"

#include "logutil.h"
#include <QEvent>

SelectPointColumnDelegate::SelectPointColumnDelegate(QObject *parent, SelectPoint::Types pointTypes, int deviceColumnIndex, int dataTypeColumnIndex,
                                                     bool specifiedDeviceFlag, bool canModify)
    : QItemDelegate(parent), pointTypes(pointTypes), deviceColumnIndex(deviceColumnIndex), dataTypeColumnIndex(dataTypeColumnIndex),
      specifiedDeviceFlag(specifiedDeviceFlag), parentWidget(qobject_cast<QWidget *>(parent)), canModify(canModify)
{
    bool onlyOneType;
    pointDataType = Util::selectPointType_2_dataType(pointTypes, &onlyOneType);
    Q_ASSERT((dataTypeColumnIndex == -1 && onlyOneType) || dataTypeColumnIndex != -1);
}

void SelectPointColumnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool ok;
    int descId = index.data().toInt(&ok);
    Q_ASSERT(ok);

    int deviceId = index.model()->index(index.row(), deviceColumnIndex).data().toInt(&ok);
    Q_ASSERT(ok);

    int dataType;
    if (dataTypeColumnIndex != -1)
    {
        dataType = index.model()->index(index.row(), dataTypeColumnIndex).data().toInt(&ok);
    }
    else
    {
        dataType = pointDataType;
    }

    QString desc = SelectPoint::getDescInfo(deviceId, dataType, descId);
    drawDisplay(painter, option, option.rect, desc);
}

bool SelectPointColumnDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() != QEvent::Type::MouseButtonDblClick || !canModify)
    {
        return QItemDelegate::editorEvent(event, model, option, index);
    }

    bool ok;
    int oldDeviceId = index.model()->index(index.row(), deviceColumnIndex).data().toInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }

    SelectPointDialog *dialog;
    dialog = new SelectPointDialog(parentWidget, pointTypes, SelectPoint::Mode::Single,
                                   specifiedDeviceFlag ? SelectPoint::Source::Device : SelectPoint::Source::All, specifiedDeviceFlag, oldDeviceId);
    dialog->setModal(true);
    connect(dialog, &QDialog::finished, [=]() { delete dialog; });
    connect(dialog, &SelectPointDialog::selectedOk, [=](QList<SelectPoint> list) {
        int descId = list.at(0).getPointDescId();
        int deviceId = list.at(0).getDeviceId();
        int pointType = list.at(0).getType();

        model->setData(index, descId);
        model->setData(index.model()->index(index.row(), deviceColumnIndex), deviceId);
        if (dataTypeColumnIndex != -1)
        {
            model->setData(index.model()->index(index.row(), dataTypeColumnIndex), pointType);
        }

        emit selectPointChanged(index.row(), index.column(), deviceId, pointType, descId);

        QSqlTableModel *tempModel = qobject_cast<QSqlTableModel *>(model);
        if (tempModel)
        {
            tempModel->submitAll();
        }
    });
    dialog->show();
    return QItemDelegate::editorEvent(event, model, option, index);
}

QWidget *SelectPointColumnDelegate::createEditor(QWidget * /*parent*/, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/) const
{
    return 0;
}
