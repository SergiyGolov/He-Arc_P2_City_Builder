#include "Client/MapView.h"
#include "Client/ConstantBuilding.h"
#include "Client/GameManagementService.h"
#include "Client/BuildingManagementService.h"
#include "Client/GraphicService.h"
#include "Client/GuiView.h"
#include "general/RandomService.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <sys/time.h>
#include <QBrush>
#include <QtMath>
#include <QPointF>
#include <QKeyEvent>
#include <QPointF>
#include <QTimer>

MapView* MapView::mapViewInstance=nullptr;

MapView* MapView::getMapView()
{
    if(mapViewInstance == nullptr)
        mapViewInstance = new MapView();
    return mapViewInstance;
}

MapView::MapView(QWidget *parent): QGraphicsView(parent)
{
    currentBuild=nullptr;
    nbTimeTick=0;
    bTimeTick=false;
    roadStartX=0;
    roadStartY=0;
    bRoad=false;
    bPicker=false;
    bGrid=false;
    pickerBId=0;
    cameraX=0;
    cameraY=0;
    prevRect=nullptr;
    roadDir=0;
    lastTilePix=nullptr;
    bRadius=false;
    this->setStyleSheet("background-image: url(:/ressources/clouds.jpg)");
    //the purpose of this timer is to blink in red if the user try to add a building without an adjacent road
    timer=new QTimer(this);
    timer->setInterval(100);
    connect(timer,SIGNAL(timeout()),this,SLOT(blinkRedTileSlot()));

    this->setBackgroundBrush(QBrush(Qt::transparent));
    scene=new QGraphicsScene(this);
    MapTile::setScene(scene);

    this->setScene(scene);
    falseRoadAdd=false;

    zoomFactor=1;
    buildingCount=0;
    this->setMouseTracking(true);
    screenWidth=QApplication::desktop()->screenGeometry().width();
    screenHeight=QApplication::desktop()->screenGeometry().height();

    this->setGeometry(0,0,screenWidth,screenHeight-screenHeight/10);

    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    tempRoadList=new QList<MapTile*>();
    tempRemoveList=new QList<MapTile*>();
    neighbourList=new QList<MapTile*>();
    radiusList=new QList<QGraphicsEllipseItem*>();

    nbTiles=256;
    lastbId=-1;
    circleWidth=1;
    tiles=new QVector<MapTile*>();
    setTransformationAnchor(QGraphicsView::NoAnchor);

    srand(time(NULL));
    int seed = rand() ;
    RandomService::setSeed(seed);




}

/**
* generateMap
* generates the tiles for the map from previously given parameters (from a newgame or from a save)
*/
void MapView::generateMap()
{

    this->resetTransform();
    scene->clear();
    tiles->clear();
    tempRoadList->clear();
    tempRemoveList->clear();
    neighbourList->clear();



    radiusCircle=new QGraphicsEllipseItem(0,0,1,1);
    radiusCircle->setVisible(false);
    radiusCircle->setZValue(257);
    radiusCircle->setOpacity(0.5);

    scene->addItem(radiusCircle);
    pixelPerTile=((screenHeight-screenHeight/10)/nbTiles)*2;

    tiles->resize(nbTiles*nbTiles);

    baseColors=new QVector<QColor>(nbTiles*nbTiles);

    cells = RandomService::generateMap(nbTiles, nbTiles);

    for(int j=0;j<nbTiles;j++)
    {
        for(int i=0;i<nbTiles;i++)
        {
            int coorX=i*pixelPerTile;
            int coorY=j*pixelPerTile;

            MapTile *tile=new MapTile(i,j,0,0,pixelPerTile,pixelPerTile);
            tile->setPen(QPen(Qt::transparent));
            tile->setPos(coorX,coorY);


            tile->setBrush(QBrush(cells[i+j*nbTiles].color));

            scene->addItem(tile);

            if(cells[i+j*nbTiles].color.blue()>45)tile->setOccupied(true); //to prevent from building on water
            else tile->setOccupied(false);

            tiles->insert((i)+(j)*nbTiles,tile);
            baseColors->insert((i)+(j)*nbTiles,cells[i+j*nbTiles].color);
        }
    }

    translate(screenWidth/2,0);


    scale(1,0.5);
    rotate(45);

    this->translate(cameraX,cameraY);

    if(zoomFactor<1)
    {
        this->scale(0.5,0.5);
    }
    else if(zoomFactor>1)
    {
        for(int i=0;i<(zoomFactor-1)/0.05;i++)
        {
            this->scale(2,2);
        }
    }

    if(nbTiles>243)translateFactor=5.35*(double)pixelPerTile*(double)nbTiles/256.0;
    else if(nbTiles>75)translateFactor=4.55*(double)pixelPerTile*(double)nbTiles/256.0;
    else translateFactor=4.275*(double)pixelPerTile*(double)nbTiles/256.0;

    if(nbTiles>243)scaleFactor=0.19*256.0/(double)nbTiles;
    else scaleFactor=0.235*256.0/(double)nbTiles;




}

MapView::~MapView()
{

}

/**
* callPicker
* This function is called from other classes, initialize the picker to add a road/building and to remove a road/building
*
* @param int bId : building id
*/

void MapView::callPicker(int bId)
{

    if(!bPicker)
    {
        bPicker=true;
        prevRect=nullptr;
        pickerBId=bId;
        if(pickerBId != -1)
        {
            buildWidth=ConstantBuilding::get(pickerBId).getTileWidth();
            buildHeight=ConstantBuilding::get(pickerBId).getTileHeight();
            buildRadius=ConstantBuilding::get(pickerBId).getRadius();
        }
    }

}

