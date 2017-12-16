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

   // this->text=new QGraphicsSimpleTextItem("A");

    this->setZValue(2);
    //this->text->setZValue(1);
    //this->text->setPos(left,top);

//    QFont font;
//    font.setPixelSize(height);
//    font.setBold(false);
//    this->text->setFont(font);

   // this->text->setPen(QPen(Qt::white));
    this->setZValue(1);
    //this->text->setZValue(2);

//   if(MapTile::scene!=nullptr)MapTile::scene->addItem(this->text);
//   this->text->setRotation(45);
   //this->setRotation(45);
}


MapTile::~MapTile(){

}
QGraphicsScene* MapTile::scene=nullptr;



 void MapTile::setScene(QGraphicsScene *scene){
     MapTile::scene=scene;
 }
