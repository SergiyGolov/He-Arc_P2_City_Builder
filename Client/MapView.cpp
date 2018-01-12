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



//TO-DO: use perlinNoise tu generate background with white clouds on blue background
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
    click=false;
    grid=false;
    pickerBId=0;
    this->gui=GuiView::getGuiView();
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

    zoom=1;
    buildingCount=0;
    this->setMouseTracking(true);
    screenWidth=QApplication::desktop()->screenGeometry().width();
    screenHeight=QApplication::desktop()->screenGeometry().height();

    this->setGeometry(0,0,screenWidth,screenHeight-screenHeight/10);

    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );


    radiusTilesList=new QList<MapTile*>();
    tempRoadList=new QList<MapTile*>();
    tempRemoveList=new QList<MapTile*>();
    buildPixList=new QList<QGraphicsPixmapItem*>();
    neighbourList=new QList<MapTile*>();

    int tileNb = nbTiles;

    pixelPerTile=((screenHeight-screenHeight/10)/tileNb)*2; //*qSqrt(2)




    setTransformationAnchor ( QGraphicsView::NoAnchor );
    srand(time(NULL));
    int seed = rand() ;
    //int seed=666;
    cells = RandomService::generateMap(seed, tileNb, tileNb);

    for(int i=0;i<tileNb;i++)
    {
        for(int j=0;j<tileNb;j++)
        {

            int coorX=i*pixelPerTile;
            int coorY=j*pixelPerTile;


            MapTile *tile=new MapTile(i,j,0,0,pixelPerTile,pixelPerTile);
            tile->setPen(QPen(Qt::transparent));
            tile->setPos(coorX,coorY);

            // tile->setBrush(QBrush(Qt::darkGreen));
            tile->setBrush(QBrush(cells[i+j*tileNb].color));

            scene->addItem(tile);

            if(cells[i+j*tileNb].color.blue()>45)tile->setOccupied(true); //to prevent from building on water
            else tile->setOccupied(false);
            tiles[i][j]=tile;
            baseColors[i][j]=cells[i+j*tileNb].color;



            //tile->setRotation(45);



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
                tempRemoveList->append(tiles[rect->getMainTileX()+i][ rect->getMainTileY()+j]);
                tiles[rect->getMainTileX()+i][ rect->getMainTileY()+j]->setBrush(QBrush(Qt::black));
                tiles[rect->getMainTileX()+i][ rect->getMainTileY()+j]->setPixOpacity(0.65);
            }
        }
    }


}

