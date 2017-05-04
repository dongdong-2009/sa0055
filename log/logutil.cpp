#include "logutil.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

int LogUtil::logFileMaxSize = 10 * 1024 * 1024; /*10M*/
QString LogUtil::currentLogFileName = "sa0055-0.log";
QString LogUtil::previousLogFileName = "sa0055-1.log";
QString LogUtil::logDir;
QStringList LogUtil::msgTypeList;

QString SqlErrorTypeStr[] = {"NoError", "ConnectionError", "StatementError", "TransactionError", "UnknownError"};

void LogUtil::initialize()
{
    logDir = QCoreApplication::applicationDirPath();
    msgTypeList << "DEBUG"
                << "WARNING"
                << "CRITICAL"
                << "FATAL"
                << "INFO";

    qInstallMessageHandler(messageHandler);
}

void LogUtil::messageHandler(QtMsgType type, const QMessageLogContext &contex, const QString &msg)
{
    QFile file(currentLogFileName);
    if (file.size() > logFileMaxSize)
    {
        file.setFileName(previousLogFileName);
        qSwap(previousLogFileName, currentLogFileName);

        file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    }
    else
    {
        file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    }

    QFileInfo srcFile(contex.file);
    QTextStream out(&file);
    QString logStr = QString("[%1] %2 %3(%4):%5\n")
                         .arg(msgTypeList.at(type))
                         .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                         .arg(srcFile.baseName())
                         .arg(contex.line)
                         .arg(msg);
    out << logStr;
}
