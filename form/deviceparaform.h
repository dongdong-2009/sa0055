#ifndef DEVICEPARAFORM_H
#define DEVICEPARAFORM_H

#include "datatype.h"
#include "iec104infotablemanager.h"
#include "infotableimportexport.h"
#include <QWidget>

namespace Ui
{
class DeviceParaForm;
}

class QDataWidgetMapper;
class MapTableManager;
class QSqlTableModel;
class QTableView;
class QComboBox;

class DeviceParaForm : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceParaForm(QWidget *parent = 0);
    ~DeviceParaForm();
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);

    void setCurrentDeviceId(int deviceId);

    void addInfoRecord();
    void deleteInfoRecord();
    void clearInfoRecord();

    QStringList configValidCheck();
private:
    Ui::DeviceParaForm *ui;
    QDataWidgetMapper *deviceParaMapper;
    QDataWidgetMapper *iec104ParaMapper;
    int deviceId;
    QTableView *tableView[DataType::DataTypeNum];
    QComboBox *typeIdComboBox[DataType::DataTypeNum];

    void saveData();
    void refreshData();

    void refreshDevicePara();
    void refreshIec104Para();
    void refreshInfoTable();

    void initInfoTable();
    void initDevicePara();
    void initInfoView(DataType dataType);
    void initIec104Para();

    void createDataWidgetMapper4Iec104();
    void createDataWidgetMapper4Device();

    void saveDevicePara();
    void saveIec104Para();
    void saveInfoTable();

    void exportInfoRecord();
    void importInfoRecord();

    QStringList deviceConfigValidCheck();
    QStringList infoTableConfigValidCheck();
    QStringList iec104ParaConfigValidCheck();
    void _clearInfoRecord(DataType dataType);
    bool _processImportRecord(DataType dataType, QList<InfoTableImportExport::PointInfo> pointList);
    void _setUnusedWidgetDisable();
};

#endif // DEVICEPARAFORM_H