/**
* wheelEvent
* Handles the zoom with the mouseWheel
*/

void MapView::wheelEvent(QWheelEvent *event)
{

    if(event->angleDelta().y()>0)
    {
        //zoom
        if(zoomFactor<1.25)
        {
            zoomFactor+=0.05;
            this->scale(2,2);
        }
    }
    else
    {
        //dezoom
        if(zoomFactor>0.95)
        {
            zoomFactor-=0.05;
            this->scale(0.5,0.5);
        }
    }
}

/**
* mouseMoveEvent
* Handles the movement of the picker on the map
*/

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    QList<QGraphicsItem *> itemsList=items(event->pos());
    if(!itemsList.isEmpty())
    {
        if(MapTile *rect=dynamic_cast<MapTile*>(itemsList.last()))
        {
            if(bPicker)
            {
                if(pickerBId!=-1) //if we are not in remove mode
                {

                    moveAddBuilding(rect);
                }
                else  //mode remove
                {
                    moveRemoveBuilding(rect);
                }
            }

            if(bRoad) //not a else if because the road boolean is affected in the code above
            {
                moveAddRoad(rect);
            }
        }
    }
}

/**
* mousePressEvent
* Handles the finalization of the picker on the map/cancels picker/shows building radius
*/

void MapView::mousePressEvent(QMouseEvent *event)
{

    QList<QGraphicsItem *> itemsList=items(event->pos());
    if(!itemsList.isEmpty())
    {
        if(MapTile *rect=dynamic_cast<MapTile*>(itemsList.last()))
        {
            if (event->button()==Qt::RightButton && (bPicker || bRoad) )
            {
                cancelAdd(rect);
            }
            else if(event->button()==Qt::LeftButton)
            {
                radiusCircle->setVisible(false);
                if(bRoad)
                {
                    finalAddRoad(rect);
                }
                else if(bPicker)
                {
                    bPicker=false;
                    if(pickerBId!=-1)
                    {
                        finalAddBuilding(rect);
                    }
                    else{
                        finalRemove(rect);
                    }
                }
                else{
                    showRadius(rect);
                }
            }
        }
    }
}



/**
* keyPressEvent
* Handles keyboard shortcuts
*/

void MapView::keyPressEvent(QKeyEvent *event)
{
    GraphicService::getGraphicService()->setKeyboardShortcuts(event->key());


}

/**
* moveRemoveBuilding
* Handles movement of the remove picker
*
* @param MapTile *rect : the MapTile under the mouse
*/

void MapView::moveRemoveBuilding(MapTile *rect)
{
    foreach(MapTile* tile,*tempRemoveList)
    {
        tile->setBrush(QBrush(prevRemoveColor));
        tile->setPixOpacity(1);
    }
    tempRemoveList->clear();


    if(rect->getBId()!=-10)
    {
        prevRemoveColor=rect->brush().color();
        for(int i=0;i<rect->getBuildingWidth();i++)
        {
            for(int j=0;j<rect->getBuildingHeight();j++)
            {
                if( tiles->at((rect->getMainTileX()+i)+( rect->getMainTileY()+j)*nbTiles)->getBId()==rect->getBId())
                {
                    tempRemoveList->append(tiles->at((rect->getMainTileX()+i)+( rect->getMainTileY()+j)*nbTiles));
                    tiles->at((rect->getMainTileX()+i)+( rect->getMainTileY()+j)*nbTiles)->setBrush(QBrush(Qt::black));
                    tiles->at((rect->getMainTileX()+i)+( rect->getMainTileY()+j)*nbTiles)->setPixOpacity(0.65);
                }
            }
        }
    }


}

/**
* moveAddBuilding
* Handles movement of the building add picker
*
* @param MapTile *rect : the MapTile under the mouse
*/
void MapView::moveAddBuilding(MapTile *rect)
{
    bool occuppiedTile=false;
    for(int i=0;i<buildWidth;i++)
    {
        for(int j=0;j<buildHeight;j++)
        {
            if(tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->isOccupied())
                occuppiedTile=true;
        }
    }
    if(rect!=prevRect && prevRect!=nullptr)
    {

        for(int i=0;i<buildWidth;i++)
        {
            for(int j=0;j<buildHeight;j++)
            {
                if(prevRect->getX()+i<nbTiles && prevRect->getY()+j<nbTiles)
                {
                    tiles->at((prevRect->getX()+i)+(prevRect->getY()+j)*nbTiles)->setBrush(QBrush(baseColors->at((prevRect->getX()+i)+(prevRect->getY()+j)*nbTiles)));
                }
            }
        }
    }

    if(rect->getX()+buildWidth-1<nbTiles && rect->getY()+buildHeight-1<nbTiles && !occuppiedTile )
    {

        QColor color=GraphicService::getColorFromBuildingCategory( ConstantBuilding::get(pickerBId).getCategory()-1);

        QString pixFilePath="NOPIX";
        bool pixExists=false;

        switch(pickerBId)//would be nice to test if the pixPath exists in constantBuilding instead...
        {
        case 1:
            pixFilePath=texHouse;
            break;
        }

        if(pixFilePath!="NOPIX")pixExists=true;

        if(currentBuild==nullptr && pixExists)
        {
            QTransform trans;
            trans.translate(256/nbTiles*pixelPerTile*0.0025,-256/nbTiles*pixelPerTile*0.005);
            trans.rotate(-45);
            trans.scale(1.2*0.04*256/nbTiles,1.6*0.04*256/nbTiles);

            currentBuild=new QGraphicsPixmapItem( QPixmap(pixFilePath));

            currentBuild->setOpacity(0.65);
            currentBuild->setZValue(3);

            currentBuild->setTransform(trans);

            scene->addItem(currentBuild);

        }
        if(prevRect!=nullptr && prevRect!=rect && pixExists)prevRect->removePixMove();
        if(prevRect!=rect && pixExists)
        {
            rect->addPixMove(currentBuild);
            lastTilePix=rect;
        }

        if(currentBuild!=nullptr)
        {
            color=baseColors->at((rect->getX())+(rect->getY())*nbTiles);
        }



        for(int i=0;i<buildWidth;i++)
        {
            for(int j=0;j<buildHeight;j++)
            {
                tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->setBrush(color);
            }
        }

        if(buildRadius>0 && (lastbId!=pickerBId ||rect!=prevRect)) //if the selected building has an effect radius
        {

            radiusCircle->setVisible(true);
            radiusCircle->setRect(rect->pos().x()-circleWidth/2,rect->pos().y()-circleWidth/2,circleWidth,circleWidth);

            int buildSizeOffset=buildHeight;
            if(buildWidth>buildHeight)buildSizeOffset=buildWidth;
            circleWidth=(buildRadius+buildSizeOffset-3)*pixelPerTile*2;


            radiusCircle->setPen(QPen(color));
            radiusCircle->setBrush(QBrush(color));


            lastbId=pickerBId;
        }

        if(prevRect!=rect && !bRoad)
            prevRect=rect;

    }
    else if(lastTilePix!=nullptr)
    {

        lastTilePix->removePixMove();
        lastTilePix=nullptr;
    }
    else{
        radiusCircle->setVisible(false);
    }

}


