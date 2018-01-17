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

void GameManagementService::setDifficulty(int difficulty)
{
    this->difficulty = difficulty;
    switch (difficulty)
    {
    case 0:
        this->money = 2000000000;
        break;
    case 1:
        this->money = 100000;
        break;
    case 2:
        this->money = 10000;
        break;
    case 3:
        this->money = 1000;
        break;
    default:
        break;
    }
}
