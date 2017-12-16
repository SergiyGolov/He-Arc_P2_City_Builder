#ifndef GAMEMANAGEMENTSERVICE_H
#define GAMEMANAGEMENTSERVICE_H

#include <QList>
#include <Building.h>



class ConstantBuilding;
class TickService;


class GameManagementService
{
private:
    //constructor
    static GameManagementService* gameManagementServiceInstance;
    GameManagementService();
    int money;
    int totalPopulation;
    int happiness;
    TickService *ts;
public:
    static GameManagementService* getGameManagementService();
    //destructor
    ~GameManagementService();
    //methods
    void exitGame();
    void addRoad(int x, int y);
    void addBuilding(int buildingId,int x,int y);
    void removeRoad(int x,int y);
    void removeBuilding(int id);
    //getters
    int getMoney(){return money;}
    int getTotalPopulation(){return totalPopulation;}
    int getHappiness(){return happiness;}
    //setters
    void setHappiness(int newHappiness);
    void setMoney(int newMoney);
};
#endif // GAMEMANAGEMENTSERVICE_H
