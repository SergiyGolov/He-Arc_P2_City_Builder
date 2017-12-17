#include "TickService.h"
#include "GuiView.h"
#include "GameManagementService.h"
#include "BuildingManagementService.h"

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
    double money = GameManagementService::getGameManagementService()->getMoney();
    money += 1.0; //Default 1$/s
    money -= BuildingManagementService::getBuildingManagementService()->getSumPricePerSeconds(); //Building price per seconds
    money += BuildingManagementService::getBuildingManagementService()->getSumPopulation()*1.0 * 1.5 * GameManagementService::getGameManagementService()->getTaxes();

    GameManagementService::getGameManagementService()->setMoney(money);
}

void TickService::updatePopulation()
{
    int population = BuildingManagementService::getBuildingManagementService()->getSumPopulation();

    GameManagementService::getGameManagementService()->setTotalPopulation(population);
}

//methods
void TickService::timerEvent(QTimerEvent *tevent)
{
    updatePopulation();
    updateHappiness();
    updateMoney();
    GuiView::getGuiView()->showBuildingPickerMenu(GuiView::getGuiView()->getCurrentTabId()); // to update building that we can afford (if they are too expensive their names become red)


}
