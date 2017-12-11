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



MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
{

    bPicker=false;
    click=false;
    pickerBId=0;
    this->gui=GuiView::getGuiView();
    prevRect=nullptr;

    this->setBackgroundBrush(QBrush(Qt::black));
    scene=new QGraphicsScene(this);
    this->setScene(scene);
    zoom=1;
    this->setMouseTracking(true);
    screenWidth=QApplication::desktop()->screenGeometry().width();
    screenHeight=QApplication::desktop()->screenGeometry().height();

    this->setGeometry(0,0,screenWidth,screenHeight-screenHeight/10);

    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    // scene->addRect(0,0,screenWidth,screenHeight,QPen(Qt::white),QBrush(Qt::white));

    int nbCases=nbcases;



    pixelParCase=((screenHeight-screenHeight/10)/nbCases); //*qSqrt(2)
    QColor color;

    //    QTransform transform;
    //    transform.setMatrix(0.707,0.409, -0.707, 0.409, 0, -0.816);
    //    transform="matrix(0.707 0.409 -0.707 0.409 0 -0.816)"

    //    this->setTransform(transform);
    // transform.shear(0,0.6);
    setTransformationAnchor ( QGraphicsView::NoAnchor );
    srand(time(NULL));
    for(int i=0;i<nbCases;i++){
        for(int j=0;j<nbCases;j++){


            int random = (rand() % (6)) ;

            switch(random){
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
            //faut limite construire un MapTile
            tiles[i][j]=false;
            tile->setRotation(45);

            // rect->setScale(0.925);


        }

    }
    this->translate(screenWidth/2,0);

    //this->setTransform(transform);
    scale(1,0.5);


}

MapView* MapView::mapViewInstance=nullptr;


MapView::~MapView()
{

}

void MapView::picker(int bId){
    bPicker=true;
    pickerBId=bId;

}

void MapView::wheelEvent(QWheelEvent * event){


    if(event->angleDelta().y()>0){
        //zoom
        if(zoom<1.25){
            zoom+=0.05;
            this->scale(2,2);
        }

    }
    else{

        if(zoom>1){zoom-=0.05;
            this->scale(0.5,0.5);
        }
    }


}

void MapView::mouseMoveEvent(QMouseEvent *event){

    /* if(event->pos().x()<20){
        this->translate(50,0);


    }else if(event->pos().x()>screenWidth-30){
        this->translate(-50,0);


    }else if(event->pos().y()<20){
        this->translate(0,50);

    }
    else if(event->pos().y()>screenHeight-screenHeight/10*2.5){
        this->translate(0,-50);

    }
*/
    if(bPicker){


        if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos()))){ //we, faut utiliser les dynamic_cast pour eviter de caster du chenil qui n'est pas sensé être downcastable


             if(prevColor!=rect->brush().color())prevColor=rect->brush().color();


            if(rect!=prevRect && prevRect!=nullptr && tiles[prevRect->getX()][prevRect->getY()]==false){ //faire un test pour ne pas pouvoir passer au dessus d'un bâtiment déjà construit, donc we faut implementer addbuilding avec la sdd contenant les batiments et tester s'il n'y a pas deja un batiment sur cette case
                // + implementer un systeme de coordonnée des cases simple à gérer pour pouvoir dire quelle case est occupée par un bâtiment/route
                prevRect->setBrush(QBrush(Qt::darkGreen));
            }

            if(tiles[rect->getX()][rect->getY()]==false){
                switch(pickerBId){
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
                    rect->setBrush(Qt::blue);
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

            }
            if(prevRect!=rect)prevRect=rect;
            //faire en sorte que le bâtiment déposé reste + utiliser addBuilding()
        }


    }
}



void MapView::mousePressEvent(QMouseEvent *event){
    //QGraphicsRectItem *rect=( QGraphicsRectItem *)itemAt(event->pos());
    //if(rect->brush()!=Qt::darkBlue)rect->setBrush(Qt::red);
    if(bPicker){
        bPicker=false;

        if(MapTile *rect=dynamic_cast<MapTile*>(itemAt(event->pos()))){
            prevRect=nullptr;
            if(tiles[rect->getX()][rect->getY()]==false){
                switch(pickerBId){
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
                    rect->setBrush(Qt::blue);
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
                tiles[rect->getX()][rect->getY()]=true;
            }


        }
    }
}

void MapView::keyPressEvent(QKeyEvent *event){
    //le pas de translation devrait dependre du niveau de zoom: plus on zoom moins on fait de pas
    switch(event->key()){
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
        this->translate(0,-25-25/zoom);
        break;
    case Qt::Key_Up:
    case Qt::Key_W:
        this->translate(0,25+25/zoom);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        this->translate(-25-25/zoom,0);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        this->translate(25+25/zoom,0);
        break;
    case Qt::Key_Plus:
        if(zoom<1.25){
            zoom+=0.05;
            this->scale(2,2);
        }
        break;
    case Qt::Key_Minus:
        if(zoom>1){
            zoom-=0.05;
            this->scale(0.5,0.5);
        }
        break;
    }
}

void zoom(bool plusMinus){
    //TODO
}

MapView* MapView::getMapView(){
    if(MapView::mapViewInstance==nullptr){
        MapView::mapViewInstance=new MapView();
    }

    return MapView::mapViewInstance;

}

void MapView::zoomMeth(bool plusMinus)
{

}