void MapView::moveAddBuilding(MapTile *rect){
    foreach(MapTile* tile,*radiusTilesList)
    {
        if(!grid)tile->setPen(QPen(Qt::transparent));
        else tile->setPen(QPen(Qt::black));
    }

    radiusTilesList->clear();
    bool occuppiedTile=false;
    for(int i=0;i<buildWidth;i++)
    {
        for(int j=0;j<buildHeight;j++)
        {
            if(tiles[rect->getX()+i][rect->getY()+j]->isOccupied())
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

        if(pixFilePath!="NOPIX") pixExists=true;

        if(currentBuild==nullptr && pixExists){
            QTransform trans;

            trans.scale(1.1*0.04,1.2*0.04);
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
            color=baseColors[rect->getX()][rect->getY()];
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
                        radiusTilesList->append(tiles[rect->getX()+i][rect->getY()+j]);
                    }
                }
            }
        }

        if(prevRect!=rect && !bRoad)
            prevRect=rect;




    }else if(lastTilePix!=nullptr){

        lastTilePix->removePixMove();
        lastTilePix=nullptr;
    }
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





        if(rect != prevRect && (rect->getX()>roadStartX &&roadDir==1 && rect->getY()==roadStartY || rect->getX()<roadStartX &&roadDir==-1&& rect->getY()==roadStartY || rect->getY()>roadStartY && roadDir==2 && rect->getX()==roadStartX|| rect->getY()<roadStartY && roadDir==-2&& rect->getX()==roadStartX) && tempRoadList->indexOf(rect)==-1)
        {

            int tileDistanceX=0;
            int tileDistanceY=0;
            if(prevRect!=nullptr){
                //if(roadDir>0){
                tileDistanceX=(int)qFabs((double)rect->getX() - (double)prevRect->getX());
                tileDistanceY=(int)qFabs((double)rect->getY() - (double)prevRect->getY());
                //                            }else{
                //                                tileDistanceX=(int)qFabs((double)rect->getX() - (double)prevRect->getX()+1);
                //                                tileDistanceY=(int)qFabs((double)rect->getY() - (double)prevRect->getY()+1);
                //                            }

            }
            int multDirection=1;

            if(tileDistanceX>1 || tileDistanceY>1){
                if(rect->getX()<prevRect->getX() && (int)qFabs(roadDir)==1 || rect->getY()<prevRect->getY() && (int)qFabs(roadDir)==2)multDirection=-1;
            }

            if(tileDistanceX > 1 && (int)qFabs(roadDir)==1){ //if the mouse "jumped" more than one tile in X

                for(int i=0;i<tileDistanceX+1;i++){
                    if(!tiles[prevRect->getX()+i*multDirection][prevRect->getY()]->isOccupied()){
                        tiles[prevRect->getX()+i*multDirection][prevRect->getY()]->setBrush(QBrush(Qt::darkGray));

                        tempRoadList->append(tiles[prevRect->getX()+i*multDirection][prevRect->getY()]);
                    }
                }
            }else if(tileDistanceY > 1 && (int)qFabs(roadDir)==2){ //same thing but in y

                for(int i=0;i<tileDistanceY+1;i++){
                    if(!tiles[prevRect->getX()][prevRect->getY()+i*multDirection]->isOccupied()){
                        tiles[prevRect->getX()][prevRect->getY()+i*multDirection]->setBrush(QBrush(Qt::darkGray));

                        tempRoadList->append(tiles[prevRect->getX()][prevRect->getY()+i*multDirection]);
                    }
                }
            }else if(!rect->isOccupied()){
                tempRoadList->append(rect);
                rect->setBrush(QBrush(Qt::darkGray));


            }


        }
        prevRect=rect;
    }
}

void MapView::finalRemove(MapTile* rect){
    if(rect->getBId()!=-10){ //if it is not a default tile (no building/road on it)
        if(rect->getUniqueBId()!=-10 && rect->getUniqueBId()!=0)BuildingManagementService::getBuildingManagementService()->removeBuilding(rect->getUniqueBId());

        for(int i=0;i<rect->getBuildingWidth();i++){
            for(int j=0;j<rect->getBuildingHeight();j++){
                tiles[rect->getMainTileX()+i][rect->getMainTileY()+j]->removePix();
                tiles[rect->getMainTileX()+i][rect->getMainTileY()+j]->setOccupied(false);
                tiles[rect->getMainTileX()+i][rect->getMainTileY()+j]->setBId(-10);

                tiles[rect->getMainTileX()+i][rect->getMainTileY()+j]->setUniqueBId(-10);
                tiles[rect->getMainTileX()+i][rect->getMainTileY()+j]->setBrush(QBrush(baseColors[rect->getMainTileX()+i][rect->getMainTileY()+j]));

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
            if(tiles[rect->getX()+i][rect->getY()+j]->isOccupied())caseOccupe=true;
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
                color=baseColors[rect->getX()][rect->getY()];
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
                        tiles[rect->getX()+i][rect->getY()+j]->setOccupied(true);
                        tiles[rect->getX()+i][rect->getY()+j]->setBId(pickerBId);
                        tiles[rect->getX()+i][rect->getY()+j]->setMainTile(mainTileX,mainTileY);
                        tiles[rect->getX()+i][rect->getY()+j]->setBuildingWidth(buildWidth);
                        tiles[rect->getX()+i][rect->getY()+j]->setBuildingHeight(buildHeight);
                        tiles[rect->getX()+i][rect->getY()+j]->setUniqueBId(buildingCount);

                    }
                }
                BuildingManagementService::getBuildingManagementService()->addBuilding(pickerBId,mainTileX,mainTileY, 0); //TODO : set 0 to 3 angles
            }
        }else if(!bRoad){


            for(int i=0;i<buildWidth;i++){
                for(int j=0;j<buildHeight;j++){
                    if(rect->getX()+i<nbTiles && rect->getY()+j<nbTiles){
                        tiles[rect->getX()+i][rect->getY()+j]->setBrush(QBrush(baseColors[rect->getX()+i][rect->getY()+j]));

                    }
                }
            }
            blinkTileRed(rect);
        }
        foreach(MapTile* tile,*radiusTilesList){
            if(!grid)tile->setPen(QPen(Qt::transparent));
            else tile->setPen(QPen(Qt::black));
        }

        radiusTilesList->clear();


    }

}

