#include "selectcolorcolumndelegate.h"
#include "logutil.h"
#include <QColorDialog>
#include <QEvent>
#include <QPainter>

SelectColorColumnDelegate::SelectColorColumnDelegate(QObject *parent) : QItemDelegate(parent), parentWidget(qobject_cast<QWidget *>(parent))
{
}

void SelectColorColumnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool ok;
    uint color = index.data().toUInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }

    painter->fillRect(option.rect, QColor::fromRgb(color));
}

QWidget *SelectColorColumnDelegate::createEditor(QWidget * /*parent*/, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/) const
{
    return 0;
}

bool SelectColorColumnDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() != QEvent::Type::MouseButtonDblClick)
    {
        return QItemDelegate::editorEvent(event, model, option, index);
    }

    bool ok;
    uint color = index.data().toUInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }

    QColor selectColor =
        QColorDialog::getColor(QColor::fromRgb(color), parentWidget, "选择颜色", QColorDialog::ColorDialogOption::DontUseNativeDialog);
    if (selectColor.isValid())
    {
        model->setData(index, selectColor.rgb() & 0xFFFFFF);
    }

    return QItemDelegate::editorEvent(event, model, option, index);
}
