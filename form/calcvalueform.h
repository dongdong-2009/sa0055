#ifndef CALCVALUEFORM_H
#define CALCVALUEFORM_H

#include <QWidget>

namespace Ui {
class CalcValueForm;
}

class CalcValueForm : public QWidget
{
    Q_OBJECT

public:
    explicit CalcValueForm(QWidget *parent = 0);
    ~CalcValueForm();

private:
    Ui::CalcValueForm *ui;
};

#endif // CALCVALUEFORM_H
