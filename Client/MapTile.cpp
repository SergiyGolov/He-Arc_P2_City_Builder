#include "Client/MapTile.h"

#include <QFont>
#include <QDebug>

MapTile::MapTile(int x, int y, int left, int top, int width, int height,QGraphicsItem* parent):QGraphicsRectItem(left,top,width,height,parent)
{
    this->x=x;
    this->y=y;

    this->bId=-10;
    uniqueBId=-10;
    mainTileX=-10;
    mainTileY=-10;
    buildingWidth=-10;
    buildingHeight=-10;

    this->setZValue(1);
    bPix=false;
    bOccupied=false;
    buildImage=nullptr;
    delimiterRect=nullptr;
}

MapTile::~MapTile()
{

}
QGraphicsScene* MapTile::scene=nullptr;



void MapTile::setScene(QGraphicsScene *scene)
{
    MapTile::scene=scene;
}


/**
* addPixMove
* adds a pixmap to the tile while it is "moving" on the map
*
* @param QGraphicsPixmapItem *pix : the pixmap to add
*/

void MapTile::addPixMove(QGraphicsPixmapItem *pix)
{

    buildImage=pix;
    buildImage->setVisible(true);
    buildImage->setPos(pos().x()-rect().width()/4,pos().y()+rect().height()/2);

}

/**
* addPix
* adds the final pixmap to the tile
*
* @param QGraphicsPixmapItem *pix : the pixmap to add
*/
void MapTile::addPix(QGraphicsPixmapItem *pix)
{

    buildImage=pix;
    buildImage->setVisible(true);
    buildImage->setPos(pos().x()-rect().width()/4,pos().y()+rect().height()/2);
    buildImage->setOpacity(1);
    bPix=true;
}


/**
* addPixRoad
* adds the final pixmap to the tile if it is a road
*
* @param QGraphicsPixmapItem *pix : the pixmap to add
*/
void MapTile::addPixRoad(QGraphicsPixmapItem *pix)
{
    if(buildImage==nullptr)
    {

        buildImage=pix;
        buildImage->setPos(pos());
        bPix=true;
    }
}

/**
* removePixMove
* removes temporary pixmap
*/
void MapTile::removePixMove()
{
    if(buildImage!=nullptr)buildImage->setVisible(false);
    buildImage=nullptr;
}

/**
* removePix
* removes permanent pixmap
*/
void MapTile::removePix()
{
    if(buildImage!=nullptr && bPix)
    {
        bPix=false;
        MapTile::scene->removeItem(buildImage);

        delete buildImage;
        buildImage=nullptr;
    }
}

void MapTile::setDelimiterRect(int x,int y,int w,int h)
{
    delimiterRect=new QGraphicsRectItem(x,y,w,h);
    delimiterRect->setPen(QPen(Qt::black));
    delimiterRect->setVisible(true);
    delimiterRect->setZValue(4);
    scene->addItem(delimiterRect);
}
void MapTile::unsetDelimiterRect()
{
    if(delimiterRect!=nullptr)
    {
        scene->removeItem(delimiterRect);
        delete delimiterRect;
    }
}
