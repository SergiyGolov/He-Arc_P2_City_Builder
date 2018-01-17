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
    QString file="";
    if(argc==2){
        file=argv[1];
    }else if(argc>2){
        int mapSize=atoi(argv[1]);
        int difficulty=atoi(argv[2]);
        int seed=atoi(argv[3]);
        QString name=argv[4];

        MapView::getMapView()->setNbTiles(mapSize);
        GameManagementService::getGameManagementService()->setCityName(name);
        GameManagementService::getGameManagementService()->setDifficulty(difficulty);
        RandomService::setSeed(seed);
    }

    GraphicService::getGraphicService(file);


    return a.exec();
}
