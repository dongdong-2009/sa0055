#include "infotableimportexport.h"
#include "iec104infotablemanager.h"
#include "logutil.h"
#include "maptablemanager.h"
#include <QFile>
#include <QTextStream>

enum CSVColumn
{
    DiDesc,
    AiDesc,
    AiMul,
    AiOffset,
    DoDesc,
    DoQual,
    DoType,
    AoDesc,
    CiDesc,
    MaxColumn
};

const QString csvHeader[] = {"遥信(DI)描述",
                             "遥测(AI)描述",
                             "遥测(AI)乘法因子",
                             "遥测(AI)偏移量",
                             "遥控(DO)描述",
                             "遥控(DO)限定词：0-未指定；1-短脉冲；2-长脉冲；3-持续",
                             "遥控(DO)类型：0-选择执行；1-立即执行",
                             "遥调(AO)描述",
                             "累计量(CI)描述"};

bool InfoTableImportExport::exportInfoTable(const QMap<DataType, QSqlTableModel *> map, const QString fileName)
{
    QSqlTableModel *diModel = map.value(DataType::DI, 0);
    QSqlTableModel *aiModel = map.value(DataType::AI, 0);
    QSqlTableModel *doModel = map.value(DataType::DO, 0);
    QSqlTableModel *aoModel = map.value(DataType::AO, 0);
    QSqlTableModel *ciModel = map.value(DataType::CI, 0);

    Q_ASSERT(diModel && aiModel && doModel && aoModel && ciModel);

    QList<int> rowCountList;
    rowCountList << diModel->rowCount() << aiModel->rowCount() << doModel->rowCount() << aoModel->rowCount() << ciModel->rowCount();
    qSort(rowCountList);
    int maxNum = rowCountList.last();

    QFile file(fileName);
    if (!file.open(QIODevice::OpenModeFlag::WriteOnly | QIODevice::OpenModeFlag::Text))
    {
        ERROR << QString("open csv file(%1) fail: %2").arg(fileName).arg(file.errorString());
        return false;
    }

    QTextStream out(&file);
    for (uint i = 0; i < CSVColumn::MaxColumn; i++)
    {
        out << csvHeader[i];
        if (i < CSVColumn::MaxColumn - 1)
        {
            out << ',';
        }
        else
        {
            out << '\n';
        }
    }

    bool ok;
    for (int i = 0; i < maxNum; i++)
    {
        if (i < diModel->rowCount())
        {
            int pointDescId = diModel->index(i, Iec104InfoTableManager::ColumnIndex::DescrId).data().toInt(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            QString pointDesc = MapTableManager::mapTableManager(MapTableManager::TableIndex::DI)->nameFromId(pointDescId);
            out << pointDesc << ',';
        }
        else
        {
            out << ',';
        }

        if (i < aiModel->rowCount())
        {
            int pointDescId = aiModel->index(i, Iec104InfoTableManager::ColumnIndex::DescrId).data().toInt(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            QString pointDesc = MapTableManager::mapTableManager(MapTableManager::TableIndex::AI)->nameFromId(pointDescId);
            double multipleFactor = aiModel->index(i, Iec104InfoTableManager::ColumnIndex::AI_Multiple).data().toDouble(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toDouble");
            }

            double offsetFactor = aiModel->index(i, Iec104InfoTableManager::ColumnIndex::AI_Offset).data().toDouble(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toDouble");
            }

            out << pointDesc << ',' << multipleFactor << ',' << offsetFactor << ',';
        }
        else
        {
            out << ',' << ',' << ',';
        }

        if (i < doModel->rowCount())
        {
            int pointDescId = doModel->index(i, Iec104InfoTableManager::ColumnIndex::DescrId).data().toInt(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }
            QString pointDesc = MapTableManager::mapTableManager(MapTableManager::TableIndex::DO)->nameFromId(pointDescId);
            int doQual = doModel->index(i, Iec104InfoTableManager::ColumnIndex::DO_Qualifier).data().toInt(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            int doType = doModel->index(i, Iec104InfoTableManager::ColumnIndex::DO_Type).data().toInt(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            out << pointDesc << ',' << doQual << ',' << doType << ',';
        }
        else
        {
            out << ',' << ',' << ',';
        }

        if (i < aoModel->rowCount())
        {
            int pointDescId = aoModel->index(i, Iec104InfoTableManager::ColumnIndex::DescrId).data().toInt(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            QString pointDesc = MapTableManager::mapTableManager(MapTableManager::TableIndex::AO)->nameFromId(pointDescId);
            out << pointDesc << ',';
        }
        else
        {
            out << ',';
        }

        if (i < ciModel->rowCount())
        {
            int pointDescId = ciModel->index(i, Iec104InfoTableManager::ColumnIndex::DescrId).data().toInt(&ok);
            if (!ok)
            {
                ERROR << QString("call %1 fail").arg("toInt");
            }

            QString pointDesc = MapTableManager::mapTableManager(MapTableManager::TableIndex::CI)->nameFromId(pointDescId);
            out << pointDesc << '\n';
        }
        else
        {
            out << '\n';
        }
    }

    return true;
}

QMap<DataType, QList<InfoTableImportExport::PointInfo>> InfoTableImportExport::importInfoTable(QString fileName, bool *success)
{
    QMap<DataType, QList<InfoTableImportExport::PointInfo>> map;

    QList<InfoTableImportExport::PointInfo> diPointList;
    QList<InfoTableImportExport::PointInfo> aiPointList;
    QList<InfoTableImportExport::PointInfo> doPointList;
    QList<InfoTableImportExport::PointInfo> aoPointList;
    QList<InfoTableImportExport::PointInfo> ciPointList;

    bool diReadFinished = false;
    bool aiReadFinished = false;
    bool doReadFinished = false;
    bool aoReadFinished = false;
    bool ciReadFinished = false;

    *success = true;

    QFile file(fileName);
    if (!file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
    {
        ERROR << QString("open file(%1) fail: %2").arg(fileName).arg(file.errorString());
        *success = false;
        return map;
    }

    bool ok;
    int lineNum = 0;
    QString lineStr;
    QTextStream out(&file);
    while (static_cast<void>(lineStr = out.readLine()), static_cast<void>(lineNum++), !lineStr.isNull())
    {
        if (lineNum == 1)
        {
            continue;
        }

        QStringList list = lineStr.split(',');

        if (list.size() != CSVColumn::MaxColumn)
        {
            ERROR << QString("csv file (%1) format invalid，line:%2").arg(fileName).arg(lineNum);
            *success = false;
            break;
        }

        PointInfo diPointInfo;
        PointInfo aiPointInfo;
        PointInfo doPointInfo;
        PointInfo aoPointInfo;
        PointInfo ciPointInfo;

        for (int col = 0; col < CSVColumn::MaxColumn; col++)
        {
            QString data = list.at(col);
            if (!data.isEmpty())
            {
                switch (col)
                {
                case CSVColumn::DiDesc:
                    diPointInfo.setPointDesc(data);
                    break;

                case CSVColumn::AiDesc:
                    aiPointInfo.setPointDesc(data);
                    break;

                case CSVColumn::AiMul:

                    if (aiPointInfo.isValid())
                    {
                        double tempVal1 = data.toDouble(&ok);
                        if (!ok)
                        {
                            ERROR << QString("CSV file:%1,row:%2,column:%3 format error，call %4 fail").arg(fileName).arg(lineNum).arg(col + 1).arg("toDouble");
                        }
                        else
                        {
                            aiPointInfo.setAiMultipleFactor(tempVal1);
                        }
                    }
                    break;

                case CSVColumn::AiOffset:
                    if (aiPointInfo.isValid())
                    {
                        double tempVal2 = data.toDouble(&ok);
                        if (!ok)
                        {
                            ERROR << QString("CSV file:%1,row:%2,column:%3 format error，call %4 fail").arg(fileName).arg(lineNum).arg(col + 1).arg("toDouble");
                        }
                        else
                        {
                            aiPointInfo.setAiOffsetFactor(tempVal2);
                        }
                    }
                    break;

                case CSVColumn::DoDesc:
                    doPointInfo.setPointDesc(data);
                    break;

                case CSVColumn::DoQual:

                    if (doPointInfo.isValid())
                    {
                        int tempVal3 = data.toInt(&ok);
                        if (!ok)
                        {
                            ERROR << QString("CSV file:%1,row:%2,column:%3 format error，call %4 fail").arg(fileName).arg(lineNum).arg(col + 1).arg("toInt");
                        }
                        else
                        {
                            doPointInfo.setDoQual(tempVal3);
                        }
                    }
                    break;

                case CSVColumn::DoType:
                    if (doPointInfo.isValid())
                    {
                        int tempVal4 = data.toInt(&ok);
                        if (!ok)
                        {
                            ERROR << QString("CSV file:%1,row:%2,column:%3 format error，call %4 fail").arg(fileName).arg(lineNum).arg(col + 1).arg("toInt");
                        }
                        else
                        {
                            doPointInfo.setDoType(tempVal4);
                        }
                    }
                    break;

                case CSVColumn::AoDesc:
                    aoPointInfo.setPointDesc(data);
                    break;

                case CSVColumn::CiDesc:
                    ciPointInfo.setPointDesc(data);
                    break;
                }
            }
        }

        if (diPointInfo.isValid() && !diReadFinished)
        {
            diPointList.append(diPointInfo);
        }
        else
        {
            diReadFinished = true;
        }

        if (aiPointInfo.isValid() && !aiReadFinished)
        {
            aiPointList.append(aiPointInfo);
        }
        else
        {
            aiReadFinished = true;
        }

        if (doPointInfo.isValid() && !doReadFinished)
        {
            doPointList.append(doPointInfo);
        }
        else
        {
            doReadFinished = true;
        }

        if (aoPointInfo.isValid() && !aoReadFinished)
        {
            aoPointList.append(aoPointInfo);
        }
        else
        {
            aoReadFinished = true;
        }

        if (ciPointInfo.isValid() && !ciReadFinished)
        {
            ciPointList.append(ciPointInfo);
        }
        else
        {
            ciReadFinished = true;
        }
    }

    if (out.status() == QTextStream::Status::ReadCorruptData)
    {
        *success = false;
    }

    map.insert(DataType::DI, diPointList);
    map.insert(DataType::AI, aiPointList);
    map.insert(DataType::DO, doPointList);
    map.insert(DataType::AO, aoPointList);
    map.insert(DataType::CI, ciPointList);

    return map;
}

QDebug &operator<<(QDebug &out, InfoTableImportExport::PointInfo const &p)
{
    out << QString("desc:%1,aiMul:%2,aiOffset:%3,doQual:%4,doType:%5")
               .arg(p.getPointDesc())
               .arg(p.getAiMultipleFactor())
               .arg(p.getAiOffsetFactor())
               .arg(p.getDoQual())
               .arg(p.getDoType());
    return out;
}