/**
* moveAddRoad
* Handles movement of the road add picker
*
* @param MapTile *rect : the MapTile under the mouse
*/
void MapView::moveAddRoad(MapTile* rect)
{
    if(rect->getX()==roadStartX || rect->getY()==roadStartY)
    {
        if(roadDir==0)
        {
            if(rect->getX()>roadStartX)
            {
                roadDir=1; //to allow to build only in one direction
            }
            else if(rect->getX()<roadStartX)
            {
                roadDir=-1;
            }
            else if(rect->getY()>roadStartY)
            {
                roadDir=2;
            }
            else if(rect->getY()<roadStartY)
            {
                roadDir=-2;
            }
        }

        int dir=(int)qFabs(roadDir);

        if(rect->getBId()==-10&&rect != prevRect && ((rect->getX()>roadStartX &&roadDir==1 && rect->getY()==roadStartY) ||(rect->getX()<roadStartX &&roadDir==-1&& rect->getY()==roadStartY )|| (rect->getY()>roadStartY && roadDir==2 && rect->getX()==roadStartX)|| (rect->getY()<roadStartY && roadDir==-2&& rect->getX()==roadStartX)) && tempRoadList->indexOf(rect)==-1)
        {

            int tileDistanceX=0;
            int tileDistanceY=0;
            if(prevRect!=nullptr)
            {
                tileDistanceX=(int)qFabs((double)rect->getX() - (double)prevRect->getX());
                tileDistanceY=(int)qFabs((double)rect->getY() - (double)prevRect->getY());

            }
            int multDirection=1;

            if(tileDistanceX>1 || tileDistanceY>1)
            {
                if((rect->getX()<prevRect->getX() && dir==1 )|| (rect->getY()<prevRect->getY() && (int)qFabs(roadDir)==2))multDirection=-1;
            }
            //if the mouse "jumped" more than one tile in X
            if(tileDistanceX > 1 && dir==1)
            {

                for(int i=0;i<tileDistanceX+1;i++)
                {
                    if(!tiles->at((prevRect->getX()+i*multDirection)+(prevRect->getY())*nbTiles)->isOccupied())
                    {
                        tiles->at((prevRect->getX()+i*multDirection)+(prevRect->getY())*nbTiles)->setBrush(QBrush(Qt::darkGray));

                        tempRoadList->append(tiles->at((prevRect->getX()+i*multDirection)+(prevRect->getY())*nbTiles));
                    }
                }
            }
            //same thing but in y
            else if(tileDistanceY > 1 && dir==2)
            {

                for(int i=0;i<tileDistanceY+1;i++)
                {
                    if(!tiles->at((prevRect->getX())+(prevRect->getY()+i*multDirection)*nbTiles)->isOccupied())
                    {
                        tiles->at((prevRect->getX())+(prevRect->getY()+i*multDirection)*nbTiles)->setBrush(QBrush(Qt::darkGray));

                        tempRoadList->append(tiles->at((prevRect->getX())+(prevRect->getY()+i*multDirection)*nbTiles));
                    }
                }
            }
            else if(!rect->isOccupied())
            {
                tempRoadList->append(rect);
                rect->setBrush(QBrush(Qt::darkGray));
            }

            prevRect=rect;
        }

    }
}


