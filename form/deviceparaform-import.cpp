#include "deviceparaform.h"
#include "datatype.h"
#include "infotableimportexport.h"
#include "maptablemanager.h"
#include "util.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSqlError>

void DeviceParaForm::exportInfoRecord()
{
    QString deviceName = MapTableManager::mapTableManager(MapTableManager::DEV)->nameFromId(deviceId);
    QString fileName = QString("%1/%2_四遥信息表.csv").arg(QDir::homePath()).arg(deviceName);

    QString csvFileName = QFileDialog::getSaveFileName(this, "导出信息表", fileName, "CSV(逗号分隔) (*.csv)");
    if (csvFileName.isEmpty())
    {
        return;
    }

    QMap<DataType, QSqlTableModel *> map;
    for (int i = DataType::DI; i < DataType::DataTypeNum; i++)
    {
        DataType dataType = Util::integer_2_dataType(i);
        map.insert(dataType, Iec104InfoTableManager::infoTableManager(dataType)->model());
    }

    if (!InfoTableImportExport::exportInfoTable(map, csvFileName))
    {
        QMessageBox::warning(this, "提示", "导出信息表失败!");
    }
    else
    {
        QMessageBox::information(this, "提示", "导出信息表成功!");
    }
}

bool DeviceParaForm::_processImportRecord(DataType dataType, QList<InfoTableImportExport::PointInfo> pointList)
{
    MapTableManager::TableIndex tableIndex = Util::dataType_2_mapTableIndex(dataType);
    QList<QString> importDescList;

    foreach (InfoTableImportExport::PointInfo p, pointList)
    {
        importDescList << p.getPointDesc();
    }

    QSet<QString> importDescSet = QSet<QString>::fromList(importDescList);

    /*
     * 检查导入的CSV文件是否有相同的描述
     */
    if (importDescSet.size() < importDescList.size())
    {
        QMessageBox::warning(this, "提示", QString("csv文件包含相同的%1描述!").arg(DataTypeStr[dataType]));
        return false;
    }

    QList<QString> existDescList;

    bool ok;
    QSqlTableModel *model = Iec104InfoTableManager::infoTableManager(dataType)->model();
    for (int row = 0; row < model->rowCount(); row++)
    {
        int descId = model->index(row, Iec104InfoTableManager::ColumnIndex::DescrId).data().toInt(&ok);
        if (!ok)
        {
            ERROR << QString("call %1 fail!").arg("toInt");
        }

        QString desc = MapTableManager::mapTableManager(tableIndex)->nameFromId(descId);
        existDescList << desc;
    }

    int minRecordNum = qMin(importDescList.size(), existDescList.size());
    bool mismatch = minRecordNum > 0 ? importDescList.mid(0, minRecordNum) != existDescList.mid(0, minRecordNum) : false;

    if (mismatch)
    {
        if (QMessageBox::question(this, "提示",
                                  QString("CSV文件中%1描述与配置表中记录不匹配! 是否清空配置表中记录,然后导入?").arg(DataTypeStr[dataType])) ==
            QMessageBox::StandardButton::No)
        {
            return false;
        }
        else
        {
            _clearInfoRecord(dataType);
            existDescList.clear();
            minRecordNum = qMin(importDescList.size(), existDescList.size());
        }
    }

    if (importDescList.size() < existDescList.size())
    {
        Iec104InfoTableManager::infoTableManager(dataType)->deleteRecord(deviceId, minRecordNum, model->rowCount() - minRecordNum);
    }
    else if (importDescList.size() > existDescList.size())
    {
        for (int i = minRecordNum; i < importDescList.size(); i++)
        {
            QString importNewDesc = importDescList.at(i);
            int descId = MapTableManager::mapTableManager(tableIndex)->idFromName(importNewDesc);
            if (descId != -1)
            {
                QMessageBox::warning(this, "提示", QString("%1描述(%2)已经被使用，不能导入!").arg(DataTypeStr[dataType]).arg(importNewDesc));
                return false;
            }
        }

        QList<int> descIdList;
        for (int i = minRecordNum; i < importDescList.size(); i++)
        {
            QString importNewDesc = importDescList.at(i);
            int descId = MapTableManager::mapTableManager(tableIndex)->addDesc(importNewDesc);
            Q_ASSERT(descId != -1);
            descIdList.append(descId);
        }

        Iec104InfoTableManager::infoTableManager(dataType)->addRecordBatch(deviceId, descIdList, pointList);
    }


    return true;
}

void DeviceParaForm::importInfoRecord()
{
    static QString defaultInitDir = QDir::homePath();
    QString csvFileName = QFileDialog::getOpenFileName(this, "导入信息表", defaultInitDir, "CSV(逗号分隔) (*.csv)");
    if (csvFileName.isEmpty())
    {
        return;
    }

    QFileInfo fileInfo(csvFileName);
    defaultInitDir = fileInfo.absolutePath();

    bool ok;
    QMap<DataType, QList<InfoTableImportExport::PointInfo>> map = InfoTableImportExport::importInfoTable(csvFileName, &ok);
    if (!ok)
    {
        QMessageBox::warning(this, "提示", QString("csv文件(%1)格式错误!").arg(csvFileName));
        return;
    }

    QList<InfoTableImportExport::PointInfo> diPointList = map.value(DataType::DI);
    QList<InfoTableImportExport::PointInfo> aiPointList = map.value(DataType::AI);
    QList<InfoTableImportExport::PointInfo> doPointList = map.value(DataType::DO);
    QList<InfoTableImportExport::PointInfo> aoPointList = map.value(DataType::AO);
    QList<InfoTableImportExport::PointInfo> ciPointList = map.value(DataType::CI);

    if (!_processImportRecord(DataType::DI, diPointList))
    {
        QMessageBox::warning(this, "提示", QString("从csv文件(%1)导入%2失败!").arg(csvFileName).arg(DataTypeStr[DataType::DI]));
        return;
    }

    if (!_processImportRecord(DataType::AI, aiPointList))
    {
        QMessageBox::warning(this, "提示", QString("从csv文件(%1)导入%2失败!").arg(csvFileName).arg(DataTypeStr[DataType::AI]));
        return;
    }

    if (!_processImportRecord(DataType::DO, doPointList))
    {
        QMessageBox::warning(this, "提示", QString("从csv文件(%1)导入%2失败!").arg(csvFileName).arg(DataTypeStr[DataType::DO]));
        return;
    }

    if (!_processImportRecord(DataType::AO, aoPointList))
    {
        QMessageBox::warning(this, "提示", QString("从csv文件(%1)导入%2失败!").arg(csvFileName).arg(DataTypeStr[DataType::AO]));
        return;
    }

    if (!_processImportRecord(DataType::CI, ciPointList))
    {
        QMessageBox::warning(this, "提示", QString("从csv文件(%1)导入%2失败!").arg(csvFileName).arg(DataTypeStr[DataType::CI]));
        return;
    }
}
