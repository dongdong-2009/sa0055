#include "foreignkeycolumndelegate.h"
#include "abstractmaptablemanager.h"
#include <QtSql>

ForeignKeyColumnDelegate::ForeignKeyColumnDelegate(AbstractMapTableManager *manager, bool canModify, QObject *parent)
    : QItemDelegate(parent), mgr(manager), canModify(canModify)
{
}

void ForeignKeyColumnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool ok;
    int value = index.data(Qt::ItemDataRole::EditRole).toInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }

    drawDisplay(painter, option, option.rect, mgr->nameFromId(value));
}

QWidget *ForeignKeyColumnDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/) const
{
    if (!canModify)
    {
        return 0;
    }
    else
    {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->setModel(mgr->model());
        comboBox->setModelColumn(mgr->nameColumnIndex());
        connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ForeignKeyColumnDelegate::commitAndClose);
        return comboBox;
    }
}

void ForeignKeyColumnDelegate::commitAndClose()
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    commitData(comboBox);
    closeEditor(comboBox);
}

void ForeignKeyColumnDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    bool ok;
    int value = index.data(Qt::ItemDataRole::EditRole).toInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }

    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    comboBox->setCurrentText(mgr->nameFromId(value));
}

void ForeignKeyColumnDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    int val = mgr->idFromName(comboBox->currentText());
    model->setData(index, val, Qt::ItemDataRole::EditRole);
}
