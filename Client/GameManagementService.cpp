#include "Services.h"
#include "ConstantBuilding.h"
#include "TopView.h"
#include "GuiView.h"

#include <QSound>

GameManagementService* GameManagementService::gameManagementServiceInstance=nullptr;

GameManagementService* GameManagementService::getGameManagementService()
{
    if(gameManagementServiceInstance==nullptr)
        gameManagementServiceInstance=new GameManagementService();
    return gameManagementServiceInstance;
}

GameManagementService::GameManagementService()
{
    saveCount = 0;
    money=100000;
    happiness=100;
    TickService::getTickService();
    AudioService::getAudioService()->playMusic();
}

GameManagementService::~GameManagementService()
{

}

//setters
void GameManagementService::setHappiness(int newHappiness)
{
    this->happiness=newHappiness;
}

void GameManagementService::setMoney(int newMoney)
{
    this->money=newMoney;
}

void GameManagementService::setTotalPopulation(int newTotalPopulation)
{
    this->totalPopulation=newTotalPopulation;
}

void GameManagementService::setTaxes(double newTaxes)
{
    this->taxes=newTaxes;
}
