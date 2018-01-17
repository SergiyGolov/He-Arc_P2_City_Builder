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

    currentBuild=nullptr;
    nbTimeTick=0;
    timeTick=false;
    roadStartX=0;
    roadStartY=0;
    bRoad=false;
    bPicker=false;
    bClick=false;
    bGrid=false;
    pickerBId=0;
    cameraX=0;
    cameraY=0;
    prevRect=nullptr;
    roadDir=0;
    lastTilePix=nullptr;


    //the purpose of this timer is to blink in red if the user try to add a building without an adjacent road
    timer=new QTimer(this);
    timer->setInterval(100);
    connect(timer,SIGNAL(timeout()),this,SLOT(blinkRedTileSlot()));

    this->setBackgroundBrush(QBrush(Qt::black));
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
    buildPixList=new QList<QGraphicsPixmapItem*>();
    neighbourList=new QList<MapTile*>();

    nbTiles=256;
    lastbId=-1;
    circleWidth=1;
    tiles=new QVector<MapTile*>();
    setTransformationAnchor(QGraphicsView::NoAnchor);

    srand(time(NULL));
    int seed = rand() ;
    RandomService::setSeed(seed);


}

void MapView::generateMap(){

    this->resetTransform();
    scene->clear();
    tiles->clear();
    tempRoadList->clear();
    tempRemoveList->clear();
    buildPixList->clear();
    neighbourList->clear();

    radiusCircle=new QGraphicsEllipseItem(0,0,1,1);
    radiusCircle->setVisible(false);
    radiusCircle->setZValue(5);
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


    //    QGraphicsRectItem *darkDirt=new QGraphicsRectItem(0,screenHeight*1.0075,screenWidth*0.5675,screenHeight*1.4);
    //    scene->addItem(darkDirt);
    //    darkDirt->setBrush(QBrush(QColor::fromRgb(40,26,13)));
    //    darkDirt->setRotation(-45);

    //    QGraphicsRectItem *lightDirt=new QGraphicsRectItem(-screenWidth*0.5675,screenHeight*1.0075,screenWidth*0.5675,screenHeight*1.4);
    //    scene->addItem(lightDirt);
    //    lightDirt->setBrush(QBrush(QColor::fromRgb(153,101,21)));
    //    lightDirt->setRotation(-45);

    translate(screenWidth/2,0);


    scale(1,0.5);
    rotate(45);


this->translate(cameraX,cameraY);

    if(zoomFactor<1){
        this->scale(0.5,0.5);
    }else if(zoomFactor>1){
        for(int i=0;i<(zoomFactor-1)/0.05;i++){
            this->scale(2,2);
        }
    }

    if(nbTiles>243)translateFactor=5.35*(double)pixelPerTile*(double)nbTiles/256.0;
    else translateFactor=4.275*(double)pixelPerTile*(double)nbTiles/256.0;

    if(nbTiles>243)scaleFactor=0.19*256.0/(double)nbTiles;
    else scaleFactor=0.235*256.0/(double)nbTiles;

}

MapView::~MapView()
{

}

