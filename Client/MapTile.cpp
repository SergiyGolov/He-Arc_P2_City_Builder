#include "MapTile.h"
#include <QFont>
#include <QDebug>

MapTile::MapTile(int x, int y, int left, int top, int width, int height,QGraphicsItem* parent):QGraphicsRectItem(left,top,width,height,parent){

    this->x=x;
    this->y=y;

    this->bId=-10;
    uniqueBId=-10;
    mainTileX=-10;
    mainTileY=-10;
    largeurBat=-10;
    hauteurBat=-10;




    this->setZValue(1);
    bPix=false;
    bOccupied=false;
    buildImage=nullptr;
}


MapTile::~MapTile(){

}
QGraphicsScene* MapTile::scene=nullptr;



void MapTile::setScene(QGraphicsScene *scene){
    MapTile::scene=scene;
}


void MapTile::addPixMove(QGraphicsPixmapItem *pix){

        buildImage=pix;
        buildImage->setVisible(true);
        buildImage->setPos(pos().x()-rect().width()/4,pos().y()+rect().height()/2);

}



void MapTile::addPix(QGraphicsPixmapItem *pix){

        buildImage=pix;
        buildImage->setVisible(true);
        buildImage->setPos(pos().x()-rect().width()/4,pos().y()+rect().height()/2);
        buildImage->setOpacity(1);


        bPix=true;

}



void MapTile::addPixRoad(QGraphicsPixmapItem *pix){

    if(buildImage==nullptr){
        // pix->setVisible(true);
        // pix->setPos(pos().x()-rect().width()*3/4,pos().y()+rect().height()/2);
        //pix->setPos(pos().x()+rect().width()/6,pos().y()+rect().height()/6);
        buildImage=pix;
        buildImage->setPos(pos());





        bPix=true;
    }
}

void MapTile::removePixMove(){
    if(buildImage!=nullptr)buildImage->setVisible(false);
    buildImage=nullptr;
}

void MapTile::removePix(){
    if(buildImage!=nullptr && bPix){

        bPix=false;

        MapTile::scene->removeItem(buildImage);


        delete buildImage;
        buildImage=nullptr;
    }
}
