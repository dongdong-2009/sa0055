#ifndef EVENTALARMFORM_H
#define EVENTALARMFORM_H

#include "alarmcausetablemanager.h"
#include "selectpoint.h"
#include <QWidget>

namespace Ui
{
class EventAlarmForm;
}

class QDataWidgetMapper;
class SelectPointColumnDelegate;
class SelectDeviceColumnDelegate;

class EventAlarmForm : public QWidget
{
    Q_OBJECT

public:
    explicit EventAlarmForm(QWidget *parent = 0);
    ~EventAlarmForm();
    void setCurrentAlarmRuleId(int alarmRuleId);
    void hideEvent(QHideEvent *event);

private:
    QDataWidgetMapper *mapper;
    Ui::EventAlarmForm *ui;
    SelectPointColumnDelegate *selectPointDelegate[4];
    SelectPoint::Type selectPointType;
    SelectDeviceColumnDelegate *selectDeviceDelegate;
    bool selectDeviceFlag;

    void prepareData();
    int alarmRuleId;
    void saveData();
    void addNewBindRecord();
    void updateBindTable(AlarmCauseTableManager::BindPointType bindPointType, int alarmCauseId);
    void batchSetValue(const QModelIndex &index);
};

#endif // EVENTALARMFORM_H
