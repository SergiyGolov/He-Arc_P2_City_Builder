#ifndef GAMESERVICE_H
#define GAMESERVICE_H
#include <QList>

class QPoint;
class Building;

class GameService
{
protected:
    // GameService();
private:

    //~GameService();
    QList<Building> *buildingList; //la liste est la même
    QList<QPoint> *roadList;

public:
    //methods
    virtual  void addRoad(int x, int y)=0;
    virtual   void addBuilding(int buildingId,int x,int y)=0;
    virtual   void removeRoad(int x,int y)=0;
    virtual   void removeBuilding(int id)=0;

    //getters

    virtual  QList<Building>* getBuildingList()=0;
    virtual QList<QPoint>* getRoadList()=0;


};

#endif // GAMESERVICE_H
