#ifndef PLANCURVEFORM_H
#define PLANCURVEFORM_H

#include <QWidget>

namespace Ui
{
class PlanCurveForm;
}

class PlanCurveForm : public QWidget
{
    Q_OBJECT

public:
    explicit PlanCurveForm(QWidget *parent = 0);
    ~PlanCurveForm();
    void hideEvent(QHideEvent *event);

    QStringList configValidCheck();
private:
    Ui::PlanCurveForm *ui;
    void addRecord();
    void deleteRecord();
};

#endif // PLANCURVEFORM_H
