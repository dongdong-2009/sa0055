#ifndef DBTOOLS_H
#define DBTOOLS_H

#include <QSqlDatabase>

class DbTools
{
public:
    static bool initMemoryDbSchama();
    static bool dumpMemoryToFile();
    static bool dumpFileToMemory();
};

#endif // DBTOOLS_H
