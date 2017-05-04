#include "picturefiletablemanager.h"
#include "logutil.h"
#include <QSqlError>
#include <QSqlTableModel>

PictureFileTableManager *PictureFileTableManager::_pictureFileTableManager = 0;

PictureFileTableManager *PictureFileTableManager::pictureFileTableManager()
{
    if (!_pictureFileTableManager)
    {
        _pictureFileTableManager = new PictureFileTableManager();
    }

    return _pictureFileTableManager;
}

PictureFileTableManager::PictureFileTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable("config_picture_file");

    if (!_model->select())
    {
        ERROR << QString("call %1 fail:%2").arg("select").arg(model()->lastError().text());
    }

    while (_model->canFetchMore())
    {
        _model->fetchMore();
    }
}
