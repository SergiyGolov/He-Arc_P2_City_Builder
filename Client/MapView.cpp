#include "MapView.h"
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
#include <GuiView.h>
#include <QTimer>
#include "ConstantBuilding.h"
#include "GameManagementService.h"
#include "BuildingManagementService.h"
#include "GraphicService.h"
#include "RandomService.h"


MapView* MapView::mapViewInstance=nullptr;

MapView* MapView::getMapView()
{
    if(mapViewInstance == nullptr)
        mapViewInstance = new MapView();
    return mapViewInstance;
}

MapView::MapView(QWidget *parent): QGraphicsView(parent)
{
    nbTimeTick=0;
    timeTick=false;
    roadStartX=0;
    roadStartY=0;
    road=false;
    bPicker=false;
    click=false;
    grid=false;
    pickerBId=0;
    this->gui=GuiView::getGuiView();
    prevRect=nullptr;
    roadDir=0;


    //the purpose of this timer is to blink in red if the user try to add a building without an adjacent road
    timer=new QTimer(this);
    timer->setInterval(100);
    connect(timer,SIGNAL(timeout()),this,SLOT(blinkRedTileSlot()));

    this->setBackgroundBrush(QBrush(Qt::black));
    scene=new QGraphicsScene(this);
    MapTile::setScene(scene);

    this->setScene(scene);
    zoom=1;
    buildingCount=0;
    this->setMouseTracking(true);
    screenWidth=QApplication::desktop()->screenGeometry().width();
    screenHeight=QApplication::desktop()->screenGeometry().height();

    this->setGeometry(0,0,screenWidth,screenHeight-screenHeight/10);

    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );


    radiusTiles=new QList<MapTile*>();
    tempRoad=new QList<MapTile*>();
    tempRemove=new QList<MapTile*>();

    int nbCases = nbTiles;

    pixelPerTile=((screenHeight-screenHeight/10)/nbCases)*2; //*qSqrt(2)



    setTransformationAnchor ( QGraphicsView::NoAnchor );
    srand(time(NULL));
    int seed = rand() ;

    cells = RandomService::generateMap(seed, nbCases, nbCases);

    for(int i=0;i<nbCases;i++)
    {
        for(int j=0;j<nbCases;j++)
        {
            /*

            int random = (rand() % (6));

            switch(random)
            {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                // color=Qt::darkGreen;
                //break;
            case 5:
                color=Qt::darkGreen;//darkBlue
                break;
            }
            */
            int coorX=i*pixelPerTile;
            int coorY=j*pixelPerTile;


            MapTile *tile=new MapTile(i,j,coorX,coorY,pixelPerTile,pixelPerTile);
            tile->setPen(QPen(Qt::transparent));

            // tile->setBrush(QBrush(Qt::darkGreen));
            tile->setBrush(QBrush(cells[i+j*nbCases].color));
            scene->addItem(tile);
            tiles[i][j]=tile;
            baseColors[i][j]=cells[i+j*nbCases].color;
            if(cells[i+j*nbCases].color.blue()>45)tilesBool[i][j]=true;
            else tilesBool[i][j]=false;

            tile->setRotation(45);
        }

    }
    translate(screenWidth/2,0);


    scale(1,0.5);
}

MapView::~MapView()
{

}

