#include "GameManagementService.h"
#include "ConstantBuilding.h"
#include "TickService.h"
#include "TopView.h"
#include <QDebug>
#include "GuiView.h"

GameManagementService* GameManagementService::gameManagementServiceInstance=nullptr;

GameManagementService* GameManagementService::getGameManagementService()
{
    if(gameManagementServiceInstance==nullptr)
        gameManagementServiceInstance=new GameManagementService();
    return gameManagementServiceInstance;
}

GameManagementService::GameManagementService()
{
    this->money=1000;
    this->happiness=0;
    ts = new TickService(1000);
}

GameManagementService::~GameManagementService()
{


}

//setters
void GameManagementService::setHappiness(int newHappiness)
{
    this->happiness=newHappiness;
    TopView::getTopView()->setHappiness(happiness);
}

void GameManagementService::setMoney(int newMoney)
{
    this->money=newMoney;
    TopView::getTopView()->setMoney(money);
}

//methods
void GameManagementService::addBuilding(int buildingId,int x,int y)
{
    setMoney(getMoney()-(int)ConstantBuilding::get(buildingId).getPrice());
    qDebug()<<(int)ConstantBuilding::get(buildingId).getPrice();
    GuiView::getGuiView()->showBuildingPickerMenu(ConstantBuilding::get(buildingId).getCategory()-1);
}

void GameManagementService::addRoad(int x, int y)
{

}

void GameManagementService::removeRoad(int x, int y)
{

}

void GameManagementService::removeBuilding(int id)
{

}