/**
* finalRemove
* Handles the removing of a selected building on the map
*
* @param MapTile *rect : the MapTile under the mouse
*/
void MapView::finalRemove(MapTile* rect)
{
    int initialBid=rect->getBId();
    //if it is not a default tile (no building/road on it)
    if(initialBid!=-10)
    {
        if(rect->getUniqueBId()!=-10)BuildingManagementService::getBuildingManagementService()->removeBuilding(rect->getUniqueBId());
        tiles->at((rect->getMainTileX())+(rect->getMainTileY())*nbTiles)->unsetDelimiterRect();
        for(int i=0;i<rect->getBuildingWidth();i++)
        {
            for(int j=0;j<rect->getBuildingHeight();j++)
            {
                if( tiles->at((rect->getMainTileX()+i)+(rect->getMainTileY()+j)*nbTiles)->getBId()==initialBid)
                {
                    tiles->at((rect->getMainTileX()+i)+(rect->getMainTileY()+j)*nbTiles)->removePix();
                    tiles->at((rect->getMainTileX()+i)+(rect->getMainTileY()+j)*nbTiles)->setOccupied(false);
                    tiles->at((rect->getMainTileX()+i)+(rect->getMainTileY()+j)*nbTiles)->setBId(-10);

                    tiles->at((rect->getMainTileX()+i)+(rect->getMainTileY()+j)*nbTiles)->setUniqueBId(-10);
                    tiles->at((rect->getMainTileX()+i)+(rect->getMainTileY()+j)*nbTiles)->setBrush(QBrush(baseColors->at((rect->getMainTileX()+i)+(rect->getMainTileY()+j)*nbTiles)));
                }
            }
        }
        tempRemoveList->clear();
        lastTilePix=nullptr;
    }
}

/**
* finalAddBuilding
* Handles the adding of a selected building on the map
*
* @param MapTile *rect : the MapTile under the mouse
*/
void MapView::finalAddBuilding(MapTile* rect)
{
    bool caseOccupe=false;
    for(int i=0;i<buildWidth;i++)
    {
        for(int j=0;j<buildHeight;j++)
        {
            if(tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->isOccupied())caseOccupe=true;
        }
    }

    if(rect->getX()+buildWidth-1<nbTiles && rect->getX()+buildHeight-1<nbTiles && caseOccupe==false)
    {
        if(pickerBId==0 && !bRoad)
        {
            bRoad=true;
            roadDir=0;
            roadStartX=rect->getX();
            roadStartY=rect->getY();
            tempRoadList->append(rect);

        }
        prevRect=nullptr;
        if(checkIfNearRoad(rect) || bRoad )
        {
            QColor color=GraphicService::getColorFromBuildingCategory(ConstantBuilding::get(pickerBId).getCategory()-1);

            //TODO: set the real image of the building instead of colorizing the tiles => switch on the buildingId instead of the category
            if(currentBuild!=nullptr)
            {

                color=baseColors->at((rect->getX())+(rect->getY())*nbTiles);
            }

            lastTilePix=nullptr;
            rect->setBrush(color);
            int mainTileX=rect->getX();
            int mainTileY=rect->getY();

            if(currentBuild!=nullptr)
            {
                rect->addPix(currentBuild);

                currentBuild=nullptr;
            }

            if(pickerBId!=0)
            {
                buildingCount++;
                if(pickerBId!=1)rect->setDelimiterRect(rect->pos().x(),rect->pos().y(),buildWidth*pixelPerTile,buildHeight*pixelPerTile);
                for(int i=0;i<buildWidth;i++)
                {
                    for(int j=0;j<buildHeight;j++)
                    {
                        tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->setOccupied(true);
                        tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->setBId(pickerBId);
                        tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->setMainTile(mainTileX,mainTileY);
                        tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->setBuildingWidth(buildWidth);
                        tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->setBuildingHeight(buildHeight);
                        tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->setUniqueBId(buildingCount);

                    }
                }
                BuildingManagementService::getBuildingManagementService()->addBuilding(pickerBId,mainTileX,mainTileY, 0); //TODO : set 0 to 3 angles
            }
        }
        else if(!bRoad)
        {
            for(int i=0;i<buildWidth;i++)
            {
                for(int j=0;j<buildHeight;j++)
                {
                    if(rect->getX()+i<nbTiles && rect->getY()+j<nbTiles)
                    {
                        tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->setBrush(QBrush(baseColors->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)));

                    }
                }
            }
            blinkTileRed(rect);
        }
        radiusCircle->setVisible(false);
    }
}