void MapView::callPicker(int bId){
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

void MapView::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y()>0)
    {
        //zoom
        if(zoom<1.25)
        {
            zoom+=0.05;
            this->scale(2,2);
        }
    }
    else
    {
        //dezoom
        if(zoom>1)
        {
            zoom-=0.05;
            this->scale(0.5,0.5);
        }
    }
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{

    //1: right -1: left 2: up -2: down
    //     if(event->pos().x()<20 && event->pos().x()>0){
    //       translateMeth(-1);


    //    }else if(event->pos().x()>width()-20 && event->pos().x()<width()){
    //       translateMeth(1);


    //    }else if(event->pos().y()<20 && event->pos().y()>0){
    //          translateMeth(2);

    //    }
    //    else if(event->pos().y()>height()-20 && event->pos().y()<height()){
    //          translateMeth(-2);

    //    }

    if(bPicker)
    {
        if(pickerBId!=-1) //if we are not in remove mode
        {
            if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos())))
            {

                foreach(MapTile* tile,*radiusTiles)
                {
                    if(!grid)tile->setPen(QPen(Qt::transparent));
                    else tile->setPen(QPen(Qt::black));
                }

                radiusTiles->clear();
                bool occuppiedTile=false;
                for(int i=0;i<buildWidth;i++)
                {
                    for(int j=0;j<buildHeight;j++)
                    {
                        if(tilesBool[rect->getX()+i][rect->getY()+j]==true)
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
                                tiles[prevRect->getX()+i][prevRect->getY()+j]->setBrush(QBrush(baseColors[prevRect->getX()+i][prevRect->getY()+j]));

                            }
                        }
                    }
                }
                if(rect->getX()+buildWidth<nbTiles && rect->getX()+buildHeight<nbTiles && !occuppiedTile)
                {

                    if(!occuppiedTile)
                    {
                        QColor color;
                        if(pickerBId!=-1)
                        {
                            switch(ConstantBuilding::get(pickerBId).getCategory()-1)
                            {
                            case -1:
                                color=Qt::darkGray;
                                break;
                            case 0:
                                color=Qt::yellow;
                                break;
                            case 1:
                                color=Qt::red;
                                break;
                            case 2:
                                color=Qt::green;
                                break;
                            case 3:
                                color=Qt::darkRed;
                                break;
                            case 4:
                                color=Qt::magenta;
                                break;
                            case 5:
                                color=Qt::cyan;
                                break;
                            case 6:
                                color=Qt::white;
                                break;
                            case 7:
                                color=Qt::lightGray;
                                break;
                            case 8:
                                color=Qt::darkMagenta;
                                break;

                            }

                            for(int i=0;i<buildWidth;i++)
                            {
                                for(int j=0;j<buildHeight;j++)
                                {
                                    tiles[rect->getX()+i][rect->getY()+j]->setBrush(color);
                                }
                            }

                            if(buildRadius>0) //if the selected building has an effect radius
                            {
                                for(int i=-buildRadius;i<buildRadius+buildWidth;i++)
                                {
                                    for(int j=-buildRadius;j<buildRadius+buildHeight;j++)
                                    {
                                        if(rect->getX()+i>=0 && rect->getY()+j>=0 && rect->getX()+i<nbTiles && rect->getY()+j<nbTiles)
                                        {
                                            tiles[rect->getX()+i][rect->getY()+j]->setPen(color);
                                            radiusTiles->append(tiles[rect->getX()+i][rect->getY()+j]);
                                        }
                                    }
                                }
                            }

                            if(prevRect!=rect && !road)
                                prevRect=rect;
                        }
                    }

                }
            }
        }
        else  //mode remove
        {

            foreach(MapTile* tile,*tempRemove)
            {
                tile->setBrush(QBrush(prevRemoveColor));
            }
            tempRemove->clear();

            if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos())))
            {
                if(rect->getBId()!=-10)
                {
                    prevRemoveColor=rect->brush().color();
                    for(int i=0;i<rect->getLargeurBat();i++)
                    {
                        for(int j=0;j<rect->getHauteurBat();j++)
                        {
                            tempRemove->append(tiles[rect->getMainTileX()+i][ rect->getMainTileY()+j]);
                            tiles[rect->getMainTileX()+i][ rect->getMainTileY()+j]->setBrush(QBrush(Qt::black));
                        }
                    }
                }
            }
        }
    }

    if(road) //not a else if because the road boolean is affected in the code above
    {
        if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos())))
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

                if(rect->getX()>roadStartX &&roadDir==1 && rect->getY()==roadStartY || rect->getX()<roadStartX &&roadDir==-1&& rect->getY()==roadStartY || rect->getY()>roadStartY && roadDir==2 && rect->getX()==roadStartX|| rect->getY()<roadStartY && roadDir==-2&& rect->getX()==roadStartX)
                {
                    int tileDistanceX=0;
                    int tileDistanceY=0;
                    if(prevRect!=nullptr){
                        tileDistanceX=(int)qFabs((double)rect->getX() - (double)prevRect->getX());
                        tileDistanceY=(int)qFabs((double)rect->getY() - (double)prevRect->getY());

                    }
                    int multDirection=1;

                    if(tileDistanceX>1 || tileDistanceY>1){
                        if(rect->getX()<prevRect->getX() && (int)qFabs(roadDir)==1 || rect->getY()<prevRect->getY() && (int)qFabs(roadDir)==2)multDirection=-1;
                    }

                    if(tileDistanceX > 1 && (int)qFabs(roadDir)==1){ //if the mouse "jumped" more than one tile in X

                        for(int i=0;i<tileDistanceX;i++){
                            if(!tilesBool[prevRect->getX()+i*multDirection][prevRect->getY()]){
                                tiles[prevRect->getX()+i*multDirection][prevRect->getY()]->setBrush(QBrush(Qt::darkGray));
                                tempRoad->append(tiles[prevRect->getX()+i*multDirection][prevRect->getY()]);
                            }
                        }
                    }else if(tileDistanceY > 1 && (int)qFabs(roadDir)==2){ //same thing but in y

                        for(int i=0;i<tileDistanceY;i++){
                            if(!tilesBool[prevRect->getX()][prevRect->getY()+i*multDirection]){
                                tiles[prevRect->getX()][prevRect->getY()+i*multDirection]->setBrush(QBrush(Qt::darkGray));
                                tempRoad->append(tiles[prevRect->getX()][prevRect->getY()+i*multDirection]);
                            }
                        }
                    }else{
                        if(!tilesBool[rect->getX()][rect->getY()]){
                            tempRoad->append(rect);
                            rect->setBrush(QBrush(Qt::darkGray));
                        }
                    }
                    if(prevRect!=rect)
                        prevRect=rect;
                }
            }
        }
    }
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::RightButton && (bPicker || road) )
    {
        if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos())))
        {
            bool caseOccupe=false;
            for(int i=0;i<buildWidth;i++)
            {
                for(int j=0;j<buildHeight;j++)
                {
                    if(tilesBool[rect->getX()+i][rect->getY()+j]==true)
                        caseOccupe=true;
                }
            }
            if(!caseOccupe){
                road=false;
                bPicker=false;
                foreach(MapTile* tile,*tempRoad)
                {
                    if(!grid)tile->setPen(QPen(Qt::transparent));
                    else tile->setPen(QPen(Qt::black));
                    tile->setBrush(QBrush(baseColors[tile->getX()][tile->getY()]));
                    tilesBool[tile->getX()][tile->getY()]=false;
                    tile->setLargeurBat(-10);
                    tile->setHauteurBat(-10);
                    tile->setMainTile(-10,-10);
                }

                tempRoad->clear();
                foreach(MapTile* tile,*radiusTiles)
                {
                    if(!grid)
                        tile->setPen(QPen(Qt::transparent));
                    else
                        tile->setPen(QPen(Qt::black));
                }

                radiusTiles->clear();

                if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos())))
                {
                    rect->setBrush(QBrush(baseColors[rect->getX()][rect->getY()]));
                    for(int i=0;i<buildWidth;i++)
                    {
                        for(int j=0;j<buildHeight;j++)
                        {
                            if(rect->getX()+i<nbTiles && rect->getY()+j<nbTiles)
                            {
                                tiles[rect->getX()+i][rect->getY()+j]->setBrush(QBrush(baseColors[rect->getX()+i][rect->getY()+j]));
                            }
                        }
                    }
                }
            }
        }
    }

    if(road){
        road=false;
        if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos()))){
            if(rect->getX()>roadStartX &&roadDir==1 && rect->getY()==roadStartY || rect->getX()<roadStartX &&roadDir==-1&& rect->getY()==roadStartY || rect->getY()>roadStartY && roadDir==2 && rect->getX()==roadStartX|| rect->getY()<roadStartY && roadDir==-2&& rect->getX()==roadStartX){
                tempRoad->append(rect);

                rect->setBrush(QBrush(Qt::darkGray));

                foreach(MapTile* tile, *tempRoad){

                    tilesBool[tile->getX()][tile->getY()]=true;
                    tile->setBId(0);

                    tile->setMainTile(roadStartX,roadStartY);
                    if(roadDir==1){

                        tile->setLargeurBat(rect->getX()-roadStartX+1);
                        tile->setHauteurBat(1);

                    }else if(roadDir==-1){

                        tile->setLargeurBat(roadStartX-rect->getX()+1);
                        tile->setHauteurBat(1);
                        tile->setMainTile(rect->getX(),rect->getY());
                        rect->setMainTile(rect->getX(),rect->getY());
                    }else if(roadDir==2){

                        tile->setLargeurBat(1);
                        tile->setHauteurBat(rect->getY()-roadStartY+1);

                    }else if(roadDir==-2){

                        tile->setLargeurBat(1);
                        tile->setHauteurBat(roadStartY-rect->getY()+1);
                        tile->setMainTile(rect->getX(),rect->getY());
                        rect->setMainTile(rect->getX(),rect->getY());
                    }

                }
                tempRoad->clear();
            }else{
                foreach(MapTile* tile,*tempRoad){
                    if(!grid)tile->setPen(QPen(Qt::transparent));
                    else tile->setPen(QPen(Qt::black));
                    tile->setBrush(QBrush(baseColors[tile->getX()][tile->getY()]));
                    tilesBool[tile->getX()][tile->getY()]=false;
                    tile->setLargeurBat(-10);
                    tile->setHauteurBat(-10);
                    tile->setMainTile(-10,-10);
                }

                tempRoad->clear();
            }
        }
    }else{

        if(bPicker && !road){
            bPicker=false;
            if(pickerBId!=-1){

                if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos()))){

                    bool caseOccupe=false;
                    for(int i=0;i<buildWidth;i++){
                        for(int j=0;j<buildHeight;j++){
                            if(tilesBool[rect->getX()+i][rect->getY()+j]==true)caseOccupe=true;
                        }
                    }

                    if(rect->getX()+buildWidth<nbTiles && rect->getX()+buildHeight<nbTiles && caseOccupe==false){
                        if(pickerBId==0 && !road){
                            road=true;
                            roadDir=0;
                            roadStartX=rect->getX();
                            roadStartY=rect->getY();
                            tempRoad->append(rect);
                        }
                        prevRect=nullptr;
                        if(checkIfNearRoad(rect) || road ){

                            //TODO: set the real image of the building instead of colorizing the tiles => switch on the buildingId instead of the category
                            switch(ConstantBuilding::get(pickerBId).getCategory()-1){
                            case -1:
                                rect->setBrush(Qt::darkGray);
                                break;
                            case 0:
                                rect->setBrush(Qt::yellow);
                                break;
                            case 1:
                                rect->setBrush(Qt::red);
                                break;
                            case 2:
                                rect->setBrush(Qt::green);
                                break;
                            case 3:
                                rect->setBrush(Qt::darkRed);
                                break;
                            case 4:
                                rect->setBrush(Qt::magenta);
                                break;
                            case 5:
                                rect->setBrush(Qt::cyan);
                                break;
                            case 6:
                                rect->setBrush(Qt::white);
                                break;
                            case 7:
                                rect->setBrush(Qt::lightGray);
                                break;
                            case 8:
                                rect->setBrush(Qt::darkMagenta);
                                break;

                            }
                            int mainTileX=rect->getX();
                            int mainTileY=rect->getY();
                            if(pickerBId!=0){
                                buildingCount++;
                                for(int i=0;i<buildWidth;i++){
                                    for(int j=0;j<buildHeight;j++){
                                        tilesBool[rect->getX()+i][rect->getY()+j]=true;
                                        tiles[rect->getX()+i][rect->getY()+j]->setBId(pickerBId);
                                        tiles[rect->getX()+i][rect->getY()+j]->setMainTile(mainTileX,mainTileY);
                                        tiles[rect->getX()+i][rect->getY()+j]->setLargeurBat(buildWidth);
                                        tiles[rect->getX()+i][rect->getY()+j]->setHauteurBat(buildHeight);
                                        tiles[rect->getX()+i][rect->getY()+j]->setUniqueBId(buildingCount);

                                    }
                                }
                                BuildingManagementService::getBuildingManagementService()->addBuilding(pickerBId,mainTileX,mainTileY, 0); //TODO : set 0 to 3 angles
                            }
                        }else if(!road){


                            for(int i=0;i<buildWidth;i++){
                                for(int j=0;j<buildHeight;j++){
                                    if(rect->getX()+i<nbTiles && rect->getY()+j<nbTiles){
                                        tiles[rect->getX()+i][rect->getY()+j]->setBrush(QBrush(baseColors[rect->getX()+i][rect->getY()+j]));

                                    }
                                }
                            }
                            blinkTileRed(rect);
                        }
                        foreach(MapTile* tile,*radiusTiles){
                            if(!grid)tile->setPen(QPen(Qt::transparent));
                            else tile->setPen(QPen(Qt::black));
                        }

                        radiusTiles->clear();


                    }

                }
            }else{
                //REMOVE MODE
                if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos()))){
                    if(rect->getBId()!=-10){ //if it is a default tile (no building/road on it)
                        if(rect->getUniqueBId()!=-10)BuildingManagementService::getBuildingManagementService()->removeBuilding(rect->getUniqueBId());
                        for(int i=0;i<rect->getLargeurBat();i++){
                            for(int j=0;j<rect->getHauteurBat();j++){

                                tilesBool[rect->getMainTileX()+i][rect->getMainTileY()+j]=false;
                                tiles[rect->getMainTileX()+i][rect->getMainTileY()+j]->setBId(-10);

                                tiles[rect->getMainTileX()+i][rect->getMainTileY()+j]->setUniqueBId(-10);
                                tiles[rect->getMainTileX()+i][rect->getMainTileY()+j]->setBrush(QBrush(baseColors[rect->getMainTileX()+i][rect->getMainTileY()+j]));


                            }
                        }
                        tempRemove->clear();
                    }
                }
            }
        }
    }

}

