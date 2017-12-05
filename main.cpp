#include <QDebug>
#include "ConstantBuilding.h"
#include "LauncherService.h"
#include "LoadSaveService.h"
#include "TickService.h"

int main(int argc, char *argv[])
{
    ConstantBuilding* a = ConstantBuilding::getBuildingArray();
    qDebug() << a[bID::HouseAverageDensityMiddleClass].getEfficiancy() << " ";
    qDebug() << a[bID::Barn].getEfficiancy() << " ";
    qDebug() << a[bID::AmusementPark].getEfficiancy() << " ";
    return 0;
}