/**
* finalAddRoad
* Handles the adding of a road
*
* @param MapTile *rect : the MapTile under the mouse
*/
void MapView::finalAddRoad(MapTile* rect)
{

    bRoad=false;
    int dir=(int)qFabs(roadDir);

    if(rect->getBId()!=0)
    {
        if(((rect->getX()>roadStartX &&roadDir==1 && rect->getY()==roadStartY || rect->getX()<roadStartX &&roadDir==-1&& rect->getY()==roadStartY || rect->getY()>roadStartY && roadDir==2 && rect->getX()==roadStartX|| rect->getY()<roadStartY && roadDir==-2&& rect->getX()==roadStartX)))
        {

            int lastX=-1;
            int lastY=-1;

            int aNeighbourCount[tempRoadList->count()];
            bool bNeighbour=false;
            foreach(MapTile* tile, *tempRoadList)
            {
                int neighbourCount=countNeighbourRoads(tile);
                aNeighbourCount[tempRoadList->indexOf(tile)]=neighbourCount;
                if(neighbourCount>0)bNeighbour=true;
            }

            foreach(MapTile* tile, *tempRoadList)
            {
                if(tile->getBId()==-10 && tile->getX()!=lastX && dir==1 || tile->getY()!=lastY && dir==2)
                {

                    tile->setOccupied(true);
                    tile->setBId(0);

                    tile->setMainTile(roadStartX,roadStartY);
                    if(roadDir==1)
                    {
                        tile->setBuildingWidth(rect->getX()-roadStartX+1);
                        tile->setBuildingHeight(1);
                    }
                    else if(roadDir==-1)
                    {
                        tile->setBuildingWidth(roadStartX-rect->getX()+1);
                        tile->setBuildingHeight(1);
                        tile->setMainTile(rect->getX(),rect->getY());
                        rect->setMainTile(rect->getX(),rect->getY());
                    }
                    else if(roadDir==2)
                    {
                        tile->setBuildingWidth(1);
                        tile->setBuildingHeight(rect->getY()-roadStartY+1);
                    }
                    else if(roadDir==-2)
                    {
                        tile->setBuildingWidth(1);
                        tile->setBuildingHeight(roadStartY-rect->getY()+1);
                        tile->setMainTile(rect->getX(),rect->getY());
                        rect->setMainTile(rect->getX(),rect->getY());
                    }

                    QString pixFilePath=texRoad;
                    QTransform trans;
                    trans.scale(scaleFactor,scaleFactor);

                    int angle;
                    if(dir==1 )
                    {
                        angle=0;
                    }
                    else{
                        trans.rotate(-90);
                        trans.translate(-translateFactor,0);
                        angle=1;
                    }

                    tile->setUniqueBId(++buildingCount);
                    BuildingManagementService::getBuildingManagementService()->addBuilding(0,tile->getX(),tile->getY(), angle);
                    currentBuild=new QGraphicsPixmapItem(QPixmap(pixFilePath));

                    currentBuild->setZValue(1);
                    currentBuild->setTransform(trans);
                    scene->addItem(currentBuild);
                    tile->addPixRoad(currentBuild);
                    currentBuild=nullptr;
                    tile->setBrush(QBrush(baseColors->at((tile->getX())+(tile->getY())*nbTiles)));
                    lastX=tile->getX();
                    lastY=tile->getY();
                }

            }

            if(bNeighbour)
            {
                for(int i=0;i<tempRoadList->count();i++)
                {
                    if(aNeighbourCount[i]>0)
                    {
                        updateNeighbourRoad(tempRoadList->at(i));
                    }
                }
            }


            tempRoadList->clear();

        }
        else{

            blinkTileRed(rect);
            cancelAdd(rect);
        }

    }
    else{
        falseRoadAdd=true;
        //copier rect, cancelAdd, et remettre rect
        int mainTileX=rect->getMainTileX();
        int mainTileY=rect->getMainTileY();
        int width=rect->getBuildingWidth();
        int height=rect->getBuildingHeight();
        int uniqBid=rect->getUniqueBId();
        QGraphicsPixmapItem *pix=rect->getPix();

        cancelAdd(rect);
        rect->addPix(pix);
        pix->moveBy(0.25*pixelPerTile, -0.5*pixelPerTile);
        rect->setMainTile(mainTileX,mainTileY);
        rect->setBuildingWidth(width);
        rect->setBuildingHeight(height);
        rect->setBId(0);
        rect->setUniqueBId(uniqBid);


    }
}

/**
* cancelAdd
* Handels the cancelling of the picker
*
* @param MapTile* rect : The Maptile under the mouse
*/

void MapView::cancelAdd(MapTile* rect)
{

    if(currentBuild!=nullptr)
    {
        delete currentBuild;
        currentBuild=nullptr;
    }
    bool occupiedTile=false;
    for(int i=0;i<buildWidth;i++)
    {
        for(int j=0;j<buildHeight;j++)
        {
            if(rect->isOccupied())
                occupiedTile=true;
        }
    }
    if(!occupiedTile || falseRoadAdd)
    {
        falseRoadAdd=false;
        bRoad=false;
        bPicker=false;
        foreach(MapTile* tile,*tempRoadList)
        {
            tile->removePix();
            if(!bGrid)tile->setPen(QPen(Qt::transparent));
            else tile->setPen(QPen(Qt::black));
            tile->setBrush(QBrush(baseColors->at((tile->getX())+(tile->getY())*nbTiles)));
            tile->setOccupied(false);
            tile->setBuildingWidth(-10);
            tile->setBuildingHeight(-10);
            tile->setMainTile(-10,-10);

        }

        tempRoadList->clear();
        radiusCircle->setVisible(false);


        rect->setBrush(QBrush(baseColors->at((rect->getX())+(rect->getY())*nbTiles)));
        for(int i=0;i<buildWidth;i++)
        {
            for(int j=0;j<buildHeight;j++)
            {
                if(rect->getX()+i<nbTiles && rect->getY()+j<nbTiles)
                {
                    if(!tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->isOccupied())tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->setBrush(QBrush(baseColors->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)));
                }
            }
        }
    }
}

/**
* toggleGrid
* Toggles the grid display
*/

void MapView::toggleGrid()
{
    for(int i=0;i<nbTiles;i++)
    {
        for(int j=0;j<nbTiles;j++)
        {
            if(bGrid)
            {
                if(!tiles->at((i)+(j)*nbTiles)->isOccupied())tiles->at((i)+(j)*nbTiles)->setPen(QPen(Qt::transparent));
            }
            else if(!bGrid)
            {
                if(!tiles->at((i)+(j)*nbTiles)->isOccupied())tiles->at((i)+(j)*nbTiles)->setPen(QPen(Qt::black));
            }
        }
    }
    bGrid=!bGrid;
}


/**
* zoomMeth
* Handles the zooming, is called from another class
*
* @param bool plusMinus : true->zoom in false->zoom out
*/

void MapView::zoomMeth(bool plusMinus)
{
    if(plusMinus)
    {
        if(zoomFactor<1.25)
        {
            zoomFactor+=0.05;
            this->scale(2,2);
        }
    }
    else{
        if(zoomFactor>0.95)
        {
            zoomFactor-=0.05;
            this->scale(0.5,0.5);
        }
    }
}


