
#include "GraphicService.h"



GraphicService::GraphicService():GameService(){

}

GraphicService::~GraphicService(){

}

GraphicService* GraphicService::graphicServiceInstance=nullptr;

void  GraphicService::addRoad(int x, int y){
}

void  GraphicService::addBuilding(int buildingId,int x,int y){
}

void  GraphicService::removeRoad(int x,int y){
}

void  GraphicService::removeBuilding(int id){

}

QList<Building>* GraphicService::getBuildingList()
{

}
QList<QPoint>* GraphicService::getRoadList(){

}


void GraphicService::showMap(){

}

void GraphicService::showBuildingPickerMenu(int tabId){

}

void GraphicService::toggleFilter(int filterId){

}

GraphicService* GraphicService::getGraphicService(){
    if(GraphicService::graphicServiceInstance==nullptr){
        GraphicService::graphicServiceInstance=new GraphicService();
    }

    return GraphicService::graphicServiceInstance;

}
