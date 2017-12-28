#include "TickService.h"
#include "TopView.h"
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
int TickService::updatePopulation()
{
    int population = BuildingManagementService::getBuildingManagementService()->getSumPopulation();


    return population;
}

double TickService::updateHappiness()
{
    double happiness = GameManagementService::getGameManagementService()->getHappiness();

    //Smoothing of the happiness
    happiness = (happiness * 40 + BuildingManagementService::getBuildingManagementService()->getAverageHappiness() * 60) / 100;

    //Barriere of happiness
    happiness = (happiness * 50 + 100 * 50) / 100;

    return happiness;
}

double TickService::updateMoney()
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

    return money;
}

//methods
void TickService::timerEvent(QTimerEvent *tevent)
{
    oldPopulation = newPopulation;
    oldHappiness = newHappiness;
    oldMoney = newMoney;

    newPopulation = updatePopulation();
    newHappiness = updateHappiness();
    newMoney = updateMoney();

    GameManagementService::getGameManagementService()->setTotalPopulation(newPopulation);
    GameManagementService::getGameManagementService()->setHappiness(newHappiness);
    GameManagementService::getGameManagementService()->setMoney(newMoney);

    TopView::getTopView()->setPopulationDelta(getPopulationDelta());
    TopView::getTopView()->setHappinessDelta(getHappinessDelta());
    TopView::getTopView()->setMoneyDelta(getMoneyDelta());

    GuiView::getGuiView()->showBuildingPickerMenu(GuiView::getGuiView()->getCurrentTabId()); // to update building that we can afford (if they are too expensive their names become red)
}