void MapView::callPicker(int bId){

    if(!bPicker){
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
        if(zoomFactor>1)
        {
            zoomFactor-=0.05;
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

    QList<QGraphicsItem *> itemsList=items(event->pos());
    if(!itemsList.isEmpty()){
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

void MapView::mousePressEvent(QMouseEvent *event)
{

    QList<QGraphicsItem *> itemsList=items(event->pos());
    if(!itemsList.isEmpty()){
        if(MapTile *rect=dynamic_cast<MapTile*>(itemsList.last()))
        {
            if (event->button()==Qt::RightButton && (bPicker || bRoad) )
            {
                cancelAdd(rect);
            }else if(event->button()==Qt::LeftButton){
                if(bRoad){
                    finalAddRoad(rect);
                }else if(bPicker){
                    bPicker=false;
                    if(pickerBId!=-1){
                        finalAddBuilding(rect);
                    }else{
                        finalRemove(rect);
                    }
                }
            }
        }
    }
}




void MapView::keyPressEvent(QKeyEvent *event){
    GraphicService::getGraphicService()->setKeyboardShortcuts(event->key());


}

void MapView::moveRemoveBuilding(MapTile *rect){
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
                if( tiles->at((rect->getMainTileX()+i)+( rect->getMainTileY()+j)*nbTiles)->getBId()==rect->getBId()){
                    tempRemoveList->append(tiles->at((rect->getMainTileX()+i)+( rect->getMainTileY()+j)*nbTiles));
                    tiles->at((rect->getMainTileX()+i)+( rect->getMainTileY()+j)*nbTiles)->setBrush(QBrush(Qt::black));
                    tiles->at((rect->getMainTileX()+i)+( rect->getMainTileY()+j)*nbTiles)->setPixOpacity(0.65);
                }
            }
        }
    }


}

void MapView::moveAddBuilding(MapTile *rect){



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


        QColor color;


        QString pixFilePath="NOPIX";
        bool pixExists=false;

        switch(pickerBId)
        {
        case 1:
            pixFilePath=":/ressources/house.png";
            break;
        }

        if(pixFilePath!="NOPIX")pixExists=true;

        if(currentBuild==nullptr && pixExists){
            QTransform trans;

            trans.scale(1.1*0.04*256/nbTiles,1.2*0.04*256/nbTiles);
            trans.rotate(-45);
            currentBuild=new QGraphicsPixmapItem( QPixmap(pixFilePath));

            //currentBuild->setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
            currentBuild->setOpacity(0.65);
            //currentBuild->setScale(0.04);


            currentBuild->setZValue(2);
            //currentBuild->setRotation(-45);
            currentBuild->setTransform(trans);

            scene->addItem(currentBuild);

        }
        if(prevRect!=nullptr && prevRect!=rect && pixExists)prevRect->removePixMove();
        if(prevRect!=rect && pixExists){
            rect->addPixMove(currentBuild);
            lastTilePix=rect;
        }



        if(currentBuild==nullptr){
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
        }else{
            color=baseColors->at((rect->getX())+(rect->getY())*nbTiles);
        }


        for(int i=0;i<buildWidth;i++)
        {
            for(int j=0;j<buildHeight;j++)
            {
                tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->setBrush(color);
            }
        }

        if(buildRadius>0 && rect!=prevRect) //if the selected building has an effect radius
        {

            radiusCircle->setVisible(true);
            radiusCircle->setRect(rect->pos().x()-circleWidth/2,rect->pos().y()-circleWidth/2,circleWidth,circleWidth);
            if(lastbId!=pickerBId){
                int buildSizeOffset=buildHeight;
                if(buildWidth>buildHeight)buildSizeOffset=buildWidth;
                circleWidth=(buildRadius+buildSizeOffset-3)*pixelPerTile*2;


                radiusCircle->setPen(QPen(color));
                radiusCircle->setBrush(QBrush(color));
            }

            //            QRadialGradient radialGrad;

            //               radialGrad.setColorAt(0, color);

            //               radialGrad.setColorAt(1, Qt::white);

            //               radialGrad.setCenter(mapFromScene(rect->pos().toPoint()));

            //               radialGrad.setRadius(circleWidth);



            // radiusCircle->setBrush(radialGrad);

            //radiusCircle->setOpacity(0.5);

            //            for(int i=-buildRadius;i<buildRadius+buildWidth;i++)
            //            {
            //                for(int j=-buildRadius;j<buildRadius+buildHeight;j++)
            //                {
            //                    if(rect->getX()+i>=0 && rect->getY()+j>=0 && rect->getX()+i<nbTiles && rect->getY()+j<nbTiles)
            //                    {
            //                        tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->setPen(color);
            //                        radiusTilesList->append(tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles));
            //                    }
            //                }
            //            }

        }

        if(prevRect!=rect && !bRoad)
            prevRect=rect;




    }else if(lastTilePix!=nullptr){

        lastTilePix->removePixMove();
        lastTilePix=nullptr;
    }else{
        radiusCircle->setVisible(false);
    }

    lastbId=pickerBId;
}

