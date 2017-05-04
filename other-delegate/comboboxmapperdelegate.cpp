#include "comboboxmapperdelegate.h"
#include "logutil.h"
#include <QComboBox>

ComboBoxMapperDelegate::ComboBoxMapperDelegate(QObject *parent) : QItemDelegate(parent)
{
}

void ComboBoxMapperDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    if (comboBox)
    {
        bool ok;
        int val = index.data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        QAbstractItemModel *mapModel = comboBox->model();
        for (int i = 0; i < mapModel->rowCount(); i++)
        {
            int id = mapModel->index(i, 0).data().toInt(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail:").arg("toInt") << mapModel->index(i, 0).data();
            }

            if (id == val)
            {
                comboBox->setCurrentIndex(i);
                break;
            }
        }

        return;
    }

    return QItemDelegate::setEditorData(editor, index);
}

void ComboBoxMapperDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    if (comboBox)
    {
        int val = comboBox->currentIndex();
        QAbstractItemModel *mapModel = comboBox->model();

        model->setData(index, mapModel->data(mapModel->index(val, 0)).toInt());
        return;
    }

    return QItemDelegate::setModelData(editor, model, index);
}
