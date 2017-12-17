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

//singleton
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
    grille=false;
    pickerBId=0;
    this->gui=GuiView::getGuiView();
    prevRect=nullptr;
    roadDir=0;

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

    // scene->addRect(0,0,screenWidth,screenHeight,QPen(Qt::white),QBrush(Qt::white));

    rayonTile=new QList<MapTile*>();
    tempRoad=new QList<MapTile*>();
    tempRemove=new QList<MapTile*>();

    int nbCases = nbcases;

    pixelParCase=((screenHeight-screenHeight/10)/nbCases)*2; //*qSqrt(2)
    QColor color;

    //QTransform transform;
    //transform.setMatrix(0.707,0.409, -0.707, 0.409, 0, -0.816);
    //transform="matrix(0.707 0.409 -0.707 0.409 0 -0.816)"

    //this->setTransform(transform);
    // transform.shear(0,0.6);
    setTransformationAnchor ( QGraphicsView::NoAnchor );
    srand(time(NULL));
    for(int i=0;i<nbCases;i++)
    {
        for(int j=0;j<nbCases;j++)
        {
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
            int coorX=i*pixelParCase;
            int coorY=j*pixelParCase;

            //            int coorX2=(i+1)*pixelParCase;
            //            int coorY2=(j+1)*pixelParCase;

            //            int isoX=coorX-coorY;
            //            int isoY=(coorX+coorY)/2;

            //int isoX2=coorX2-coorY2;
            //int isoY2=(coorX2+coorY2)/2;
            //double w,h;
            //            double t = 0.3 ;// tilt angle - try different values
            //            w=h=pixelParCase;
            //            double X = coorX - w/2 ;
            //            double Y = coorY - h/2 ;
            //            double a = h/(h + Y*qSin(t)) ;
            //            double u = a*X + w/2 ;
            //            double v = a*Y*qCos(t) + h/2 ;

            //QRect rectTest(this->mapToScene(QPoint(isoX,isoY)),this->mapToScene(QPoint(isoX2,isoY2)));
            // double x=this->mapFromScene(QPoint(isoX,isoY)).x();
            //double y=this->mapFromScene(QPoint(isoX,isoY)).y();
            //double w=x-this->mapFromScene(QPoint(isoX2,isoY2)).x();
            //double h=y-this->mapFromScene(QPoint(isoX2,isoY2)).y();

            // QGraphicsRectItem *rect= scene->addRect(coorX,coorY,pixelParCase,pixelParCase,QPen(Qt::transparent),QBrush(color));
            MapTile *tile=new MapTile(i,j,coorX,coorY,pixelParCase,pixelParCase);
            tile->setPen(QPen(Qt::transparent));
            tile->setBrush(QBrush(color));
            scene->addItem(tile);
            tiles[i][j]=tile;
            //faut limite construire un MapTile
            tilesBool[i][j]=false;

            //tile->setScale(0.5);
            // rect->setScale(0.925);
            tile->setRotation(45);
        }

    }
    translate(screenWidth/2,0);

    //this->setTransform(transform);
    scale(1,0.5);
}

MapView::~MapView()
{

}

void MapView::picker(int bId){
    bPicker=true;
    pickerBId=bId;
    if(pickerBId != -1)
    {
        largeurBat=ConstantBuilding::get(pickerBId).getTileWidth();
        hauteurBat=ConstantBuilding::get(pickerBId).getTileHeight();
        rayonBat=ConstantBuilding::get(pickerBId).getRadius();
    }
    // qDebug()<<bId;
}

