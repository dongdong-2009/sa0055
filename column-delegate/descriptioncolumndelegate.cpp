#include "descriptioncolumndelegate.h"
#include "constvalue.h"
#include "logutil.h"
#include "maptablemanager.h"
#include <QLineEdit>
#include <QMessageBox>

#define OldId "OldId"
#define OldDesc "OldDesc"

DescriptionColumnDelegate::DescriptionColumnDelegate(MapTableManager *mapMgr, QObject *parent) : QItemDelegate(parent), mapMgr(mapMgr)
{
    Q_ASSERT(mapMgr);
}

void DescriptionColumnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool ok;
    int id = index.data(Qt::ItemDataRole::EditRole).toInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }
    drawDisplay(painter, option, option.rect, mapMgr->nameFromId(id));
}

QWidget *DescriptionColumnDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & /*option*/, const QModelIndex &index) const
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setMaxLength(DescriptionMaxLength);
    lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    bool ok;
    int oldId = index.data(Qt::ItemDataRole::EditRole).toInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }

    lineEdit->setProperty(OldId, oldId);
    lineEdit->setProperty(OldDesc, mapMgr->nameFromId(oldId));

    return lineEdit;
}

void DescriptionColumnDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    bool ok;
    int id = index.data(Qt::ItemDataRole::EditRole).toInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }

    lineEdit->setText(mapMgr->nameFromId(id));
}

void DescriptionColumnDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    int newId;
    bool ok;
    int oldId = lineEdit->property(OldId).toInt(&ok);
    if (!ok)
    {
        ERROR << QString("call %1 fail").arg("toInt");
    }

    QString oldDesc = lineEdit->property(OldDesc).toString();
    QString newDesc = lineEdit->text();

    if (newDesc.isEmpty())
    {
        QMessageBox::warning(0, "警告", "描述不能为空，请重新输入!");
        newId = oldId;
    }
    else
    {
        newId = mapMgr->modifyDesc(oldId, oldDesc, newDesc);
        if (newId == MapTableManager::Error::AlreadyExist)
        {
            QMessageBox::warning(0, "警告", "修改后的描述和其它正在使用的描述相同, 请重新输入!");
            newId = oldId;
        }
    }

    if (!model->setData(index, newId, Qt::ItemDataRole::EditRole))
    {
        ERROR << QString("call %1 fail").arg("setData");
    }
}