void MapView::moveAddRoad(MapTile* rect){
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
            if(prevRect!=nullptr){
                tileDistanceX=(int)qFabs((double)rect->getX() - (double)prevRect->getX());
                tileDistanceY=(int)qFabs((double)rect->getY() - (double)prevRect->getY());

            }
            int multDirection=1;

            if(tileDistanceX>1 || tileDistanceY>1){
                if((rect->getX()<prevRect->getX() && dir==1 )|| (rect->getY()<prevRect->getY() && (int)qFabs(roadDir)==2))multDirection=-1;
            }

            if(tileDistanceX > 1 && dir==1){ //if the mouse "jumped" more than one tile in X

                for(int i=0;i<tileDistanceX+1;i++){
                    if(!tiles->at((prevRect->getX()+i*multDirection)+(prevRect->getY())*nbTiles)->isOccupied()){
                        tiles->at((prevRect->getX()+i*multDirection)+(prevRect->getY())*nbTiles)->setBrush(QBrush(Qt::darkGray));

                        tempRoadList->append(tiles->at((prevRect->getX()+i*multDirection)+(prevRect->getY())*nbTiles));
                    }
                }
            }else if(tileDistanceY > 1 && dir==2){ //same thing but in y

                for(int i=0;i<tileDistanceY+1;i++){
                    if(!tiles->at((prevRect->getX())+(prevRect->getY()+i*multDirection)*nbTiles)->isOccupied()){
                        tiles->at((prevRect->getX())+(prevRect->getY()+i*multDirection)*nbTiles)->setBrush(QBrush(Qt::darkGray));

                        tempRoadList->append(tiles->at((prevRect->getX())+(prevRect->getY()+i*multDirection)*nbTiles));
                    }
                }
            }else if(!rect->isOccupied()){
                tempRoadList->append(rect);
                rect->setBrush(QBrush(Qt::darkGray));


            }

            prevRect=rect;
        }

    }
}

