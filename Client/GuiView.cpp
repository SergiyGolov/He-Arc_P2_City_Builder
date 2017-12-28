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
#include "GraphicService.h"

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


    tabCat=new std::vector<PickerElement*>(9,nullptr);
    for(int i=0;i<9;i++){
         tabCat->at(i)= new PickerElement();
         tabCat->at(i)->setText(QString("Category %1 [F%2]").arg(i+1).arg(i+1));
         tabCat->at(i)->bouger(screenWidth/10*i,3);
         tabCat->at(i)->setRect(0,0,105,20);
         tabCat->at(i)->setBId(-10-i*10);

         this->scene->addItem(tabCat->at(i));
    }

    road=new PickerElement();
    road->setText("Road[Q]");
    road->setBId(0);

    road->bouger(3,150);
    road->setRect(0,0,55,20);

    this->scene->addItem(road);


    remove=new PickerElement();

    remove->setText("Remove[R]");
    remove->bouger(screenWidth-100,150);
    remove->setRect(0,0,70,20);
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

        activeTabId=0;
        break;
    case 1:
        tab->setBrush(QBrush(Qt::red));

        activeTabId=1;
        break;
    case 2:
        tab->setBrush(QBrush(Qt::green));

        activeTabId=2;
        break;
    case 3:
        tab->setBrush(QBrush(Qt::darkRed));

        activeTabId=3;
        break;
    case 4:
        tab->setBrush(QBrush(Qt::magenta));

        activeTabId=4;
        break;
    case 5:
        tab->setBrush(QBrush(Qt::cyan));

        activeTabId=5;
        break;
    case 6:
        tab->setBrush(QBrush(Qt::white));

        activeTabId=6;
        break;
    case 7:
        tab->setBrush(QBrush(Qt::lightGray));

        activeTabId=7;
        break;

    case 8:
        tab->setBrush(QBrush(Qt::darkMagenta));

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

                if(ptrElem<9)tabText->at(ptrElem)->setText(ConstantBuilding::get(i).getDisplayName()+QString("[%1]\n").arg(ptrElem+1)+QString::number(ConstantBuilding::get(i).getPrice())+" $\n"+QString::number(ConstantBuilding::get(i).getPricePerSeconds())+" $/s");
                else if(ptrElem==9)tabText->at(ptrElem)->setText(ConstantBuilding::get(i).getDisplayName()+"[0]\n"+QString::number(ConstantBuilding::get(i).getPrice())+" $\n"+QString::number(ConstantBuilding::get(i).getPricePerSeconds())+" $/s");
                else tabText->at(ptrElem)->setText(ConstantBuilding::get(i).getDisplayName()+"\n"+QString::number(ConstantBuilding::get(i).getPrice())+" $\n"+QString::number(ConstantBuilding::get(i).getPricePerSeconds())+" $/s");
                if(nbElem==0)tabText->at(ptrElem)->bouger(screenWidth/(nbElem+1)*ptrElem+3,50);
                else tabText->at(ptrElem)->bouger(screenWidth/(nbElem)*ptrElem+3,50);

                tabText->at(ptrElem)->setRect(0,0,ConstantBuilding::get(i).getDisplayName().count()*8+6,60);
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

void GuiView::selectBuilding(int n){
    if(tabText->at(n)->getText()!="" && (GameManagementService::getGameManagementService()->getMoney()-ConstantBuilding::get(tabText->at(n)->getBId()).getPrice()>=0 ||ConstantBuilding::get(tabText->at(n)->getBId()).getCategory()<=2) ){
        MapView::getMapView()->picker(tabText->at(n)->getBId());
    }
}


void GuiView::mousePressEvent(QMouseEvent *event){
    //qDebug()<<event->pos().x()<<";"<<event->pos().y();

    if(PickerElement *pick=dynamic_cast<PickerElement*>(itemAt(event->pos()))){
        if(pick->getBId()>=-1 && (pick->getBId()==-1 || GameManagementService::getGameManagementService()->getMoney()-ConstantBuilding::get(pick->getBId()).getPrice()>=0 ||ConstantBuilding::get(pick->getBId()).getCategory()<=2) ){ //test si on a assez de thune pour ajouter le batiment
            //idee: colorier les batiment dont on a pas assez de thunes en rouge (en tout cas le texte)
            MapView::getMapView()->picker(pick->getBId());

        }else if(pick->getBId()<-1){
            showBuildingPickerMenu(-1*(pick->getBId()/10)-1);
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
void GuiView::keyPressEvent(QKeyEvent *event){
    GraphicService::getGraphicService()->setKeyboardShortcuts(event->key());


}

GuiView* GuiView::getGuiView(){
    if(GuiView::guiViewInstance==nullptr){
        GuiView::guiViewInstance=new GuiView();
    }

    return GuiView::guiViewInstance;

}
