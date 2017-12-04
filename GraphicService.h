#ifndef GRAPHICSERVICE_H
#define GRAPHICSERVICE_H

#include <GameService.h>

class GraphicService: public  GameService
{

private:
    GraphicService();
    static GraphicService* graphicServiceInstance;

public:
    //constructor

    //destructor
    ~GraphicService();
    //methods
    void showMap();
    void showBuildingPickerMenu(int tabId);
    void toggleFilter(int filterId);
    static GraphicService* getGraphicService();


    virtual  void addRoad(int x, int y);
    virtual   void addBuilding(int buildingId,int x,int y);
    virtual   void removeRoad(int x,int y);
    virtual   void removeBuilding(int id);

    //getters

    virtual  QList<Building>* getBuildingList();
    virtual QList<QPoint>* getRoadList();



};
#endif // GRAPHICSERVICE_H
