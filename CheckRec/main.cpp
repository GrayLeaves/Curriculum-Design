#include "windows/mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSplashScreen *splash = new QSplashScreen;

    splash->setPixmap(QPixmap( ":/images/startqt.png"));
    splash->show();
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->showMessage(QObject::tr("Setting up the MainWindow..."),topRight,Qt::white);
    MainWindow w;
    splash->showMessage(QObject::tr("Loading moudles and Establishing connections...")
                            ,topRight,Qt::white);
    //loadMoudles(); establishConnections();
    w.show();
    splash->finish(&w);
    delete splash;
    return app.exec();
}
