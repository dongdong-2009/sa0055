#ifndef SYSTEMPARAFORM_H
#define SYSTEMPARAFORM_H

#include <QWidget>

namespace Ui
{
class SystemParaForm;
}

class QSqlTableModel;

class GlobalParaForm : public QWidget
{
    Q_OBJECT

public:
    explicit GlobalParaForm(QWidget *parent = 0);
    ~GlobalParaForm();

    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private:
    Ui::SystemParaForm *ui;
    QSqlTableModel *model;
};

#endif // SYSTEMPARAFORM_H
