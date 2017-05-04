#include "comboboxitemdelegate.h"
#include <QComboBox>

ComboBoxItemDelegate::ComboBoxItemDelegate(QAbstractItemModel *model, QObject *parent) : QItemDelegate(parent), model(model)
{
}

QWidget *ComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/) const
{
    QComboBox *comboBox = new QComboBox(parent);
    comboBox->setModel(model);
    connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ComboBoxItemDelegate::commitAndClose);
    return comboBox;
}

void ComboBoxItemDelegate::commitAndClose()
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    commitData(comboBox);
    closeEditor(comboBox);
}
