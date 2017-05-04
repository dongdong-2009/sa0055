#include "soundfiletablemanager.h"
#include "logutil.h"
#include <QSqlError>
#include <QSqlTableModel>

SoundFileTableManager *SoundFileTableManager::_soundFileTableManager = 0;

SoundFileTableManager *SoundFileTableManager::soundFileTableManager()
{
    if (!_soundFileTableManager)
    {
        _soundFileTableManager = new SoundFileTableManager();
    }

    return _soundFileTableManager;
}

SoundFileTableManager::SoundFileTableManager(QObject *parent)
{
    _model = new QSqlTableModel(parent);
    _model->setTable("config_sound_file");

    if (!_model->select())
    {
        ERROR << QString("call %1 fail:%2").arg("select").arg(model()->lastError().text());
    }

    while (_model->canFetchMore())
    {
        _model->fetchMore();
    }
}