void MapView::finalRemove(MapTile* rect){
    int initialBid=rect->getBId();
    if(initialBid!=-10){ //if it is not a default tile (no building/road on it)
        if(rect->getUniqueBId()!=-10)BuildingManagementService::getBuildingManagementService()->removeBuilding(rect->getUniqueBId());

        for(int i=0;i<rect->getBuildingWidth();i++){
            for(int j=0;j<rect->getBuildingHeight();j++){
                if( tiles->at((rect->getMainTileX()+i)+(rect->getMainTileY()+j)*nbTiles)->getBId()==initialBid){
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

void MapView::finalAddBuilding(MapTile* rect){
    bool caseOccupe=false;
    for(int i=0;i<buildWidth;i++){
        for(int j=0;j<buildHeight;j++){
            if(tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->isOccupied())caseOccupe=true;
        }
    }

    if(rect->getX()+buildWidth-1<nbTiles && rect->getX()+buildHeight-1<nbTiles && caseOccupe==false){
        if(pickerBId==0 && !bRoad){
            bRoad=true;
            roadDir=0;
            roadStartX=rect->getX();
            roadStartY=rect->getY();
            tempRoadList->append(rect);

        }
        prevRect=nullptr;
        if(checkIfNearRoad(rect) || bRoad ){
            QColor color;

            //TODO: set the real image of the building instead of colorizing the tiles => switch on the buildingId instead of the category
            if(currentBuild==nullptr){
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
            }else{
                color=baseColors->at((rect->getX())+(rect->getY())*nbTiles);
            }
            lastTilePix=nullptr;
            rect->setBrush(color);
            int mainTileX=rect->getX();
            int mainTileY=rect->getY();

            if(currentBuild!=nullptr){
                rect->addPix(currentBuild);
                currentBuild=nullptr;
            }

            if(pickerBId!=0){
                buildingCount++;
                for(int i=0;i<buildWidth;i++){
                    for(int j=0;j<buildHeight;j++){
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
        }else if(!bRoad){


            for(int i=0;i<buildWidth;i++){
                for(int j=0;j<buildHeight;j++){
                    if(rect->getX()+i<nbTiles && rect->getY()+j<nbTiles){
                        tiles->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)->setBrush(QBrush(baseColors->at((rect->getX()+i)+(rect->getY()+j)*nbTiles)));

                    }
                }
            }
            blinkTileRed(rect);
        }
        radiusCircle->setVisible(false);


    }

}

void MapView::finalAddRoad(MapTile* rect){

    bRoad=false;
    int dir=(int)qFabs(roadDir);

    if(rect->getBId()!=0){
        if(((rect->getX()>roadStartX &&roadDir==1 && rect->getY()==roadStartY || rect->getX()<roadStartX &&roadDir==-1&& rect->getY()==roadStartY || rect->getY()>roadStartY && roadDir==2 && rect->getX()==roadStartX|| rect->getY()<roadStartY && roadDir==-2&& rect->getX()==roadStartX))){

            int lastX=-1;
            int lastY=-1;

            int aNeighbourCount[tempRoadList->count()];
            bool bNeighbour=false;
            foreach(MapTile* tile, *tempRoadList){
                int neighbourCount=countNeighbourRoads(tile);
                aNeighbourCount[tempRoadList->indexOf(tile)]=neighbourCount;
                if(neighbourCount>0)bNeighbour=true;
            }

            foreach(MapTile* tile, *tempRoadList){
                if(tile->getBId()==-10 && tile->getX()!=lastX && dir==1 || tile->getY()!=lastY && dir==2){

                    tile->setOccupied(true);
                    tile->setBId(0);

                    tile->setMainTile(roadStartX,roadStartY);
                    if(roadDir==1){

                        tile->setBuildingWidth(rect->getX()-roadStartX+1);
                        tile->setBuildingHeight(1);

                    }else if(roadDir==-1){

                        tile->setBuildingWidth(roadStartX-rect->getX()+1);
                        tile->setBuildingHeight(1);
                        tile->setMainTile(rect->getX(),rect->getY());
                        rect->setMainTile(rect->getX(),rect->getY());
                    }else if(roadDir==2){

                        tile->setBuildingWidth(1);
                        tile->setBuildingHeight(rect->getY()-roadStartY+1);

                    }else if(roadDir==-2){

                        tile->setBuildingWidth(1);
                        tile->setBuildingHeight(roadStartY-rect->getY()+1);
                        tile->setMainTile(rect->getX(),rect->getY());
                        rect->setMainTile(rect->getX(),rect->getY());
                    }


                    QString pixFilePath=":/ressources/route.png";


                    QTransform trans;
                    trans.scale(scaleFactor,scaleFactor);

                    int angle;
                    if(dir==1 ){
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

                    currentBuild->setZValue(2);
                    currentBuild->setTransform(trans);
                    scene->addItem(currentBuild);
                    tile->addPixRoad(currentBuild);
                    currentBuild=nullptr;
                    tile->setBrush(QBrush(baseColors->at((tile->getX())+(tile->getY())*nbTiles)));
                    lastX=tile->getX();
                    lastY=tile->getY();
                }

            }

            if(bNeighbour){
                for(int i=0;i<tempRoadList->count();i++){
                    if(aNeighbourCount[i]>0){
                        updateNeighbourRoad(tempRoadList->at(i));
                    }
                }
            }


            tempRoadList->clear();

        }else{

            blinkTileRed(rect);
            cancelAdd(rect);
        }

    }else{
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

void MapView::cancelAdd(MapTile* rect){

    if(currentBuild!=nullptr){
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
    if(!occupiedTile || falseRoadAdd){
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



void MapView::toggleGrid(){
    for(int i=0;i<nbTiles;i++){
        for(int j=0;j<nbTiles;j++){
            if(bGrid){
                tiles->at((i)+(j)*nbTiles)->setPen(QPen(Qt::transparent));
            }else if(!bGrid){
                tiles->at((i)+(j)*nbTiles)->setPen(QPen(Qt::black));
            }
        }
    }
    bGrid=!bGrid;
}


//true: zoom in false: zoom out
void MapView::zoomMeth(bool plusMinus)
{
    if(plusMinus){
        if(zoomFactor<1.25){
            zoomFactor+=0.05;
            this->scale(2,2);
        }
    }else{
        if(zoomFactor>0.95){
            zoomFactor-=0.05;
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
    int zoomFactor=5;
    switch(direction){
    case -2:
        this->translate(-15/zoomFactor-35/(zoomFactor*zoomFactor),-15/zoomFactor-35/(zoomFactor*zoomFactor));
        cameraX+=-15/zoomFactor-35/(zoomFactor*zoomFactor);
        cameraY+=-15/zoomFactor-35/(zoomFactor*zoomFactor);
        break;
    case 2:
        this->translate(15/zoomFactor+35/(zoomFactor*zoomFactor),15/zoomFactor+35/(zoomFactor*zoomFactor));
        cameraX+=15/zoomFactor+35/(zoomFactor*zoomFactor);
        cameraY+=15/zoomFactor+35/(zoomFactor*zoomFactor);
        break;
    case 1:
        this->translate(-15/zoomFactor-35/(zoomFactor*zoomFactor),15/zoomFactor+35/(zoomFactor*zoomFactor));
        cameraX+=-15/zoomFactor-35/(zoomFactor*zoomFactor);
        cameraY+=15/zoomFactor+35/(zoomFactor*zoomFactor);
        break;
    case -1:
        this->translate(15/zoomFactor+35/(zoomFactor*zoomFactor),-15/zoomFactor-35/(zoomFactor*zoomFactor));
        cameraX+=15/zoomFactor+35/(zoomFactor*zoomFactor);
        cameraY+=-15/zoomFactor-35/(zoomFactor*zoomFactor);
        break;
    }


}


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

//TO-DO: RETURN TILE CONTAINING NEIGHTBOUR ROAD TO CHANGE HIS PIXMAP OR NULLPTR IF NOT FOUND
bool MapView::checkIfNearRoadX(MapTile* tile)
{

    if(tile->getX()-1>=0 && tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0)return true;
    if(tile->getX()+1<nbTiles && tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0)return true;


    return false;
}






//TO-DO: RETURN TILE CONTAINING NEIGHTBOUR ROAD TO CHANGE HIS PIXMAP OR NULLPTR IF NOT FOUND
bool MapView::checkIfNearRoadY(MapTile* tile)
{

    if(tile->getY()-1>=0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0)return true;
    if(tile->getY()+1<nbTiles && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0)return true;


    return false;
}



void MapView::blinkTileRed(MapTile* tile)
{
    tile->removePixMove();
    currentBuild=nullptr;
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
                if(bGrid)
                    tiles->at((blinkRedTile->getX()+i)+(blinkRedTile->getY()+j)*nbTiles)->setPen(QPen(Qt::black));
                else
                    tiles->at((blinkRedTile->getX()+i)+(blinkRedTile->getY()+j)*nbTiles)->setPen(QPen(Qt::transparent));
            }
        }
    }
}

int MapView::countNeighbourRoads(MapTile* tile){
    int n=0;

    if(tile->getX()-1>=0 && tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0)n++;
    if(tile->getX()+1<nbTiles && tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0)n++;
    if(tile->getY()-1>=0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0)n++;
    if(tile->getY()+1<nbTiles && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0)n++;

    return n;
}



void MapView::getNeighbours(MapTile* tile){
    neighbourList->clear();
    if(tile->getX()-1>=0 && tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0)neighbourList->append(tiles->at((tile->getX()-1)+(tile->getY())*nbTiles));
    if(tile->getY()-1>=0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0)neighbourList->append(tiles->at((tile->getX())+(tile->getY()-1)*nbTiles));
    if(tile->getX()+1<nbTiles && tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0)neighbourList->append(tiles->at((tile->getX()+1)+(tile->getY())*nbTiles));
    if(tile->getY()+1<nbTiles && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0)neighbourList->append(tiles->at((tile->getX())+(tile->getY()+1)*nbTiles));
    neighbourList->append(tile);

}

void  MapView::updateNeighbourRoad(MapTile* tileIn){


    getNeighbours(tileIn);

    foreach(MapTile* tile,*neighbourList){

        int neighbourCount=countNeighbourRoads(tile);

        QString pixPath="NOPIX";
        QTransform trans;

        switch(neighbourCount){
        //"T"
        case 3:
            pixPath=":/ressources/routeT.png";
            BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),3);

            break;
            //crossroad
        case 4:

            pixPath=":/ressources/routeX.png";
            BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),2);

            break;
        }

        trans.scale(scaleFactor,scaleFactor);

        switch(neighbourCount){
        case 2:
            if(tile->getX()-1>=0 && tile->getY()+1<nbTiles && tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0){
                trans.rotate(-90);
                trans.translate(-translateFactor,0);
                pixPath=":/ressources/routeL.png";
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),7);
            }else if(tile->getX()+1<nbTiles && tile->getY()-1>=0 && tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0){
                trans.rotate(90);
                trans.translate(0,-translateFactor);
                pixPath=":/ressources/routeL.png";
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),8);
            }else if(tile->getX()+1<nbTiles && tile->getY()+1<nbTiles && tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0){
                trans.rotate(180);
                trans.translate(-translateFactor,-translateFactor);
                pixPath=":/ressources/routeL.png";
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),9);
            }else if(tile->getX()-1>=0 && tile->getY()-1>=0 && tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0){
                pixPath=":/ressources/routeL.png";
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),10);
            }
            break;

        case 3:
            if(tile->getX()-1 >=0 && tile->getY()-1>=0 && tile->getX()+1<nbTiles&&tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0){
                trans.rotate(180);
                trans.translate(-translateFactor,-translateFactor);
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),4);
            }
            if(tile->getX()+1 <nbTiles && tile->getY()-1>=0 && tile->getY()+1<nbTiles&&tiles->at((tile->getX()+1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0){
                trans.rotate(-90);
                trans.translate(-translateFactor,0);
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),5);
            }
            if(tile->getX()-1>=0 && tile->getY()-1>=0 && tile->getY()+1<nbTiles&&tiles->at((tile->getX()-1)+(tile->getY())*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()+1)*nbTiles)->getBId()==0 && tiles->at((tile->getX())+(tile->getY()-1)*nbTiles)->getBId()==0){
                trans.rotate(90);
                trans.translate(0,-translateFactor);
                BuildingManagementService::getBuildingManagementService()->setAngleFromId(tile->getUniqueBId(),6);
            }
            break;
        }

        if(pixPath!="NOPIX"){
            QGraphicsPixmapItem *roadPix=new QGraphicsPixmapItem(QPixmap(pixPath));
            scene->addItem(roadPix);
            tile->removePix();

            roadPix->setTransform(trans);
            roadPix->setZValue(2);
            tile->addPixRoad(roadPix);
        }

    }
}

