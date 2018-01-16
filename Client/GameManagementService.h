#ifndef GAMEMANAGEMENTSERVICE_H
#define GAMEMANAGEMENTSERVICE_H

#include <QList>
#include <Building.h>
#include <TopView.h>


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
    QString cityName;
    int saveCount;
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
    int incrementAndGetSaveCount() {return ++saveCount;}
    QString getCityName(){return cityName;}
    //setters
    void setHappiness(int newHappiness);
    void setMoney(int newMoney);
    void setTotalPopulation(int newTotalPopulation);
    void setTaxes(double newTaxes);
    void setCityName(QString cityName) { this->cityName = cityName;
                                      TopView::getTopView()->setCityName();}
    void setSaveCount(int saveCount) { this->saveCount = saveCount; }
};
#endif // GAMEMANAGEMENTSERVICE_H
