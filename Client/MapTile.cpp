#include "MapTile.h"
#include <QFont>

MapTile::MapTile(int x, int y, int left, int top, int width, int height,QGraphicsItem* parent):QGraphicsRectItem(left,top,width,height,parent){

    this->x=x;
    this->y=y;

    this->bId=-10;
      uniqueBId=-10;
     mainTileX=-10;
     mainTileY=-10;
     largeurBat=-10;
     hauteurBat=-10;


    this->setZValue(2);

    this->setZValue(1);
}


MapTile::~MapTile(){

}
QGraphicsScene* MapTile::scene=nullptr;



 void MapTile::setScene(QGraphicsScene *scene){
     MapTile::scene=scene;
 }
