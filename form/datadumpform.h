#ifndef DATADUMPFORM_H
#define DATADUMPFORM_H

#include "selectpointdialog.h"
#include <QSqlTableModel>
#include <QWidget>

namespace Ui
{
class DataDumpForm;
}

class DataDumpForm : public QWidget
{
    Q_OBJECT

public:
    explicit DataDumpForm(QWidget *parent = 0);
    ~DataDumpForm();

    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

    void saveData();
    void fetchData();

private:
    Ui::DataDumpForm *ui;
    QSqlTableModel *model;
    void addRecord(QList<SelectPoint> list);
    void deleteRecord();

    void setInterval(const QModelIndex &index);
};

#endif // DATADUMPFORM_H