void MapView::finalAddRoad(MapTile* rect){

    bRoad=false;
    int dir=(int)qFabs(roadDir);

    if(rect->getBId()!=0){
        if(((rect->getX()>roadStartX &&roadDir==1 && rect->getY()==roadStartY || rect->getX()<roadStartX &&roadDir==-1&& rect->getY()==roadStartY || rect->getY()>roadStartY && roadDir==2 && rect->getX()==roadStartX|| rect->getY()<roadStartY && roadDir==-2&& rect->getX()==roadStartX))){



            //if(roadDir==-1 || roadDir==-2)std::reverse(tempRoadList->begin(), tempRoadList->end());
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
                if(tile->getX()!=lastX && dir==1 || tile->getY()!=lastY && dir==2){




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





                    //if((dir==2 && checkIfNearRoadX(tile)) || (dir==1 && checkIfNearRoadY(tile))) pixFilePath=":/ressources/roadCross.png";



                    QTransform trans;

                    if(dir==1 ){
                        //trans.scale(0.19,0.175);
                        trans.scale(0.19,0.19);

                    }
                    else {
                        //trans.scale(0.175,0.19);
                        trans.scale(0.19,0.19);
                        trans.rotate(-90);
                        trans.translate(-5.4*pixelPerTile,0);
                    }
                    currentBuild=new QGraphicsPixmapItem(QPixmap(pixFilePath));


                    currentBuild->setZValue(2);
                    currentBuild->setTransform(trans);
                    scene->addItem(currentBuild);
                    tile->addPixRoad(currentBuild);
                    currentBuild=nullptr;
                    tile->setBrush(QBrush(baseColors[tile->getX()][tile->getY()]));
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
            if(!grid)tile->setPen(QPen(Qt::transparent));
            else tile->setPen(QPen(Qt::black));
            tile->setBrush(QBrush(baseColors[tile->getX()][tile->getY()]));
            tile->setOccupied(false);
            tile->setBuildingWidth(-10);
            tile->setBuildingHeight(-10);
            tile->setMainTile(-10,-10);

        }

        tempRoadList->clear();
        foreach(MapTile* tile,*radiusTilesList)
        {
            if(!grid)
                tile->setPen(QPen(Qt::transparent));
            else
                tile->setPen(QPen(Qt::black));
        }

        radiusTilesList->clear();


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
    int zoomFactor=5;
    switch(direction){
    case -2:
        this->translate(-15/zoomFactor-35/(zoom*zoomFactor),-15/zoomFactor-35/(zoom*zoomFactor));
        break;
    case 2:
        this->translate(15/zoomFactor+35/(zoom*zoomFactor),15/zoomFactor+35/(zoom*zoomFactor));
        break;
    case 1:
        this->translate(-15/zoomFactor-35/(zoom*zoomFactor),15/zoomFactor+35/(zoom*zoomFactor));
        break;
    case -1:
        this->translate(15/zoomFactor+35/(zoom*zoomFactor),-15/zoomFactor-35/(zoom*zoomFactor));
        break;
    }


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

//TO-DO: RETURN TILE CONTAINING NEIGHTBOUR ROAD TO CHANGE HIS PIXMAP OR NULLPTR IF NOT FOUND
bool MapView::checkIfNearRoadX(MapTile* tile)
{

    if(tile->getX()-1>=0 && tiles[tile->getX()-1][tile->getY()]->getBId()==0)return true;
    if(tile->getX()+1<nbTiles && tiles[tile->getX()+1][tile->getY()]->getBId()==0)return true;


    return false;
}






//TO-DO: RETURN TILE CONTAINING NEIGHTBOUR ROAD TO CHANGE HIS PIXMAP OR NULLPTR IF NOT FOUND
bool MapView::checkIfNearRoadY(MapTile* tile)
{

    if(tile->getY()-1>=0 && tiles[tile->getX()][tile->getY()-1]->getBId()==0)return true;
    if(tile->getY()+1<nbTiles && tiles[tile->getX()][tile->getY()+1]->getBId()==0)return true;


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

int MapView::countNeighbourRoads(MapTile* tile){
    int n=0;

    if(tile->getX()-1>=0 && tiles[tile->getX()-1][tile->getY()]->getBId()==0)n++;
    if(tile->getX()+1<nbTiles && tiles[tile->getX()+1][tile->getY()]->getBId()==0)n++;
    if(tile->getY()-1>=0 && tiles[tile->getX()][tile->getY()-1]->getBId()==0)n++;
    if(tile->getY()+1<nbTiles && tiles[tile->getX()][tile->getY()+1]->getBId()==0)n++;

    return n;
}



void MapView::getNeighbours(MapTile* tile){
    neighbourList->clear();
    if(tile->getX()-1>=0 && tiles[tile->getX()-1][tile->getY()]->getBId()==0)neighbourList->append(tiles[tile->getX()-1][tile->getY()]);
    if(tile->getY()-1>=0 && tiles[tile->getX()][tile->getY()-1]->getBId()==0)neighbourList->append(tiles[tile->getX()][tile->getY()-1]);
    if(tile->getX()+1<nbTiles && tiles[tile->getX()+1][tile->getY()]->getBId()==0)neighbourList->append(tiles[tile->getX()+1][tile->getY()]);
    if(tile->getY()+1<nbTiles && tiles[tile->getX()][tile->getY()+1]->getBId()==0)neighbourList->append(tiles[tile->getX()][tile->getY()+1]);
    neighbourList->append(tile);

}

void  MapView::updateNeighbourRoad(MapTile* tileIn){


    getNeighbours(tileIn);

     foreach(MapTile* tile,*neighbourList){

        int neighbourCount=countNeighbourRoads(tile);

        QString pixPath="NOPIX";
        QTransform trans;

        switch(neighbourCount){
        //"L"
        //        case 2:
        //            pixPath=":/ressources/routeL.png";
        //            break;
        //"T"
        case 3:
            pixPath=":/ressources/routeT.png";

            break;
            //crossroad
        case 4:

            pixPath=":/ressources/routeX.png";

            break;
        }

        trans.scale(0.19,0.19);

        switch(neighbourCount){
        case 2:
            if(tile->getX()-1>=0 && tile->getY()+1<nbTiles && tiles[tile->getX()-1][tile->getY()]->getBId()==0 && tiles[tile->getX()][tile->getY()+1]->getBId()==0){
                trans.rotate(-90);
                trans.translate(-5.35*pixelPerTile,0);
                pixPath=":/ressources/routeL.png";
            }else if(tile->getX()+1<nbTiles && tile->getY()-1>=0 && tiles[tile->getX()+1][tile->getY()]->getBId()==0 && tiles[tile->getX()][tile->getY()-1]->getBId()==0){
                trans.rotate(90);
                trans.translate(0,-5.35*pixelPerTile);
                pixPath=":/ressources/routeL.png";
            }else if(tile->getX()+1<nbTiles && tile->getY()+1<nbTiles && tiles[tile->getX()+1][tile->getY()]->getBId()==0 && tiles[tile->getX()][tile->getY()+1]->getBId()==0){
                trans.rotate(180);
                trans.translate(-5.35*pixelPerTile,-5.35*pixelPerTile);
                pixPath=":/ressources/routeL.png";
            }else if(tile->getX()-1>=0 && tile->getY()-1>=0 && tiles[tile->getX()-1][tile->getY()]->getBId()==0 && tiles[tile->getX()][tile->getY()-1]->getBId()==0){
                pixPath=":/ressources/routeL.png";
                // trans.translate(-pixelPerTile/7,0);
            }
            break;

        case 3:
            if(tile->getX()-1 >=0 && tile->getY()-1>=0 && tile->getX()+1<nbTiles&&tiles[tile->getX()-1][tile->getY()]->getBId()==0 && tiles[tile->getX()+1][tile->getY()]->getBId()==0 && tiles[tile->getX()][tile->getY()-1]->getBId()==0){
                trans.rotate(180);
                trans.translate(-5.35*pixelPerTile,-5.35*pixelPerTile);
            }
            if(tile->getX()+1 <nbTiles && tile->getY()-1>=0 && tile->getY()+1<nbTiles&&tiles[tile->getX()+1][tile->getY()]->getBId()==0 && tiles[tile->getX()][tile->getY()+1]->getBId()==0 && tiles[tile->getX()][tile->getY()-1]->getBId()==0){
                trans.rotate(-90);
                trans.translate(-5.35*pixelPerTile,0);
            }
            if(tile->getX()-1>=0 && tile->getY()-1>=0 && tile->getY()+1<nbTiles&&tiles[tile->getX()-1][tile->getY()]->getBId()==0 && tiles[tile->getX()][tile->getY()+1]->getBId()==0 && tiles[tile->getX()][tile->getY()-1]->getBId()==0){
                trans.rotate(90);
                trans.translate(0,-5.35*pixelPerTile);
            }
            break;
        }
        //        if(neighbourCount==2 ){
        //            if(onlyNeighbour!=tile){
        //                if(  onlyNeighbour->getY()<tile->getY() && onlyNeighbour->getX()==tile->getX() &&roadDir==2 && onlyNeighbour->getX()-1>0 && tiles[onlyNeighbour->getX()-1][onlyNeighbour->getY()]->getBId()==0){

        //                qDebug()<<"1";
        //                trans.rotate(-90);
        //                trans.translate(-5.35*pixelPerTile,0);
        //                }

        //                if( onlyNeighbour->getY()<tile->getY() && onlyNeighbour->getX()==tile->getX()  &&roadDir==2 && onlyNeighbour->getX()+1<nbTiles && tiles[onlyNeighbour->getX()+1][onlyNeighbour->getY()]->getBId()==0 && tempRoadList->indexOf(tiles[onlyNeighbour->getX()+1][onlyNeighbour->getY()])==-1){

        //                qDebug()<<"2";
        //                trans.rotate(180);
        //                trans.translate(-5.35*pixelPerTile,-5.35*pixelPerTile);
        //                }

        //                if(onlyNeighbour->getX()<tile->getX() && onlyNeighbour->getY()==tile->getY()  && roadDir==1 && tiles[onlyNeighbour->getX()][onlyNeighbour->getY()+1]->getBId()==0){
        //                    trans.rotate(180);
        //                trans.translate(-5.35*pixelPerTile,-5.35*pixelPerTile);
        //                 qDebug()<<"3";
        //                }

        //                if( onlyNeighbour->getY()>tile->getY() && onlyNeighbour->getX()==tile->getX()  && roadDir==-2 && tiles[onlyNeighbour->getX()+1][onlyNeighbour->getY()]->getBId()==0){
        //                    trans.rotate(90);
        //                trans.translate(0,-5.35*pixelPerTile);
        //                  qDebug()<<"4";
        //                }

        //                if( onlyNeighbour->getX()<tile->getX() && onlyNeighbour->getY()==tile->getY()  && roadDir==1 && tiles[onlyNeighbour->getX()][onlyNeighbour->getY()-1]->getBId()==0){
        //                    trans.rotate(90);
        //                trans.translate(0,-5.35*pixelPerTile);
        //                 qDebug()<<"5";
        //                }

        //                if( onlyNeighbour->getX()>tile->getX() && onlyNeighbour->getY()==tile->getY()&& roadDir==-1 && tiles[onlyNeighbour->getX()][onlyNeighbour->getY()+1]->getBId()==0){
        //                    trans.rotate(-90);
        //                    trans.translate(-5.35*pixelPerTile,0);
        //                 qDebug()<<"6";
        //                }
        //            }else{
        //                //le cas dans le cas une case qu'on vient d'ajouter fait office de virage
        //            }
        //        }



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

