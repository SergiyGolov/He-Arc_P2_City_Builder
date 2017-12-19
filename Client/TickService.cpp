#include "TickService.h"
#include "GuiView.h"
#include "GameManagementService.h"
#include "BuildingManagementService.h"

//constructors
TickService::TickService(int tickrate)
{
    oldPopulation = 0;
    oldHappiness = 100;
    oldMoney = 0;
    newPopulation = 0;
    newHappiness = 100;
    newMoney = 0;
    this->start(tickrate);
}

//destructors
TickService::~TickService()
{
}

//methods
void TickService::updatePopulation()
{
    int population = BuildingManagementService::getBuildingManagementService()->getSumPopulation();

    newPopulation = population;
    GameManagementService::getGameManagementService()->setTotalPopulation(population);
}

void TickService::updateHappiness()
{
    double happiness = GameManagementService::getGameManagementService()->getHappiness();

    //Smoothing of the happiness
    happiness = (happiness * 40 + BuildingManagementService::getBuildingManagementService()->getAverageHappiness() * 60) / 100;

    //Barriere of happiness
    happiness = (happiness * 50 + 100 * 50) / 100;

    newHappiness = happiness;
    GameManagementService::getGameManagementService()->setHappiness(happiness);
}

void TickService::updateMoney()
{
    double money = GameManagementService::getGameManagementService()->getMoney();
    double diff = 0;

    //Default 1$/s
    diff += 1.0;

    //Building price per seconds
    diff -= BuildingManagementService::getBuildingManagementService()->getSumPricePerSeconds();

    //Taxes fonds
    diff += BuildingManagementService::getBuildingManagementService()->getSumPopulation()*1.0 * 1.5 * GameManagementService::getGameManagementService()->getTaxes();

    //Happiness scale
    //diff *= newHappiness / 100.0;


    money += diff;
    newMoney = money;
    GameManagementService::getGameManagementService()->setMoney(money);
}

//methods
void TickService::timerEvent(QTimerEvent *tevent)
{
    oldPopulation = newPopulation;
    oldHappiness = newHappiness;
    oldMoney = newMoney;

    //updatePopulation();
    updateHappiness();
    updateMoney();

    qDebug() << "m" << getMoneyDelta() << " h" << getHappinessDelta() << " p" << getPopulationDelta();


    GuiView::getGuiView()->showBuildingPickerMenu(GuiView::getGuiView()->getCurrentTabId()); // to update building that we can afford (if they are too expensive their names become red)
}
