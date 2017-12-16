#ifndef GAMEMANAGEMENTSERVICE_H
#define GAMEMANAGEMENTSERVICE_H

#include <QList>
#include <Building.h>



class ConstantBuilding;


class GameManagementService
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


     //setters
     void setMoney(int newMoney){this->money=newMoney;}
     void setHappiness(int newHappiness){this->money=newHappiness;}


       void addRoad(int x, int y);
        void addBuilding(int buildingId,int x,int y);
        void removeRoad(int x,int y);
        void removeBuilding(int id);


       QList<Building>* getBuildingList();
       QList<QPoint>* getRoadList();




};
#endif // GAMEMANAGEMENTSERVICE_H
