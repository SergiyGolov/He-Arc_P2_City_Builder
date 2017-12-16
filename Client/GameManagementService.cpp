#include "GameManagementService.h"
#include "ConstantBuilding.h"
#include "TopView.h"
#include <QDebug>
#include "GuiView.h"

GameManagementService::GameManagementService(){
    this->money=1000;
    this->happiness=0;
}

GameManagementService::~GameManagementService(){


}

GameManagementService* GameManagementService::gameManagementServiceInstance=nullptr;

void  GameManagementService::addRoad(int x, int y){
}

void  GameManagementService::addBuilding(int buildingId,int x,int y){
    this->money-=(int)ConstantBuilding::get(buildingId).getPrice();
    qDebug()<<(int)ConstantBuilding::get(buildingId).getPrice();
    TopView::getTopView()->setMoney(money);
    GuiView::getGuiView()->showBuildingPickerMenu(ConstantBuilding::get(buildingId).getCategory()-1);
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
