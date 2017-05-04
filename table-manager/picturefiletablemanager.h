#ifndef PICTUREFILETABLEMANAGER_H
#define PICTUREFILETABLEMANAGER_H

#include <QObject>
class QSqlTableModel;

class PictureFileTableManager
{
protected:
    PictureFileTableManager(QObject *parent = 0);

public:
    static PictureFileTableManager *pictureFileTableManager();
    QSqlTableModel *model()
    {
        return _model;
    }

private:
    static PictureFileTableManager *_pictureFileTableManager;
    QSqlTableModel *_model;
};

#endif // PICTUREFILETABLEMANAGER_H
