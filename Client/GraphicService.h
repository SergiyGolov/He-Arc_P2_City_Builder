#ifndef GRAPHICSERVICE_H
#define GRAPHICSERVICE_H

#include "Building.h"
#include "MainWidget.h"
#include <QList>


class GraphicService
{

private:
    //constructor
    GraphicService();
    static GraphicService* graphicServiceInstance;
    MainWidget *mainWidget;


public:
    //destructor
    ~GraphicService();
    //methods
    void showMap();
    void showBuildingPickerMenu(int tabId);
    void toggleFilter(int filterId);
    static GraphicService* getGraphicService();


     void addRoad(int x, int y);
     void addBuilding(int buildingId,int x,int y);
     void removeRoad(int x,int y);
     void removeBuilding(int id);

    //getters

     QList<Building>* getBuildingList();
     QList<QPoint>* getRoadList();



};
#endif // GRAPHICSERVICE_H
