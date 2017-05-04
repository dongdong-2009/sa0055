#include "globalparadelegate.h"

#include "logutil.h"
#include <QComboBox>
#include <QSpinBox>

GlobalParaDelegate::GlobalParaDelegate()
{
}

void GlobalParaDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == ColumnIndex::Value)
    {
        bool ok;
        int type = index.model()->index(index.row(), ColumnIndex::Type).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        if (type == ParaType::Bool_Type)
        {
            QString text = index.model()->index(index.row(), ColumnIndex::Value).data().toInt(&ok) == 0 ? "否" : "是";
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            drawDisplay(painter, option, option.rect, text);
            drawFocus(painter, option, option.rect);
            return;
        }
    }

    return QItemDelegate::paint(painter, option, index);
}

void GlobalParaDelegate::commitSpinBoxData()
{
    QAbstractSpinBox *spinBox = static_cast<QAbstractSpinBox *>(sender());
    emit commitData(spinBox);
    emit closeEditor(spinBox);
}

void GlobalParaDelegate::commitComboBoxData()
{
    QComboBox *comboBox = static_cast<QComboBox *>(sender());
    emit commitData(comboBox);
    emit closeEditor(comboBox);
}

QWidget *GlobalParaDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == ColumnIndex::Name)
    {
        return 0;
    }
    else if (index.column() == ColumnIndex::Value)
    {
        bool ok;
        int type = index.model()->index(index.row(), ColumnIndex::Type).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        if (type == ParaType::Integer_Type)
        {
            QSpinBox *spinBox = new QSpinBox(parent);
            spinBox->setMinimum(index.model()->index(index.row(), ColumnIndex::IntLowLimit).data().toInt(&ok));
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            spinBox->setMaximum(index.model()->index(index.row(), ColumnIndex::IntHighLimit).data().toInt(&ok));
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            connect(spinBox, &QSpinBox::editingFinished, this, &GlobalParaDelegate::commitSpinBoxData);

            return spinBox;
        }
        else if (type == ParaType::Double_Type)
        {
            QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox(parent);

            doubleSpinBox->setMinimum(index.model()->index(index.row(), ColumnIndex::DoubleLowLimit).data().toDouble(&ok));
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toDouble");
            }

            doubleSpinBox->setDecimals(5);
            doubleSpinBox->setMaximum(index.model()->index(index.row(), ColumnIndex::DoubleHighLimit).data().toDouble(&ok));
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toDouble");
            }

            connect(doubleSpinBox, &QSpinBox::editingFinished, this, &GlobalParaDelegate::commitSpinBoxData);
            return doubleSpinBox;
        }
        else if (type == ParaType::Bool_Type)
        {
            QComboBox *comboBox = new QComboBox(parent);
            comboBox->addItems(QStringList() << "否"
                                             << "是");
            connect(comboBox, &QComboBox::currentTextChanged, this, &GlobalParaDelegate::commitComboBoxData);
            return comboBox;
        }
    }

    return QItemDelegate::createEditor(parent, option, index);
}

void GlobalParaDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == ColumnIndex::Value)
    {
        QModelIndex typeIndex = index.model()->index(index.row(), ColumnIndex::Type);
        int type = index.model()->data(typeIndex).toInt();
        if (type == ParaType::Integer_Type)
        {
            QSpinBox *spinBox = static_cast<QSpinBox *>(editor);
            spinBox->setValue(index.model()->data(index).toInt());
            return;
        }
        else if (type == ParaType::Double_Type)
        {
            QDoubleSpinBox *doubleSpinBox = static_cast<QDoubleSpinBox *>(editor);
            doubleSpinBox->setValue(index.model()->data(index).toDouble());
            return;
        }
        else if (type == ParaType::Bool_Type)
        {
            QComboBox *comboBox = static_cast<QComboBox *>(editor);
            comboBox->setCurrentIndex(index.model()->data(index).toInt());
            return;
        }
    }

    return QItemDelegate::setEditorData(editor, index);
}

void GlobalParaDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == ColumnIndex::Value)
    {
        QModelIndex typeIndex = index.model()->index(index.row(), ColumnIndex::Type);
        int type = index.model()->data(typeIndex).toInt();
        if (type == ParaType::Integer_Type)
        {
            QSpinBox *spinBox = static_cast<QSpinBox *>(editor);
            model->setData(index, spinBox->value());
            return;
        }
        else if (type == ParaType::Double_Type)
        {
            QDoubleSpinBox *doubleSpinBox = static_cast<QDoubleSpinBox *>(editor);
            model->setData(index, doubleSpinBox->value());
            return;
        }
        else if (type == ParaType::Bool_Type)
        {
            QComboBox *comboBox = static_cast<QComboBox *>(editor);
            model->setData(index, comboBox->currentIndex());
            return;
        }
    }

    return QItemDelegate::setModelData(editor, model, index);
}

void GlobalParaDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::updateEditorGeometry(editor, option, index);
}
