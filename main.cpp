#include "logutil.h"
#include "dbtools.h"
#include "mainwindow.h"
#include "maptablemanager.h"
#include <QApplication>
#include <QStyleFactory>
#include <QSplashScreen>
#include <QThread>

int main(int argc, char *argv[])
{
    // QApplication::setStyle("Fusion");
    QApplication app(argc, argv);

    QPixmap pixmap(":/icon/icons/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();

    //app.processEvents();

    splash.showMessage("initialize log");
#ifdef QT_NO_DEBUG
    LogUtil::initialize();
#endif

    INFO << QString("SA0055-0(V100) start running!");

    splash.showMessage("init local database");
    //app.processEvents();

    DbTools::initMemoryDbSchama();
    if(DbTools::dumpFileToMemory())
    {
        MapTableManager::CopyModel2Map();
    }

    MainWindow window;
    window.show();
    // w.showFullScreen();

    splash.finish(&window);

    return app.exec();
}