void MapView::wheelEvent(QWheelEvent * event)
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
        if(pickerBId!=-1)
        {
            if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos())))
            {
                //we, faut utiliser les dynamic_cast pour eviter de caster du chenil qui n'est pas sensé être downcastable
                bool caseOccupe=false;

                foreach(MapTile* tile,*rayonTile)
                {
                    if(!grille)tile->setPen(QPen(Qt::transparent));
                    else tile->setPen(QPen(Qt::black));
                }

                rayonTile->clear();

                for(int i=0;i<largeurBat;i++)
                {
                    for(int j=0;j<hauteurBat;j++)
                    {
                        if(tilesBool[rect->getX()+i][rect->getY()+j]==true)
                            caseOccupe=true;
                    }
                }
                if(rect!=prevRect && prevRect!=nullptr)
                {
                    //faire un test pour ne pas pouvoir passer au dessus d'un bâtiment déjà construit, donc we faut implementer addbuilding avec la sdd contenant les batiments et tester s'il n'y a pas deja un batiment sur cette case
                    // + implementer un systeme de coordonnée des cases simple à gérer pour pouvoir dire quelle case est occupée par un bâtiment/route
                    //prevRect->setBrush(QBrush(Qt::darkGreen));
                    for(int i=0;i<largeurBat;i++)
                    {
                        for(int j=0;j<hauteurBat;j++)
                        {
                            if(prevRect->getX()+i<nbcases && prevRect->getY()+j<nbcases)
                            {
                                tiles[prevRect->getX()+i][prevRect->getY()+j]->setBrush(QBrush(Qt::darkGreen));
                                //if(!grille)tiles[prevRect->getX()+i][prevRect->getY()+j]->setPen(QPen(Qt::transparent));
                                //else tiles[prevRect->getX()+i][prevRect->getY()+j]->setPen(QPen(Qt::black));
                            }
                        }
                    }
                }
                if(rect->getX()+largeurBat<nbcases && rect->getX()+hauteurBat<nbcases && caseOccupe==false)
                {
                    //if(prevColor!=rect->brush().color())prevColor=rect->brush().color();
                    if(caseOccupe==false)
                    {
                        QColor color;
                        if(pickerBId==-1)
                        {

                        }
                        else
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

                            for(int i=0;i<largeurBat;i++)
                            {
                                for(int j=0;j<hauteurBat;j++)
                                {
                                    tiles[rect->getX()+i][rect->getY()+j]->setBrush(color);
                                }
                            }

                            if(rayonBat>0)
                            {
                                for(int i=-rayonBat;i<rayonBat+largeurBat;i++)
                                {
                                    for(int j=-rayonBat;j<rayonBat+hauteurBat;j++)
                                    {
                                        if(rect->getX()+i>=0 && rect->getY()+j>=0 && rect->getX()+i<nbcases && rect->getY()+j<nbcases)
                                        {
                                            tiles[rect->getX()+i][rect->getY()+j]->setPen(color);
                                            rayonTile->append(tiles[rect->getX()+i][rect->getY()+j]);
                                        }
                                    }
                                }
                            }

                            if(prevRect!=rect)
                                prevRect=rect;
                        }
                    }
                    //faire en sorte que le bâtiment déposé reste + utiliser addBuilding()
                }
            }
        }
        else if(bPicker || road)
        {
            //mode remove
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

    if(road==true)
    {
        if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos())))
        {
            if(rect->getX()==roadStartX || rect->getY()==roadStartY)
            {
                if(roadDir==0)
                {
                    if(rect->getX()>roadStartX)
                    {
                        roadDir=1;// pour fixer la direction a partir du premier carré depuis le debut de la route pour ne pouvoir aller que dans une direction jusqu'à que la route soit construite
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
                    tempRoad->append(rect);
                    rect->setBrush(QBrush(Qt::darkGray));
                }
            }
        }
    }
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::RightButton && bPicker || event->button()==Qt::RightButton&&road)
    {
        road=false;
        bPicker=false;
        foreach(MapTile* tile,*tempRoad)
        {
            if(!grille)tile->setPen(QPen(Qt::transparent));
            else tile->setPen(QPen(Qt::black));
            tile->setBrush(QBrush(Qt::darkGreen));
            tilesBool[tile->getX()][tile->getY()]=false;
            tile->setLargeurBat(-10);
            tile->setHauteurBat(-10);
            tile->setMainTile(-10,-10);
        }

        tempRoad->clear();
        foreach(MapTile* tile,*rayonTile)
        {
            if(!grille)
                tile->setPen(QPen(Qt::transparent));
            else
                tile->setPen(QPen(Qt::black));
        }

        rayonTile->clear();

        if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos())))
        {
            rect->setBrush(QBrush(Qt::darkGreen));
            for(int i=0;i<largeurBat;i++)
            {
                for(int j=0;j<hauteurBat;j++)
                {
                    if(rect->getX()+i<nbcases && rect->getY()+j<nbcases)
                    {
                        tiles[rect->getX()+i][rect->getY()+j]->setBrush(QBrush(Qt::darkGreen));
                    }
                }
            }
        }
    }

    if( road==true)
    {
        road=false;
        if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos()))){
            if(rect->getX()>roadStartX &&roadDir==1 && rect->getY()==roadStartY || rect->getX()<roadStartX &&roadDir==-1&& rect->getY()==roadStartY || rect->getY()>roadStartY && roadDir==2 && rect->getX()==roadStartX|| rect->getY()<roadStartY && roadDir==-2&& rect->getX()==roadStartX){
                tempRoad->append(rect);

                rect->setBrush(QBrush(Qt::darkGray));


                //  buildingCount++;


                foreach(MapTile* tile, *tempRoad){

                    tilesBool[tile->getX()][tile->getY()]=true;
                    tiles[tile->getX()][tile->getY()]->setBId(0);
                    //tiles[tile->getX()][tile->getY()]->setUniqueBId(buildingCount);
                    tiles[tile->getX()][tile->getY()]->setMainTile(roadStartX,roadStartY);
                    if(roadDir==1){

                        tiles[tile->getX()][tile->getY()]->setLargeurBat(rect->getX()-roadStartX+1);
                        tiles[tile->getX()][tile->getY()]->setHauteurBat(1);

                    }else if(roadDir==-1){

                        tiles[tile->getX()][tile->getY()]->setLargeurBat(roadStartX-rect->getX()+1);
                        tiles[tile->getX()][tile->getY()]->setHauteurBat(1);
                        tiles[tile->getX()][tile->getY()]->setMainTile(rect->getX(),rect->getY());
                        rect->setMainTile(rect->getX(),rect->getY());
                    }else if(roadDir==2){

                        tiles[tile->getX()][tile->getY()]->setLargeurBat(1);
                        tiles[tile->getX()][tile->getY()]->setHauteurBat(rect->getY()-roadStartY+1);

                    }else if(roadDir==-2){

                        tiles[tile->getX()][tile->getY()]->setLargeurBat(1);
                        tiles[tile->getX()][tile->getY()]->setHauteurBat(roadStartY-rect->getY()+1);
                        tiles[tile->getX()][tile->getY()]->setMainTile(rect->getX(),rect->getY());
                        rect->setMainTile(rect->getX(),rect->getY());
                    }

                }
                tempRoad->clear();
            }else{

                foreach(MapTile* tile,*tempRoad){
                    if(!grille)tile->setPen(QPen(Qt::transparent));
                    else tile->setPen(QPen(Qt::black));
                    tile->setBrush(QBrush(Qt::darkGreen));
                    tilesBool[tile->getX()][tile->getY()]=false;
                    tile->setLargeurBat(-10);
                    tile->setHauteurBat(-10);
                    tile->setMainTile(-10,-10);
                }

                tempRoad->clear();
            }
        }
    }else{

        if(bPicker && road==false){
            bPicker=false;
            if(pickerBId!=-1){


                if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos()))){
                    bool caseOccupe=false;
                    for(int i=0;i<largeurBat;i++){
                        for(int j=0;j<hauteurBat;j++){
                            if(tilesBool[rect->getX()+i][rect->getY()+j]==true)caseOccupe=true;
                        }
                    }

                    if(rect->getX()+largeurBat<nbcases && rect->getX()+hauteurBat<nbcases && caseOccupe==false){
                        if(pickerBId==0 && road==false){
                            road=true;
                            roadDir=0;
                            roadStartX=rect->getX();
                            roadStartY=rect->getY();
                            tempRoad->append(rect);
                        }
                        prevRect=nullptr;
                        if(checkIfNearRoad(rect) || road ){
                            // if(tilesBool[rect->getX()][rect->getY()]==false){
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
                                for(int i=0;i<largeurBat;i++){
                                    for(int j=0;j<hauteurBat;j++){
                                        tilesBool[rect->getX()+i][rect->getY()+j]=true;
                                        tiles[rect->getX()+i][rect->getY()+j]->setBId(pickerBId);
                                        tiles[rect->getX()+i][rect->getY()+j]->setMainTile(mainTileX,mainTileY);
                                        tiles[rect->getX()+i][rect->getY()+j]->setLargeurBat(largeurBat);
                                        tiles[rect->getX()+i][rect->getY()+j]->setHauteurBat(hauteurBat);
                                        tiles[rect->getX()+i][rect->getY()+j]->setUniqueBId(buildingCount);

                                    }
                                }
                                BuildingManagementService::getBuildingManagementService()->addBuilding(pickerBId,mainTileX,mainTileY, 0); //TODO : set 0 to 3 angles
                            }
                        }else if(road==false){

                            //faire clignoter en rouge?
                            for(int i=0;i<largeurBat;i++){
                                for(int j=0;j<hauteurBat;j++){
                                    if(rect->getX()+i<nbcases && rect->getY()+j<nbcases){
                                        tiles[rect->getX()+i][rect->getY()+j]->setBrush(QBrush(Qt::darkGreen));
                                        //                             if(!grille)tiles[prevRect->getX()+i][prevRect->getY()+j]->setPen(QPen(Qt::transparent));
                                        //                             else tiles[prevRect->getX()+i][prevRect->getY()+j]->setPen(QPen(Qt::black));
                                    }
                                }
                            }
                            blinkTileRed(rect);
                        }
                        foreach(MapTile* tile,*rayonTile){
                            if(!grille)tile->setPen(QPen(Qt::transparent));
                            else tile->setPen(QPen(Qt::black));
                        }

                        rayonTile->clear();

                        //}
                    }

                }
            }else{
                //SI ON EST EN MODE REMOVE
                if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos()))){
                    if(rect->getBId()!=-10){ //donc si ce n'est pas une case de base
                        if(rect->getUniqueBId()!=-10)BuildingManagementService::getBuildingManagementService()->removeBuilding(rect->getUniqueBId());
                        for(int i=0;i<rect->getLargeurBat();i++){
                            for(int j=0;j<rect->getHauteurBat();j++){

                                tilesBool[rect->getMainTileX()+i][rect->getMainTileY()+j]=false;
                                tiles[rect->getMainTileX()+i][rect->getMainTileY()+j]->setBId(-10);

                                tiles[rect->getMainTileX()+i][rect->getMainTileY()+j]->setUniqueBId(-10);
                                tiles[rect->getMainTileX()+i][rect->getMainTileY()+j]->setBrush(QBrush(Qt::darkGreen));


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
    //le pas de translation devrait dependre du niveau de zoom: plus on zoom moins on fait de pas
    switch(event->key()){
    case Qt::Key_Q:
        //addRoadMode();
        break;
    case Qt::Key_R:
        removeBuildingMode();
        break;
    case Qt::Key_1:
        gui->showBuildingPickerMenu(0);
        break;
    case Qt::Key_2:
        gui->showBuildingPickerMenu(1);
        break;
    case Qt::Key_3:
        gui->showBuildingPickerMenu(2);
        break;
    case Qt::Key_4:
        gui->showBuildingPickerMenu(3);
        break;
    case Qt::Key_5:
        gui->showBuildingPickerMenu(4);
        break;
    case Qt::Key_6:
        gui->showBuildingPickerMenu(5);
        break;
    case Qt::Key_7:
        gui->showBuildingPickerMenu(6);
        break;
    case Qt::Key_8:
        gui->showBuildingPickerMenu(7);
        break;
    case Qt::Key_9:
        gui->showBuildingPickerMenu(8);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        translateMeth(-2);
        break;
    case Qt::Key_Up:
    case Qt::Key_W:
        translateMeth(2);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        translateMeth(1);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        translateMeth(-1);
        break;
    case Qt::Key_Plus:
        zoomMeth(true);
        break;
    case Qt::Key_Minus:
        zoomMeth(false);
        break;
    case Qt::Key_G:
        for(int i=0;i<nbcases;i++){
            for(int j=0;j<nbcases;j++){
                if(grille){
                    tiles[i][j]->setPen(QPen(Qt::transparent));


                }else if(!grille){
                    tiles[i][j]->setPen(QPen(Qt::black));


                }
            }
        }
        grille=!grille;
        break;
    }
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
        if(zoom>1){
            zoom-=0.05;
            this->scale(0.5,0.5);
        }
    }
}


void MapView::removeBuildingMode(){
    if(bPicker==false){
        bPicker=true;
        pickerBId=-1;
    }
}

void MapView::addRoadMode(){
    if(road==false && bPicker==false){
        bPicker=true;
        pickerBId=0;
    }else if(bPicker==false){
        road=false;
        bPicker=true;
        pickerBId=0;
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
    for(int i=0; i < largeurBat; i++)
    {
        for(int j=0; j < hauteurBat; j++)
        {
            if(tile->getX()-1+i>=0 && tiles[tile->getX()-1+i][tile->getY()+j]->getBId()==0)return true;
            if(tile->getX()+1+i<nbcases && tiles[tile->getX()+1+i][tile->getY()+j]->getBId()==0)return true;
            if(tile->getY()-1+j>=0 && tiles[tile->getX()+i][tile->getY()-1+j]->getBId()==0)return true;
            if(tile->getY()+1+j<nbcases && tiles[tile->getX()+i][tile->getY()+1+j]->getBId()==0)return true;
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
        for(int i=0;i<largeurBat;i++)
        {
            for(int j=0;j<hauteurBat;j++)
            {
                if(timeTick)
                {
                    tiles[blinkRedTile->getX()+i][blinkRedTile->getY()+j]->setPen(QPen(Qt::red));
                }
                else
                {
                    if(grille)
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
        for(int i=0;i<largeurBat;i++)
        {
            for(int j=0;j<hauteurBat;j++)
            {
                if(grille)
                    tiles[blinkRedTile->getX()+i][blinkRedTile->getY()+j]->setPen(QPen(Qt::black));
                else
                    tiles[blinkRedTile->getX()+i][blinkRedTile->getY()+j]->setPen(QPen(Qt::transparent));
            }
        }
    }
}

