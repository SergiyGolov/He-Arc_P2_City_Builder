#include "GameManagementService.h"
#include "ConstantBuilding.h"
#include "TickService.h"
#include "TopView.h"
#include <QDebug>
#include "GuiView.h"
#include "LoadSaveService.h"

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
    //LoadSaveService::saveGame(QString("C:/test.cbsave"));
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

void GameManagementService::setTotalPopulation(int newTotalPopulation)
{
    this->totalPopulation=newTotalPopulation;
    TopView::getTopView()->setPopulation(totalPopulation);
}

void GameManagementService::setTaxes(double newTaxes)
{
    this->taxes=newTaxes;
}
