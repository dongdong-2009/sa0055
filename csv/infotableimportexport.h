#ifndef INFOTABLEIMPORTEXPORT_H
#define INFOTABLEIMPORTEXPORT_H

#include "datatype.h"
#include <QMap>
#include <QSqlTableModel>

class InfoTableImportExport
{
public:
    class PointInfo
    {
    public:
        PointInfo() : _pointDesc(""), _valid(false), _aiMultipleFactor(1), _aiOffsetFactor(0), _doQual(0), _doType(0)
        {
        }

        QString getPointDesc() const
        {
            return _pointDesc;
        }
        void setPointDesc(const QString &value)
        {
            _pointDesc = value;
            _valid = true;
        }

        bool isValid()
        {
            return _valid;
        }

        double getAiMultipleFactor() const
        {
            return _aiMultipleFactor;
        }
        void setAiMultipleFactor(double value)
        {
            _aiMultipleFactor = value;
        }

        double getAiOffsetFactor() const
        {
            return _aiOffsetFactor;
        }
        void setAiOffsetFactor(double value)
        {
            _aiOffsetFactor = value;
        }

        int getDoQual() const
        {
            return _doQual;
        }
        void setDoQual(int value)
        {
            _doQual = value;
        }

        int getDoType() const
        {
            return _doType;
        }
        void setDoType(int value)
        {
            _doType = value;
        }

    private:
        QString _pointDesc;
        bool _valid;

        double _aiMultipleFactor;
        double _aiOffsetFactor;

        int _doQual;
        int _doType;
    };

    static bool exportInfoTable(const QMap<DataType, QSqlTableModel *> map, const QString fileName);
    static QMap<DataType, QList<PointInfo>> importInfoTable(QString fileName, bool *ok);
};

QDebug &operator<<(QDebug &out, InfoTableImportExport::PointInfo const &point);

#endif // INFOTABLEIMPORTEXPORT_H
