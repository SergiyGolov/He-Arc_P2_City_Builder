#include "TickService.h"

//constructors
TickService::TickService(int tickrate)
{
    this->start(tickrate);
}

//destructors
TickService::~TickService()
{

}

//methods
void TickService::updateHappiness()
{

}

void TickService::updateMoney()
{
    double money = GameManagementService::getGameManagementService()->getMoney();/*
    int population = GameManagementService::getGameManagementService()->getTotalPopulation();

    int nbbuilding = 10;

    double coef = 9.0;
    double average = 3000;
    double multiplierplus = coef * average;

    //double multiplierminus = ;
*/
    //for(int i = 0; i < nbbuilding; i++)
    {
        money += 2;
    }

    GameManagementService::getGameManagementService()->setMoney(money);
}

void TickService::updatePopulation()
{

}

//methods
void TickService::timerEvent(QTimerEvent *tevent)
{
    updateHappiness();
    updateMoney();
    updatePopulation();
}
