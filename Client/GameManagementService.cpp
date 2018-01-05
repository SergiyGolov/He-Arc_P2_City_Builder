#include "GameManagementService.h"
#include "ConstantBuilding.h"
#include "TickService.h"
#include "TopView.h"
#include <QDebug>
#include "GuiView.h"
#include "LoadSaveService.h"
#include "AudioService.h"

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
    this->money=1000;
    this->happiness=100;
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
