#ifndef NEWDEVICEDIALOG_H
#define NEWDEVICEDIALOG_H

#include <QDialog>

namespace Ui
{
class NewDeviceDialog;
}

class NewDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDeviceDialog(QWidget *parent = 0);
    ~NewDeviceDialog();

signals:
    void createDeviceOk(int deviceId);

private:
    Ui::NewDeviceDialog *ui;
};

#endif // NEWDEVICEDIALOG_H
