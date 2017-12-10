#include <QDebug>
#include "ConstantBuilding.h"
#include "LauncherService.h"
#include "LoadSaveService.h"
#include "TickService.h"

int main(int argc, char *argv[])
{
    qDebug() << ConstantBuilding::get(bID::Road).getCategory();

    return 0;
}
