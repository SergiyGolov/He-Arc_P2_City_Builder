#include "TickService.h"
#include "TopView.h"
#include "GuiView.h"
#include "GameManagementService.h"
#include "BuildingManagementService.h"
#include "OursMaths.h"

TickService* TickService::tickService = nullptr;

TickService* TickService::getTickService()
{
    if(tickService == nullptr)
        tickService = new TickService();
    return tickService;
}

//constructors
TickService::TickService()
{
    oldPopulation = 0;
    oldHappiness = 100;
    oldMoney = 0;
    newPopulation = 0;
    newHappiness = 100;
    newMoney = 0;
    this->start(1000);
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
    happiness = (happiness * 20 + BuildingManagementService::getBuildingManagementService()->getAverageHappiness() * 80) / 100;

    double taxesratio = GameManagementService::getGameManagementService()->getTaxes();

    double taxelimit = 8;

    taxesratio -= taxelimit; //Taxe limite
    taxesratio *= -1; //Inverse the ratio
    if(taxesratio >= 0)
    {
        taxesratio = OursMaths::map(taxesratio, 0, taxelimit, 100, 200);
    }
    else
    {
        taxesratio = OursMaths::map(taxesratio, 0, -taxelimit, 100, 0);
    }

    if(happiness != 0)
        happiness *= taxesratio / 100.0;

    //Barriere of happiness by average
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
    diff += BuildingManagementService::getBuildingManagementService()->getSumPopulation()*(newHappiness/100.0) * 1.5 * GameManagementService::getGameManagementService()->getTaxes();

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

    TopView::getTopView()->update();

    GuiView::getGuiView()->showBuildingPickerMenu(GuiView::getGuiView()->getCurrentTabId()); // to update building that we can afford (if they are too expensive their names become red)
}
