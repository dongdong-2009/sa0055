#ifndef DATATRACEFORM_H
#define DATATRACEFORM_H

#include "selectpointdialog.h"

#include <QSqlTableModel>
#include <QWidget>

namespace Ui
{
class DataTraceForm;
}

class DataTraceForm : public QWidget
{
    Q_OBJECT

public:
    explicit DataTraceForm(QWidget *parent = 0);
    ~DataTraceForm();

    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

    bool saveData();
    bool fetchData();

private:
    Ui::DataTraceForm *ui;
    void addRecord(QList<SelectPoint> list);
    void deleteRecord();

    void setDeadband(const QModelIndex &index);
};

#endif // DATATRACEFORM_H
