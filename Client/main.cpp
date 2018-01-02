#include <QDebug>
#include "ConstantBuilding.h"
//#include "LauncherService.h"
#include "LoadSaveService.h"
#include "TickService.h"
#include "GameManagementService.h"
#include "GraphicService.h"
#include <QApplication>
#include "OursMaths.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GraphicService::getGraphicService();

    return a.exec();
}
