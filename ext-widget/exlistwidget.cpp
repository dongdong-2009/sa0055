#include "exlistwidget.h"
#include "logutil.h"

ExListWidget::ExListWidget(QWidget *parent) : QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);

    connect(this, &QListWidget::itemPressed, [=](QListWidgetItem *item) {
        if (item->checkState() == Qt::CheckState::Checked)
        {
            item->setCheckState(Qt::CheckState::Unchecked);
        }
        else
        {
            item->setCheckState(Qt::CheckState::Checked);
        }

        emit valueChanged(getValue());
    });
}

void ExListWidget::initializeListItem(QString listItemString)
{
    bool ok;
    valueList.clear();
    nameList.clear();

    QStringList stringList = listItemString.split("|", QString::SkipEmptyParts);
    for (QString str : stringList)
    {
        QStringList itemPair = str.split(":");
        Q_ASSERT(itemPair.size() == 2);

        int num = itemPair.at(0).toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail").arg("toInt");
        }

        Q_ASSERT(num > 0 && num <= 32);
        valueList.append(num);
        nameList.append(itemPair.at(1));
    }

    clear();
    if (nameList.size() > 0)
    {
        setVisible(true);
        addItems(nameList);
    }
    else
    {
        setVisible(false);
    }
}

void ExListWidget::setValue(int value)
{
    // 1:越上限|2:越下限|3:越上上限|4:越下下限|5:遥测恢复

    for (int i = 0; i < valueList.size(); i++)
    {
        int shiftNum = valueList.at(i) - 1;
        if ((value & (0x01 << shiftNum)) != 0)
        {
            item(i)->setCheckState(Qt::CheckState::Checked);
        }
        else
        {
            item(i)->setCheckState(Qt::CheckState::Unchecked);
        }
    }
}

int ExListWidget::getValue()
{
    int result = 0;

    for (int i = 0; i < valueList.size(); i++)
    {
        if (item(i)->checkState() == Qt::CheckState::Checked)
        {
            int shiftNum = valueList.at(i) - 1;
            result |= (0x01 << shiftNum);
        }
    }

    return result;
}
