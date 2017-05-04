#include "selectfilepathcolumndelegate.h"
#include "logutil.h"
#include <QEvent>
#include <QFileDialog>
#include <QFileInfo>

SelectFilePathColumnDelegate::SelectFilePathColumnDelegate(QObject *parent, QString filter)
    : QItemDelegate(parent), parentWidget(qobject_cast<QWidget *>(parent)), fileFilter(filter)
{
}

QWidget *SelectFilePathColumnDelegate::createEditor(QWidget * /*parent*/, const QStyleOptionViewItem & /*option*/,
                                                    const QModelIndex & /*index*/) const
{
    return 0;
}

bool SelectFilePathColumnDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() != QEvent::Type::MouseButtonDblClick)
    {
        return QItemDelegate::editorEvent(event, model, option, index);
    }

    QString path = index.data().toString();
    QString dir = "";
    if (!path.isEmpty())
    {
        QFileInfo fileInfo(path);
        dir = fileInfo.filePath();
    }

    QString selectedFile = QFileDialog::getOpenFileName(parentWidget, "提示", dir, fileFilter);
    if (!selectedFile.isEmpty())
    {
        if (!model->setData(index, selectedFile))
        {
            ERROR << QString("call %1 fail").arg("setData");
        }
    }

    return QItemDelegate::editorEvent(event, model, option, index);
}
