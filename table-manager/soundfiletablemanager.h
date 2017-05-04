#ifndef SOUNDFILETABLEMANAGER_H
#define SOUNDFILETABLEMANAGER_H

#include <QObject>
class QSqlTableModel;

class SoundFileTableManager
{
protected:
    SoundFileTableManager(QObject *parent = 0);

public:
    static SoundFileTableManager *soundFileTableManager();
    QSqlTableModel *model()
    {
        return _model;
    }

private:
    static SoundFileTableManager *_soundFileTableManager;
    QSqlTableModel *_model;
};

#endif // SOUNDFILETABLEMANAGER_H
