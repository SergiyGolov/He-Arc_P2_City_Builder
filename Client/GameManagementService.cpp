#include "GameManagementService.h"

GameManagementService::GameManagementService():GameService(){

}

GameManagementService::~GameManagementService(){


}

GameManagementService* GameManagementService::gameManagementServiceInstance=nullptr;

void  GameManagementService::addRoad(int x, int y){
}

void  GameManagementService::addBuilding(int buildingId,int x,int y){
}

void  GameManagementService::removeRoad(int x,int y){
}

void  GameManagementService::removeBuilding(int id){

}

QList<Building>* GameManagementService::getBuildingList()
{

}
QList<QPoint>* GameManagementService::getRoadList(){

}

void GameManagementService::exitGame(){

}

void GameManagementService::getValueFromConstantWithFormula(){

}


GameManagementService* GameManagementService::getGameManagementService(){
    if(gameManagementServiceInstance==nullptr){
        gameManagementServiceInstance=new GameManagementService();
    }
    return gameManagementServiceInstance;


}
