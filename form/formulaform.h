#ifndef FORMULAFORM_H
#define FORMULAFORM_H

#include <QWidget>

namespace Ui
{
class FormulaForm;
}

class TempCalcVarTableModel;

class FormulaForm : public QWidget
{
    Q_OBJECT

public:
    explicit FormulaForm(QWidget *parent = 0);
    ~FormulaForm();
    void hideEvent(QHideEvent *event);
    void setCurrentFormulaId(int formulaId);

    QStringList configValidCheck();
private:
    Ui::FormulaForm *ui;
    TempCalcVarTableModel *tempCalcVarTableModel;

    int formulaId;
    int formulaType;
    int formulaVarNum;

    void updateCalcVarTable(int formulaId);
    void initComboBoxGroup();
    void comboBoxIndexChanged(int currentIndex);
    void initCalcVarTable();
    void hideUnusedVar(int formulaVarNum);
    void addNewCalcVar();
    void deleteCalcVar();
    bool validateFormula(bool silent = true);
    int checkFormulaVar(QString expr);
    void setFormulaValid(bool valid);
};

#endif // FORMULAFORM_H
