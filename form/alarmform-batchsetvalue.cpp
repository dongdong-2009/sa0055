#include "eventalarmform.h"
#include "alarminfotablemanager.h"
#include "logutil.h"
#include "ui_alarmform.h"
#include <QInputDialog>
#include <QMessageBox>

void EventAlarmForm::batchSetValue(const QModelIndex &index)
{
    if (QApplication::mouseButtons() != Qt::MouseButton::RightButton)
    {
        return;
    }

    QModelIndexList modelIndexList = ui->tableView->selectionModel()->selectedRows(index.column());
    if (modelIndexList.size() < 2)
    {
        return;
    }

    QString label = QString("将所选记录的'%1'设置为:").arg(AlarmInfoTableManager::ColumnName[index.column()]);
    QVariant value;
    bool ok;
    int intValue;
    double doubleValue;
    QString strValue;
    QStringList items;

    switch (index.column())
    {
    case AlarmInfoTableManager::ColumnIndex::AiUpperLimit:
    case AlarmInfoTableManager::ColumnIndex::AiLowerLimit:
    case AlarmInfoTableManager::ColumnIndex::AiUpperUpperLimit:
    case AlarmInfoTableManager::ColumnIndex::AiLowerLowerLimit:
        doubleValue = modelIndexList.at(0).data().toDouble(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toDouble");
        }

        value = QInputDialog::getDouble(this, "设置", label, doubleValue, -99999999, 99999999, 6, &ok);
        if (!ok)
        {
            return;
        }
        break;
    case AlarmInfoTableManager::ColumnIndex::MustConfirmFlag:
    case AlarmInfoTableManager::ColumnIndex::PrintAlarmFlag:
        intValue = modelIndexList.at(0).data().toInt(&ok) == 0 ? 0 : 1;
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        items << "否"
              << "是";
        value = QInputDialog::getItem(this, "设置", label, items, intValue, false, &ok) == "否" ? 0 : 1;
        if (!ok)
        {
            return;
        }
        break;
    case AlarmInfoTableManager::ColumnIndex::UnconfirmedColor:
    case AlarmInfoTableManager::ColumnIndex::ConfirmedColor:

        intValue = modelIndexList.at(0).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        if (QMessageBox::question(this, "设置", QString("将所选记录的'%1'设置与第%2条记录相同?")
                                                    .arg(AlarmInfoTableManager::ColumnName[index.column()])
                                                    .arg(modelIndexList.at(0).row() + 1)) == QMessageBox::StandardButton::No)
        {
            return;
        }

        value = intValue;
        break;
    case AlarmInfoTableManager::ColumnIndex::SoundFilePath:
    case AlarmInfoTableManager::ColumnIndex::PushPicturePath:
        strValue = modelIndexList.at(0).data().toString();
        if (QMessageBox::question(this, "设置",
                                  QString("将所选记录的'%1'设置为:%2").arg(AlarmInfoTableManager::ColumnName[index.column()]).arg(strValue)) ==
            QMessageBox::StandardButton::No)
        {
            return;
        }

        value = strValue;
        break;
    case AlarmInfoTableManager::ColumnIndex::SoundPlayTime:
        intValue = modelIndexList.at(0).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        value = QInputDialog::getInt(this, "设置", label, intValue, 0, 100, 1, &ok);
        if (!ok)
        {
            return;
        }
        break;
    default:
        return;
    }

    QList<int> rowList;
    foreach (QModelIndex idx, modelIndexList)
    {
        rowList.append(idx.row());
    }

    AlarmInfoTableManager::alarmInfoTableManager()->setColumnValue(rowList, index.column(), value);
}