void MapView::keyPressEvent(QKeyEvent *event){
    GraphicService::getGraphicService()->setKeyboardShortcuts(event->key());


}

void MapView::toggleGrid(){
    for(int i=0;i<nbTiles;i++){
        for(int j=0;j<nbTiles;j++){
            if(grid){
                tiles[i][j]->setPen(QPen(Qt::transparent));
            }else if(!grid){
                tiles[i][j]->setPen(QPen(Qt::black));
            }
        }
    }
    grid=!grid;
}


//true: zoom in false: zoom out
void MapView::zoomMeth(bool plusMinus)
{
    if(plusMinus){
        if(zoom<1.25){
            zoom+=0.05;
            this->scale(2,2);
        }
    }else{
        if(zoom>0.95){
            zoom-=0.05;
            this->scale(0.5,0.5);
        }
    }
}


void MapView::removeBuildingMode(){
    if(!bPicker){
        MapView::getMapView()->callPicker(-1);
    }
}

void MapView::addRoadMode(){
    if(!bPicker){
        MapView::getMapView()->callPicker(0);
    }
}


//1: right -1: left 2: up -2: down
void MapView::translateMeth(int direction){
    if(direction==-2)this->translate(0,-15-35/(zoom*2));
    if(direction==2) this->translate(0,15+35/(zoom*2));
    if(direction==1)this->translate(-15-35/(zoom*2),0);
    if(direction==-1)this->translate(15+35/(zoom*2),0);
}


