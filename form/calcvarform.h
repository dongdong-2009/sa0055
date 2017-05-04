#ifndef CALCVALUEFORM_H
#define CALCVALUEFORM_H

#include <QWidget>

namespace Ui
{
class CalcValueForm;
}
class CalcVarBindTableModel;

class CalcVarForm : public QWidget
{
    Q_OBJECT

public:
    enum ColumnIndex
    {
        VarSeqNum,
        DeviceId,
        PointType,
        PointDesc
    };

    explicit CalcVarForm(QWidget *parent = 0);
    ~CalcVarForm();

    void setCurrentCalcVarId(int calcVarId);

private:
    Ui::CalcValueForm *ui;
    int calcVarId;
    CalcVarBindTableModel *model;
};

#endif // CALCVALUEFORM_H
