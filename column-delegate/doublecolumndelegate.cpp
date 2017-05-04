#include "doublecolumndelegate.h"
#include "logutil.h"
#include <QSpinBox>

DoubleColumnDelegate::DoubleColumnDelegate(double lowLimit, double highLimit, int decimals, QObject *parent)
    : QItemDelegate(parent), lowLimit(lowLimit), highLimit(highLimit), decimals(decimals)
{
}

void DoubleColumnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool ok;
    double value = index.data(Qt::ItemDataRole::EditRole).toDouble(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toDouble");
    }
    drawDisplay(painter, option, option.rect, QString::number(value, 'f', decimals));
}

QWidget *DoubleColumnDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/) const
{
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(parent);
    connect(spinBox, &QSpinBox::editingFinished, this, &DoubleColumnDelegate::commitAndClose);
    spinBox->setRange(lowLimit, highLimit);
    spinBox->setDecimals(decimals);
    return spinBox;
}

void DoubleColumnDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    bool ok;
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox *>(editor);
    double value = index.data(Qt::ItemDataRole::EditRole).toDouble(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toDouble");
    }

    spinBox->setValue(value);
}

void DoubleColumnDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox *>(editor);
    model->setData(index, spinBox->value(), Qt::ItemDataRole::EditRole);
}

void DoubleColumnDelegate::commitAndClose()
{
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox *>(sender());
    commitData(spinBox);
    closeEditor(spinBox);
}
