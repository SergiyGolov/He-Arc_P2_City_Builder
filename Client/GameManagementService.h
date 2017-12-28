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
    double taxes;
    TickService *ts;
public:
    static GameManagementService* getGameManagementService();
    //destructor
    ~GameManagementService();
    //methods
    void exitGame();
    //getters
    int getMoney(){return money;}
    int getHappiness(){return happiness;}
    int getTotalPopulation(){return totalPopulation;}
    int getTaxes(){return taxes;}
    //setters
    void setHappiness(int newHappiness);
    void setMoney(int newMoney);
    void setTotalPopulation(int newTotalPopulation);
    void setTaxes(double newTaxes);
};
#endif // GAMEMANAGEMENTSERVICE_H
