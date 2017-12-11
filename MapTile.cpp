#include "MapTile.h"

MapTile::MapTile(int x, int y, int left, int top, int width, int height,QGraphicsItem* parent):QGraphicsRectItem(left,top,width,height,parent){

    this->x=x;
    this->y=y;
}


MapTile::~MapTile(){

}