bool MapView::checkIfNearRoad(MapTile* tile)
{
    for(int i=0; i < buildWidth; i++)
    {
        for(int j=0; j < buildHeight; j++)
        {
            if(tile->getX()-1+i>=0 && tiles[tile->getX()-1+i][tile->getY()+j]->getBId()==0)return true;
            if(tile->getX()+1+i<nbTiles && tiles[tile->getX()+1+i][tile->getY()+j]->getBId()==0)return true;
            if(tile->getY()-1+j>=0 && tiles[tile->getX()+i][tile->getY()-1+j]->getBId()==0)return true;
            if(tile->getY()+1+j<nbTiles && tiles[tile->getX()+i][tile->getY()+1+j]->getBId()==0)return true;
        }
    }
    return false;
}

void MapView::blinkTileRed(MapTile* tile)
{
    blinkRedTile = tile;
    timer->start();
}

void MapView::blinkRedTileSlot()
{
    if(nbTimeTick<6)
    {
        nbTimeTick++;
        for(int i=0;i<buildWidth;i++)
        {
            for(int j=0;j<buildHeight;j++)
            {
                if(timeTick)
                {
                    tiles[blinkRedTile->getX()+i][blinkRedTile->getY()+j]->setPen(QPen(Qt::red));
                }
                else
                {
                    if(grid)
                        tiles[blinkRedTile->getX()+i][blinkRedTile->getY()+j]->setPen(QPen(Qt::black));
                    else
                        tiles[blinkRedTile->getX()+i][blinkRedTile->getY()+j]->setPen(QPen(Qt::transparent));
                }
            }
        }
        timeTick=!timeTick;
    }
    else
    {
        nbTimeTick=0;
        timer->stop();
        for(int i=0;i<buildWidth;i++)
        {
            for(int j=0;j<buildHeight;j++)
            {
                if(grid)
                    tiles[blinkRedTile->getX()+i][blinkRedTile->getY()+j]->setPen(QPen(Qt::black));
                else
                    tiles[blinkRedTile->getX()+i][blinkRedTile->getY()+j]->setPen(QPen(Qt::transparent));
            }
        }
    }
}

