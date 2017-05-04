#ifndef SELECTDEVICEDIALOG_H
#define SELECTDEVICEDIALOG_H

#include "selectpoint.h"
#include <QDialog>

namespace Ui
{
class SelectDeviceDialog;
}

class SelectDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDeviceDialog(QWidget *parent = 0, bool isMultiSelection = false);
    ~SelectDeviceDialog();

signals:
    void selectedOk(QList<int> deviceIdList);

private:
    Ui::SelectDeviceDialog *ui;
    void _checkResult();
};

#endif // SELECTDEVICEDIALOG_H
