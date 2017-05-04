#ifndef INTEGRALFORMULAFORM_H
#define INTEGRALFORMULAFORM_H

#include <QWidget>

namespace Ui
{
class IntegralFormulaForm;
}

class IntegralFormulaForm : public QWidget
{
    Q_OBJECT

public:
    explicit IntegralFormulaForm(QWidget *parent = 0);
    ~IntegralFormulaForm();
    void hideEvent(QHideEvent *event);

    QStringList configValidCheck();
private:
    Ui::IntegralFormulaForm *ui;
    void addIntegralRecord();
    void deleteIntegralRecord();
};

#endif // INTEGRALFORMULAFORM_H
