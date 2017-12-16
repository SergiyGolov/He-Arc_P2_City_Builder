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
#include <GameManagementService.h>

GuiView::GuiView(QWidget *parent)
    : QGraphicsView(parent)
{


    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );


    this->setBackgroundBrush(QBrush(Qt::lightGray));
    scene=new QGraphicsScene(this);
    PickerElement::setScene(this->scene); //mdr magouille pas possible
    this->setScene(scene);

    this->setMouseTracking(true);
    screenWidth=QApplication::desktop()->screenGeometry().width();
    screenHeight=QApplication::desktop()->screenGeometry().height();

    this->setGeometry(0,screenHeight-screenHeight/10,screenWidth,screenHeight/10);
    tab=this->scene->addRect(0,0,screenWidth,screenHeight/6,QPen(Qt::transparent),QBrush(Qt::yellow));
    tab->setZValue(0);

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
        tabText->at(i)->setZValue(1);
    }



    road=new PickerElement();
    road->setText("Road");
    road->setBId(0);

    road->bouger(3,150);
    road->setRect(0,0,50,20);

    this->scene->addItem(road);


    remove=new PickerElement();

    remove->setText("Remove");
    remove->bouger(screenWidth-80,150);
    remove->setRect(0,0,50,20);
    remove->setBId(-1);
    this->scene->addItem(remove);

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
        tab->setBrush(QBrush(Qt::darkRed));
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
                tabText->at(ptrElem)->bouger(screenWidth/(nbElem+1)*ptrElem+3,50);


                tabText->at(ptrElem)->setRect(0,0,ConstantBuilding::get(i).getDisplayName().count()*7.6,20);
                tabText->at(ptrElem)->setBrush(QBrush(Qt::transparent));

                tabText->at(ptrElem)->setPen(QPen(Qt::black));
                tabText->at(ptrElem)->setBId(i);


                if(GameManagementService::getGameManagementService()->getMoney()-ConstantBuilding::get(i).getPrice()<0 && ConstantBuilding::get(i).getCategory()>2){
                    tabText->at(ptrElem)->changeTextColor(Qt::red);
                }else{
                      tabText->at(ptrElem)->changeTextColor(Qt::black);
                }

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
    //qDebug()<<event->pos().x()<<";"<<event->pos().y();

        if(PickerElement *pick=dynamic_cast<PickerElement*>(itemAt(event->pos()))){
            if(pick->getBId()==-1 || GameManagementService::getGameManagementService()->getMoney()-ConstantBuilding::get(pick->getBId()).getPrice()>=0 ||ConstantBuilding::get(pick->getBId()).getCategory()<=2 ){ //test si on a assez de thune pour ajouter le batiment
                //idee: colorier les batiment dont on a pas assez de thunes en rouge (en tout cas le texte)
            MapView::getMapView()->picker(pick->getBId());

            }

          //  qDebug()<<pick->getText();
        }

//    try{
//        PickerElement *pick=(PickerElement*)itemAt(event->pos());
//        if(pick!=nullptr)qDebug()<<pick->getText();
//    }catch(int e){
//        qDebug()<<"fallait pas cliquer au mauvais endroit";
//    }
}


GuiView* GuiView::getGuiView(){
    if(GuiView::guiViewInstance==nullptr){
        GuiView::guiViewInstance=new GuiView();
    }

    return GuiView::guiViewInstance;

}
