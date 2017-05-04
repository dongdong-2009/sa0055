#include "integercolumndelegate.h"
#include "logutil.h"
#include <QSpinBox>

IntegerColumnDelegate::IntegerColumnDelegate(int lowLimit, int highLimit, QObject *parent)
    : QItemDelegate(parent), lowLimit(lowLimit), highLimit(highLimit)
{
}

QWidget *IntegerColumnDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/) const
{
    QSpinBox *spinBox = new QSpinBox(parent);
    connect(spinBox, &QSpinBox::editingFinished, this, &IntegerColumnDelegate::commitAndClose);
    spinBox->setRange(lowLimit, highLimit);
    return spinBox;
}

void IntegerColumnDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
    bool ok;
    int value = index.data(Qt::ItemDataRole::EditRole).toInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }

    spinBox->setValue(value);
}

void IntegerColumnDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
    model->setData(index, spinBox->value(), Qt::ItemDataRole::EditRole);
}

void IntegerColumnDelegate::commitAndClose()
{
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(sender());
    commitData(spinBox);
    closeEditor(spinBox);
}
