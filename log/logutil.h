#ifndef LOGUTIL_H
#define LOGUTIL_H

#include <QString>
#include <QtDebug>

class LogUtil
{
public:
    static void initialize();

private:
    static QString previousLogFileName;
    static QString currentLogFileName;
    static QString logDir;
    static int logFileMaxSize;
    static QStringList msgTypeList;
    static void messageHandler(QtMsgType type, const QMessageLogContext &contex, const QString &msg);
};

#define ERROR qCritical()<<QString("%1[%2]").arg(__FILE__).arg(__LINE__)
#define INFO qInfo()<<QString("%1[%2]").arg(__FILE__).arg(__LINE__)
#define DEBUG qDebug()<<QString("%1[%2]").arg(__FILE__).arg(__LINE__)

extern QString SqlErrorTypeStr[];

#endif // LOGUTIL_H