/**
* removeBuildingMode
* Activates the remove picker, called from another class
*/
void MapView::removeBuildingMode()
{
    if(!bPicker)
    {
        MapView::getMapView()->callPicker(-1);
    }
}

/**
* addRoadMode
* Activates the road picker, called from another class
*/
void MapView::addRoadMode()
{
    if(!bPicker && !bRoad)
    {
        MapView::getMapView()->callPicker(0);
    }
}



/**
* translateMeth
* Handles the translation, is called from another class
*
* @param int direction : 1->right -1->left 2->up -2->down
*/
void MapView::translateMeth(int direction)
{
    int zoomFactorAdapter=5;
    switch(direction)
    {
    case -2:
        this->translate(-15/zoomFactor-35/(zoomFactor*zoomFactorAdapter),-15/zoomFactor-35/(zoomFactor*zoomFactorAdapter));
        cameraX+=-15/zoomFactor-35/(zoomFactor*zoomFactorAdapter);
        cameraY+=-15/zoomFactor-35/(zoomFactor*zoomFactorAdapter);
        break;
    case 2:
        this->translate(15/zoomFactor+35/(zoomFactor*zoomFactorAdapter),15/zoomFactor+35/(zoomFactor*zoomFactorAdapter));
        cameraX+=15/zoomFactor+35/(zoomFactor*zoomFactorAdapter);
        cameraY+=15/zoomFactor+35/(zoomFactor*zoomFactorAdapter);
        break;
    case 1:
        this->translate(-15/zoomFactor-35/(zoomFactor*zoomFactorAdapter),15/zoomFactor+35/(zoomFactor*zoomFactorAdapter));
        cameraX+=-15/zoomFactor-35/(zoomFactor*zoomFactorAdapter);
        cameraY+=15/zoomFactor+35/(zoomFactor*zoomFactorAdapter);
        break;
    case -1:
        this->translate(15/zoomFactor+35/(zoomFactor*zoomFactorAdapter),-15/zoomFactor-35/(zoomFactor*zoomFactorAdapter));
        cameraX+=15/zoomFactor+35/(zoomFactor*zoomFactorAdapter);
        cameraY+=-15/zoomFactor-35/(zoomFactor*zoomFactorAdapter);
        break;
    }


}

/**
* checkIfNearRoad
* Checks if a tile's neighbour is a road
*
* @param MapTile* tile : the maptile which we want to check if it has a road as neighbour
* @return true-> a neighbour is a road false-> no neighbour is a road
*/

bool MapView::checkIfNearRoad(MapTile* tile)
{
    for(int i=0; i < buildWidth; i++)
    {
        for(int j=0; j < buildHeight; j++)
        {
            if(tile->getX()-1+i>=0 && tiles->at((tile->getX()-1+i)+(tile->getY()+j)*nbTiles)->getBId()==0)return true;
            if(tile->getX()+1+i<nbTiles && tiles->at((tile->getX()+1+i)+(tile->getY()+j)*nbTiles)->getBId()==0)return true;
            if(tile->getY()-1+j>=0 && tiles->at((tile->getX()+i)+(tile->getY()-1+j)*nbTiles)->getBId()==0)return true;
            if(tile->getY()+1+j<nbTiles && tiles->at((tile->getX()+i)+(tile->getY()+1+j)*nbTiles)->getBId()==0)return true;
        }
    }
    return false;
}

/**
* blinkTileRed
* Starts to blink in red, used if the player tries to make an illegal action
*
* @param MapTile* tile : The tile that has to start blinking
*/

void MapView::blinkTileRed(MapTile* tile)
{
    tile->removePixMove();
    currentBuild=nullptr;
    blinkRedTile = tile;
    timer->start();
}


/**
* blinkRedTileSlot
* Slot called by the timer to start a tile to blink in red
*
* @param MapTile* tile : The tile that has to start blinking
*/
void MapView::blinkRedTileSlot()
{
    if(nbTimeTick<6)
    {
        nbTimeTick++;
        for(int i=0;i<buildWidth;i++)
        {
            for(int j=0;j<buildHeight;j++)
            {
                if(bTimeTick)
                {
                    tiles->at((blinkRedTile->getX()+i)+(blinkRedTile->getY()+j)*nbTiles)->setPen(QPen(Qt::red));
                }
                else
                {
                    if(bGrid)
                        tiles->at((blinkRedTile->getX()+i)+(blinkRedTile->getY()+j)*nbTiles)->setPen(QPen(Qt::black));
                    else
                        tiles->at((blinkRedTile->getX()+i)+(blinkRedTile->getY()+j)*nbTiles)->setPen(QPen(Qt::transparent));
                }
            }
        }
        bTimeTick=!bTimeTick;
    }
    else
    {
        nbTimeTick=0;
        timer->stop();
        for(int i=0;i<buildWidth;i++)
        {
            for(int j=0;j<buildHeight;j++)
            {
                if(bGrid)
                    tiles->at((blinkRedTile->getX()+i)+(blinkRedTile->getY()+j)*nbTiles)->setPen(QPen(Qt::black));
                else
                    tiles->at((blinkRedTile->getX()+i)+(blinkRedTile->getY()+j)*nbTiles)->setPen(QPen(Qt::transparent));
            }
        }
    }
}


/**
* countNeighbourRoads
* Counts the number of neighbours that are roads
*
* @param MapTile* tile: the tile which neighbours has to be counted
* @return number of neighbours that are roads
*/

