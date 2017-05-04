#ifndef ABSTRACTTABLEMANAGER_H
#define ABSTRACTTABLEMANAGER_H

class QSqlTableModel;

class AbstractTableManager
{
public:
    AbstractTableManager();

private:
    QSqlTableModel *_model;
};

#endif // ABSTRACTTABLEMANAGER_H
