#ifndef NEWALARMRULEDIALOG_H
#define NEWALARMRULEDIALOG_H

#include <QDialog>

namespace Ui
{
class NewAlarmRuleDialog;
}

class NewAlarmRuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewAlarmRuleDialog(QWidget *parent = 0);
    ~NewAlarmRuleDialog();

signals:
    void createAlarmRuleOk(int alarmRuleId, int alarmCauseId);

private:
    Ui::NewAlarmRuleDialog *ui;
    void okButtonClicked();
};

#endif // NEWALARMRULEDIALOG_H
