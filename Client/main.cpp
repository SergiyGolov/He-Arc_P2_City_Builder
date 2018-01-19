#include <QApplication>

#include "Client/LoadSaveService.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoadSaveService::loadGame(argc, argv);
    return a.exec();
}
