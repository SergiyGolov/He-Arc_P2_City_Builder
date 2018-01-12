#include "Launcher.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Launcher w;
    w.showFullScreen();

    return a.exec();
}
