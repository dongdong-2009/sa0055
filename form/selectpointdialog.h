#ifndef SELECTPOINTDIALOG_H
#define SELECTPOINTDIALOG_H

#include "datatype.h"
#include "selectpointfilterproxymodel.h"
#include "selectpoint.h"
#include <QDialog>
#include <QTreeWidget>

namespace Ui
{
class SelectPointDialog;
}
class TreeModel;

class SelectPointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectPointDialog(QWidget *parent = 0, SelectPoint::Types types = SelectPoint::Type::AI,
                               SelectPoint::Mode mode = SelectPoint::Mode::Single, SelectPoint::Source pointSource = SelectPoint::Source::All,
                               bool specifiedDeviceFlag = false, int deviceId = -1);
    ~SelectPointDialog();

signals:
    void selectedOk(QList<SelectPoint> list);

private:
    Ui::SelectPointDialog *ui;
    SelectPoint::Types types;
    SelectPoint::Mode mode;

    TreeModel *model;
    SelectPointFilterProxyModel *proxyModel;

    void _checkSelectResult();
};

#endif // SELECTPOINTDIALOG_H
