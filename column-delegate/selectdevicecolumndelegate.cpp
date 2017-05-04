#include "selectdevicecolumndelegate.h"
#include "logutil.h"
#include "maptablemanager.h"
#include "selectdevicedialog.h"
#include <QEvent>

SelectDeviceColumnDelegate::SelectDeviceColumnDelegate(QObject *parent) : QItemDelegate(parent), parentWidget(qobject_cast<QWidget *>(parent))
{
    deviceMgr = MapTableManager::mapTableManager(MapTableManager::TableIndex::DEV);
}

void SelectDeviceColumnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool ok;
    int deviceId = index.data().toInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }

    QString deviceDesc = deviceMgr->nameFromId(deviceId);
    drawDisplay(painter, option, option.rect, deviceDesc);
}

bool SelectDeviceColumnDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() != QEvent::Type::MouseButtonDblClick)
    {
        return QItemDelegate::editorEvent(event, model, option, index);
    }

    bool ok;
    int oldDeviceId = index.data().toInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }

    SelectDeviceDialog *dialog = new SelectDeviceDialog(parentWidget);
    dialog->setModal(true);
    connect(dialog, &QDialog::finished, [=]() { delete dialog; });
    connect(dialog, &SelectDeviceDialog::selectedOk, [=](QList<int> list) {
        int deviceId = list.at(0);
        model->setData(index, deviceId);
        if (oldDeviceId != deviceId)
        {
            emit deviceIdChanged(index.row(), index.column(), deviceId);
        }
    });

    dialog->show();
    return QItemDelegate::editorEvent(event, model, option, index);
}

QWidget *SelectDeviceColumnDelegate::createEditor(QWidget * /*parent*/, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/) const
{
    return 0;
}
