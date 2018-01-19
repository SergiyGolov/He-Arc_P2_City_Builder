#include <QApplication>

#include "Launcher.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Launcher l;
    l.show();
    return a.exec();
}