int MapView::countNeighbourRoads(MapTile* tile)
{
    int n=0;

    if(tile->getX()-1>=0 && tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0)n++;
    if(tile->getX()+1<nbTiles && tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0)n++;
    if(tile->getY()-1>=0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0)n++;
    if(tile->getY()+1<nbTiles && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0)n++;

    return n;
}


/**
* getNeighbours
* Puts all the neighbours of a tile in the neighbourList
*
* @param MapTile* tile: the tile which neighbours have to be put in a list
*/
void MapView::getNeighbours(MapTile* tile)
{
    neighbourList->clear();
    if(tile->getX()-1>=0 && tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0)neighbourList->append(tiles->at((tile->getX()-1)+(tile->getY())*nbTiles));
    if(tile->getY()-1>=0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0)neighbourList->append(tiles->at((tile->getX())+(tile->getY()-1)*nbTiles));
    if(tile->getX()+1<nbTiles && tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0)neighbourList->append(tiles->at((tile->getX()+1)+(tile->getY())*nbTiles));
    if(tile->getY()+1<nbTiles && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0)neighbourList->append(tiles->at((tile->getX())+(tile->getY()+1)*nbTiles));
    neighbourList->append(tile);

}

/**
* updateNeighbourRoad
* Updates all the neighbour roads textures after adding a road next to them, for example to handle road intersections
*
* @param MapTile* tile: the tile which neighbours have to be potentially updated
*/
void MapView::updateNeighbourRoad(MapTile* tileIn)
{
    getNeighbours(tileIn);

    foreach(MapTile* tile,*neighbourList)
    {

        int neighbourCount=countNeighbourRoads(tile);

        QString pixPath="NOPIX";
        QTransform trans;

        switch(neighbourCount)
        {
        //"T"
        case 3:
            pixPath=texRoadT;
            BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),3);

            break;
            //crossroad
        case 4:

            pixPath=texRoadX;
            BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),2);

            break;
        }

        trans.scale(scaleFactor,scaleFactor);

        switch(neighbourCount)
        {
        case 2:
            if(tile->getX()-1>=0 && tile->getY()+1<nbTiles && tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0)
            {
                trans.rotate(-90);
                trans.translate(-translateFactor,0);
                pixPath=texRoadL;
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),7);
            }
            else if(tile->getX()+1<nbTiles && tile->getY()-1>=0 && tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0)
            {
                trans.rotate(90);
                trans.translate(0,-translateFactor);
                pixPath=texRoadL;
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),8);
            }
            else if(tile->getX()+1<nbTiles && tile->getY()+1<nbTiles && tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0)
            {
                trans.rotate(180);
                trans.translate(-translateFactor,-translateFactor);
                pixPath=texRoadL;
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),9);
            }
            else if(tile->getX()-1>=0 && tile->getY()-1>=0 && tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0)
            {
                pixPath=texRoadL;
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),10);
            }
            break;

        case 3:
            if(tile->getX()-1 >=0 && tile->getY()-1>=0 && tile->getX()+1<nbTiles&&tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0)
            {
                trans.rotate(180);
                trans.translate(-translateFactor,-translateFactor);
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),4);
            }
            if(tile->getX()+1 <nbTiles && tile->getY()-1>=0 && tile->getY()+1<nbTiles&&tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0)
            {
                trans.rotate(-90);
                trans.translate(-translateFactor,0);
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),5);
            }
            if(tile->getX()-1>=0 && tile->getY()-1>=0 && tile->getY()+1<nbTiles&&tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0)
            {
                trans.rotate(90);
                trans.translate(0,-translateFactor);
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),6);
            }
            break;
        }

        if(pixPath!="NOPIX")
        {
            QGraphicsPixmapItem *roadPix=new QGraphicsPixmapItem(QPixmap(pixPath));
            scene->addItem(roadPix);
            tile->removePix();

            roadPix->setTransform(trans);
            roadPix->setZValue(2);
            tile->addPixRoad(roadPix);
        }
    }
}

