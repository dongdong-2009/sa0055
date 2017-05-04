#ifndef NEWFORMULADIALOG_H
#define NEWFORMULADIALOG_H

#include "datatype.h"
#include <QDialog>

namespace Ui
{
class NewFormulaDialog;
}

class NewFormulaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFormulaDialog(QWidget *parent = 0);
    ~NewFormulaDialog();
signals:
    void createFormulaOk(int formulaId, int formulaType);

private:
    Ui::NewFormulaDialog *ui;
    void okButtonClicked();
};

#endif // NEWFORMULADIALOG_H
