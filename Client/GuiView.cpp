#include "GuiView.h"
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
#include <ConstantBuilding.h>
#include <QDebug>

GuiView::GuiView(QWidget *parent)
    : QGraphicsView(parent)
{





    this->setBackgroundBrush(QBrush(Qt::lightGray));
    scene=new QGraphicsScene(this);
    this->setScene(scene);

    this->setMouseTracking(true);
    screenWidth=QApplication::desktop()->screenGeometry().width();
    screenHeight=QApplication::desktop()->screenGeometry().height();

    this->setGeometry(0,screenHeight-screenHeight/10,screenWidth,screenHeight/10);
    tab=this->scene->addRect(0,0,screenWidth,screenHeight,QPen(Qt::transparent),QBrush(Qt::yellow));

    text=new QGraphicsSimpleTextItem("Category 0");
    this->scene->addItem(text);


    //  text->moveBy(10,20);


    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    activeTabId=0;

    //tabConstantBuilding=ConstantBuilding::tabGet();

    cptMaxCat=0;
    for(int i=0;i<10;i++){
        int cpt=0;
        for(int j=0;j<ConstantBuilding::getNbBuildings();j++){
            if(ConstantBuilding::get(j).getCategory()==i)cpt++;
        }
        if(cpt>cptMaxCat)cptMaxCat=cpt;
    }
  tabText=new std::vector<PickerElement*>(cptMaxCat,nullptr);
   //tabText(cptMaxCat);
    for(int i=0;i<cptMaxCat;i++){
       tabText->at(i)=new PickerElement();
        this->scene->addItem(tabText->at(i));
    }


    //TODO: add "remove Building/road" pickerelement en bas à droite
    PickerElement* road=new PickerElement();
    road->setText("Road");
    road->setBId(0);

    road->moveBy(0,150);
    road->setRect(0,0,50,20);
    this->scene->addItem(road);
    showBuildingPickerMenu(0);

}

GuiView* GuiView::guiViewInstance=nullptr;


GuiView::~GuiView()
{

}




void GuiView::showBuildingPickerMenu(int tabId){



    switch(tabId){
    case 0:
        tab->setBrush(QBrush(Qt::yellow));
        text->setText("Category 0");
        activeTabId=0;


        break;
    case 1:
        tab->setBrush(QBrush(Qt::red));
        text->setText("Category 1");
        activeTabId=1;
        break;
    case 2:
        tab->setBrush(QBrush(Qt::green));
        text->setText("Category 2");
        activeTabId=2;
        break;
    case 3:
        tab->setBrush(QBrush(Qt::blue));
        text->setText("Category 3");
        activeTabId=3;
        break;
    case 4:
        tab->setBrush(QBrush(Qt::magenta));
        text->setText("Category 4");
        activeTabId=4;
        break;
    case 5:
        tab->setBrush(QBrush(Qt::cyan));
        text->setText("Category 5");
        activeTabId=5;
        break;
    case 6:
        tab->setBrush(QBrush(Qt::white));
        text->setText("Category 6");
        activeTabId=6;
        break;
    case 7:
        tab->setBrush(QBrush(Qt::lightGray));
        text->setText("Category 7");
        activeTabId=7;
        break;

    case 8:
        tab->setBrush(QBrush(Qt::darkMagenta));
        text->setText("Category 8");
        activeTabId=8;
        break;
    }



    int nbElem=0;
    for(int i=0;i<ConstantBuilding::getNbBuildings();i++){
        if(ConstantBuilding::get(i).getCategory()==activeTabId+1) {
            nbElem++;
        }


        int ptrElem=0;
        for(int i=0;i<ConstantBuilding::getNbBuildings();i++){
            if(ConstantBuilding::get(i).getCategory()==activeTabId+1) {

               tabText->at(ptrElem)->setText(ConstantBuilding::get(i).getDisplayName());
               tabText->at(ptrElem)->setPos(screenWidth/(nbElem+1)*ptrElem,50);


               tabText->at(ptrElem)->setRect(0,0,ConstantBuilding::get(i).getDisplayName().count()*7.5,20);
               tabText->at(ptrElem)->setBrush(QBrush(Qt::transparent));
               tabText->at(ptrElem)->setPen(QPen(Qt::black));
               tabText->at(ptrElem)->setBId(i);
                ptrElem++;
            }
        }
        for(int i=ptrElem;i<cptMaxCat;i++){
            tabText->at(i)->setText("");
            tabText->at(i)->setRect(0,0,0,0);
        }
    }
}




void GuiView::mousePressEvent(QMouseEvent *event){
    //QGraphicsRectItem *rect=( QGraphicsRectItem *)itemAt(event->pos());
    MapView::getMapView()->picker(activeTabId);


}


GuiView* GuiView::getGuiView(){
    if(GuiView::guiViewInstance==nullptr){
        GuiView::guiViewInstance=new GuiView();
    }

    return GuiView::guiViewInstance;

}