void MapView::addBuildingFromSave(int id, int x, int y, int angle){
    int buildWidthSave=ConstantBuilding::get(id).getTileWidth();
    int buildHeightSave=ConstantBuilding::get(id).getTileHeight();


    if(id!=0){
        QColor color;
        switch(ConstantBuilding::get(id).getCategory()-1)
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

        buildingCount++;
        for(int i=0;i<buildWidthSave;i++){
            for(int j=0;j<buildHeightSave;j++){
                tiles->at((x+i)+(y+j)*nbTiles)->setOccupied(true);
                tiles->at((x+i)+(y+j)*nbTiles)->setBId(id);
                tiles->at((x+i)+(y+j)*nbTiles)->setMainTile(x,y);
                tiles->at((x+i)+(y+j)*nbTiles)->setBuildingWidth(buildWidthSave);
                tiles->at((x+i)+(y+j)*nbTiles)->setBuildingHeight(buildHeightSave);
                tiles->at((x+i)+(y+j)*nbTiles)->setUniqueBId(buildingCount);
                tiles->at((x+i)+(y+j)*nbTiles)->setBrush(color);
            }
        }
    }else{
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

        switch(angle){
        case 1:
            trans.rotate(-90);
            trans.translate(-translateFactor,0);
        case 0:
            pixPath=":/ressources/route.png";
            break;
        case 2:
            pixPath=":/ressources/routeX.png";
            break;
        case 4:
            trans.rotate(180);
            trans.translate(-translateFactor,-translateFactor);
        case 3:
            pixPath=":/ressources/routeT.png";
            break;
        case 5:
            trans.rotate(-90);
            trans.translate(-translateFactor,0);
            pixPath=":/ressources/routeT.png";
            break;
        case 6:
            trans.rotate(90);
            trans.translate(0,-translateFactor);
            pixPath=":/ressources/routeT.png";
            break;
        case 7:
            trans.rotate(-90);
            trans.translate(-translateFactor,0);
            pixPath=":/ressources/routeL.png";
            break;
        case 8:
            trans.rotate(90);
            trans.translate(0,-translateFactor);
            pixPath=":/ressources/routeL.png";
            break;
        case 9:
            trans.rotate(180);
            trans.translate(-translateFactor,-translateFactor);
        case 10:
            pixPath=":/ressources/routeL.png";
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
