#include "dbtools.h"
#include "logutil.h"
#include <QMessageBox>
#include <QtSql>

bool DbTools::initMemoryDbSchama()
{
    QSqlDatabase localDb = QSqlDatabase::addDatabase("QSQLITE");
    localDb.setDatabaseName(":memory:");

    if (!localDb.open())
    {
        ERROR << QString("open memory database fail: %1").arg(localDb.lastError().text());
        return false;
    }

    QSqlQuery query;
    QString sqlStr;

    sqlStr = "PRAGMA journal_mode = MEMORY";
    if (!query.prepare(sqlStr))
    {
        ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
    }

    if (!query.exec())
    {
        ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
    }

    QString sqlFileName = QString(":/sql/sqlite/main.sql");
    QFile sqlFile(sqlFileName);
    if (!sqlFile.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
    {
        ERROR << QString("open %1 fail").arg(sqlFileName);
        return false;
    }

    QTextStream out(&sqlFile);
    out.setCodec("UTF-8");

    QStringList sqlStrList = out.readAll().split(';', QString::SplitBehavior::SkipEmptyParts);

    bool ok = true;
    foreach (sqlStr, sqlStrList)
    {
        sqlStr = sqlStr.trimmed();
        if (sqlStr.isEmpty())
        {
            continue;
        }

        if (sqlStr.contains("DROP TABLE"))
        {
            continue;
        }

        if (sqlStr.contains("CREATE TABLE sqlite_"))
        {
            continue;
        }

        if (!query.prepare(sqlStr))
        {
            ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
            ok = false;
        }

        if (!query.exec())
        {
            ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
            ok = false;
        }
        query.finish();
    }

    return ok;
}

bool DbTools::dumpFileToMemory()
{
    QSqlQuery query;
    QString sqlStr;

    QString sqlFileName = QString("data.sql");
    QFile sqlFile(sqlFileName);
    if (!sqlFile.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
    {
        ERROR << QString("open %1 fail").arg(sqlFileName);
        return false;
    }

    QTextStream out(&sqlFile);
    out.setCodec("UTF-8");

    QStringList sqlStrList = out.readAll().split(';', QString::SplitBehavior::SkipEmptyParts);

    bool ok = true;
    foreach (sqlStr, sqlStrList)
    {
        DEBUG << sqlStr;
        sqlStr = sqlStr.trimmed();
        if(sqlStr.isEmpty())
        {
            continue;
        }

        if (!query.prepare(sqlStr))
        {
            ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
            ok = false;
        }

        if (!query.exec())
        {
            ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
            ok = false;
        }
        query.finish();
    }

    return ok;
}

bool DbTools::dumpMemoryToFile()
{
    QSqlQuery query;
    QStringList tables;

    QString sqlStr = "SELECT * FROM sqlite_master";
    if (!query.prepare(sqlStr))
    {
        ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
        return false;
    }

    if (!query.exec())
    {
        ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
        return false;
    }

    while (query.next())
    {
        DEBUG << query.value("sql").toString();
        if (query.value("type").toString() == "table")
        {
            QString tableName = query.value("name").toString();

            if (!tableName.startsWith("sa0055_") && !tableName.startsWith("sqlite_"))
            {
                tables << tableName;
            }
        }
    }

    DEBUG << tables;

    QString sqlFileName = QString("%1/%2").arg(QDir::currentPath()).arg("data.sql");
    QFile sqlFile(sqlFileName);
    if (!sqlFile.open(QIODevice::OpenModeFlag::WriteOnly | QIODevice::OpenModeFlag::Text))
    {
        ERROR << QString("打开%1 fail").arg(sqlFileName);
        return false;
    }

    QTextStream out(&sqlFile);
    out.setCodec("UTF-8");

    static const QString insertSqlStr = QString("INSERT INTO %1 (%2) VALUES (%3);\n");
    QSqlRecord record;
    bool first = true;
    foreach (QString table, tables)
    {
        sqlStr = QString("SELECT * FROM %1").arg(table);
        if (!query.prepare(sqlStr))
        {
            ERROR << QString("call %1(%2) fail: %3").arg("prepare").arg(sqlStr).arg(query.lastError().text());
            return false;
        }

        if (!query.exec())
        {
            ERROR << QString("call %1(%2) fail: %3").arg("exec").arg(sqlStr).arg(query.lastError().text());
            return false;
        }

        first = true;
        QStringList columns;
        while (query.next())
        {
            QStringList values;

            record = query.record();
            for (int i = 0; i < record.count(); i++)
            {
                if (first)
                {
                    columns << record.fieldName(i);
                }

                if (record.value(i).type() == QVariant::Type::String)
                {
                    values << QString("'%1'").arg(record.value(i).toString());
                }
                else
                {
                    values << record.value(i).toString();
                }
            }
            first = false;

            out << insertSqlStr.arg(table).arg(columns.join(", ")).arg(values.join(", "));
        }
    }

    return out.status() != QTextStream::Status::WriteFailed;
}
