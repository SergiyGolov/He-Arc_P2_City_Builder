#ifndef GAMEMANAGEMENTSERVICE_H
#define GAMEMANAGEMENTSERVICE_H

#include <GameService.h>



class ConstantBuilding;


class GameManagementService: public  GameService
{


private:
      //constructor
    GameManagementService();
     int money;
     int totalPopulation;
     int happiness;
     ConstantBuilding *constantBuildingList;

    static GameManagementService* gameManagementServiceInstance;

public:


    //destructor
    ~GameManagementService();
    //methods
     void exitGame();
     void getValueFromConstantWithFormula();
    //getters
     int getMoney(){return money;}
     int getTotalPopulation(){return totalPopulation;}
     int getHappiness(){return happiness;}
     Building* getBuildingDataFromList(int id);
     ConstantBuilding* getConstantBuildingList(){return constantBuildingList;}
     static GameManagementService* getGameManagementService();



     virtual  void addRoad(int x, int y);
     virtual   void addBuilding(int buildingId,int x,int y);
     virtual   void removeRoad(int x,int y);
     virtual   void removeBuilding(int id);


     virtual  QList<Building>* getBuildingList();
      virtual QList<QPoint>* getRoadList();




};
#endif // GAMEMANAGEMENTSERVICE_H