/**
* addBuildingFromSave
* Adds a building to the Map that is read from a save file
*
* @param int id: the building id
* @param int x: building x position in the maptile array
* @param int y: building y position in the maptile array
* @param int angle: useful for the roads, to know the orientation/texture of the road (if it is a crossroad etc...)
*/
void MapView::addBuildingFromSave(int id, int x, int y, int angle)
{
    int buildWidthSave=ConstantBuilding::get(id).getTileWidth();
    int buildHeightSave=ConstantBuilding::get(id).getTileHeight();


    if(id!=0)
    {
        QColor color=GraphicService::getColorFromBuildingCategory(ConstantBuilding::get(id).getCategory()-1);


        buildingCount++;
        for(int i=0;i<buildWidthSave;i++)
        {
            for(int j=0;j<buildHeightSave;j++)
            {
                tiles->at((x+i)+(y+j)*nbTiles)->setOccupied(true);
                tiles->at((x+i)+(y+j)*nbTiles)->setBId(id);
                tiles->at((x+i)+(y+j)*nbTiles)->setMainTile(x,y);
                tiles->at((x+i)+(y+j)*nbTiles)->setBuildingWidth(buildWidthSave);
                tiles->at((x+i)+(y+j)*nbTiles)->setBuildingHeight(buildHeightSave);
                tiles->at((x+i)+(y+j)*nbTiles)->setUniqueBId(buildingCount);
                if(id!=1)tiles->at((x+i)+(y+j)*nbTiles)->setBrush(color); //would be nice to test if the pixPath exists in constantBuilding instead...
                else {
                    tiles->at((x+i)+(y+j)*nbTiles)->setBrush(baseColors->at((x+i)+(y+j)*nbTiles));
                    QTransform trans;

                    trans.scale(1.1*0.04*256/nbTiles,1.2*0.04*256/nbTiles);
                    trans.rotate(-45);
                    currentBuild=new QGraphicsPixmapItem( QPixmap(texHouse));
                    currentBuild->setZValue(2);
                    currentBuild->setTransform(trans);

                    scene->addItem(currentBuild);
                    tiles->at((x+i)+(y+j)*nbTiles)->addPix(currentBuild);
                    currentBuild=nullptr;
                }
            }
        }
    }
    else{
        buildingCount++;
        tiles->at(x+y*nbTiles)->setOccupied(true);
        tiles->at(x+y*nbTiles)->setBId(id);
        tiles->at(x+y*nbTiles)->setMainTile(x,y);
        tiles->at(x+y*nbTiles)->setBuildingWidth(1);
        tiles->at(x+y*nbTiles)->setBuildingHeight(1);
        tiles->at(x+y*nbTiles)->setUniqueBId(buildingCount);

        QTransform trans;
        trans.scale(scaleFactor,scaleFactor);
        QString pixPath;

        switch(angle)
        {
        case 1:
            trans.rotate(-90);
            trans.translate(-translateFactor,0);
        case 0:
            pixPath=texRoad;
            break;
        case 2:
            pixPath=texRoadX;
            break;
        case 4:
            trans.rotate(180);
            trans.translate(-translateFactor,-translateFactor);
        case 3:
            pixPath=texRoadT;
            break;
        case 5:
            trans.rotate(-90);
            trans.translate(-translateFactor,0);
            pixPath=texRoadT;
            break;
        case 6:
            trans.rotate(90);
            trans.translate(0,-translateFactor);
            pixPath=texRoadT;
            break;
        case 7:
            trans.rotate(-90);
            trans.translate(-translateFactor,0);
            pixPath=texRoadL;
            break;
        case 8:
            trans.rotate(90);
            trans.translate(0,-translateFactor);
            pixPath=texRoadL;
            break;
        case 9:
            trans.rotate(180);
            trans.translate(-translateFactor,-translateFactor);
        case 10:
            pixPath=texRoadL;
            break;
        }
        currentBuild=new QGraphicsPixmapItem(QPixmap(pixPath));
        currentBuild->setZValue(2);
        currentBuild->setTransform(trans);
        scene->addItem(currentBuild);
        tiles->at(x+y*nbTiles)->addPixRoad(currentBuild);
        currentBuild=nullptr;
    }
}


/**
* toggleAllBuildingRadius
* toggles the display of all the building radius on the map
*/

void MapView::toggleAllBuildingRadius()
{
    bRadius=!bRadius;
    if(bRadius)
    {
        radiusCircle->setVisible(false);
        QVector<Building*> *vectorEffectBuildings=BuildingManagementService::getBuildingManagementService()->getEffectBuildings();
        for(int i=0;i<vectorEffectBuildings->size();i++)
        {

            QGraphicsEllipseItem *radius=new QGraphicsEllipseItem;
            setRadiusCircle(radius,vectorEffectBuildings->at(i)->getId(),vectorEffectBuildings->at(i)->getX(),vectorEffectBuildings->at(i)->getY());
            radius->setZValue(257);
            radius->setOpacity(0.35);
            radiusList->append(radius);
            scene->addItem(radius);
        }
    }
    else
    {
        foreach(QGraphicsEllipseItem* circle,*radiusList)
        {
            scene->removeItem(circle);
        }
        radiusList->clear();
    }
}


/**
* showRadius
* display the radius of a selected building
* @param MapTile* rect : The maptile which radius we want to display (if it has one)
*/
void MapView::showRadius(MapTile* rect)
{
    if(!bRadius&&rect ->isOccupied() && rect->getBId()!=-10 && ConstantBuilding::get(rect->getBId()).getRadius()>1)
    {
        radiusCircle->setVisible(true);
        setRadiusCircle(radiusCircle,rect->getBId(),rect->getMainTileX(),rect->getMainTileY());
    }
}


/**
* setRadiusCircle
* display a circle to represent the radius
* @param QGraphicsEllipseItem* radius : circle that will display the radius
* @param int bId : building id of the tile which radius we want to display
* @param int x : building x coordinate
* @param int x : building y coordinate
*/
void MapView::setRadiusCircle(QGraphicsEllipseItem* radius,int bId,int x,int y)
{
    int abuildHeight=ConstantBuilding::get(bId).getTileHeight();
    int abuildWidth=ConstantBuilding::get(bId).getTileWidth();
    int buildSizeOffset=abuildHeight;
    int aBuildRadius=ConstantBuilding::get(bId).getRadius();
    if(abuildWidth>abuildHeight)buildSizeOffset=abuildWidth;
    int aCircleWidth=(aBuildRadius+buildSizeOffset-3)*pixelPerTile*2;

    radius->setRect(tiles->at(x+y*nbTiles)->pos().x()-aCircleWidth/2,tiles->at(x+y*nbTiles)->pos().y()-aCircleWidth/2,aCircleWidth,aCircleWidth);

    if(abuildWidth>abuildHeight)buildSizeOffset=abuildWidth;
    circleWidth=(aBuildRadius+buildSizeOffset-3)*pixelPerTile*2;

    QColor color=GraphicService::getColorFromBuildingCategory(ConstantBuilding::get(bId).getCategory()-1);

    radius->setPen(QPen(color));
    radius->setBrush(QBrush(color));
}
