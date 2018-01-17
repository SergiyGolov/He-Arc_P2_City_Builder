#include <QDebug>
#include "ConstantBuilding.h"
//#include "LauncherService.h"
#include "LoadSaveService.h"
#include "TickService.h"
#include "GameManagementService.h"
#include "GraphicService.h"
#include <QApplication>
#include "OursMaths.h"
#include "RandomService.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    LoadSaveService::loadGame(argc, argv);

    return a.exec();
}
