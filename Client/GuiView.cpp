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
#include <Client/ConstantBuilding.h>
#include <QDebug>
#include <Client/GameManagementService.h>
#include "GraphicService.h"
#include "BuildingManagementService.h"

GuiView::GuiView(QWidget *parent)
    : QGraphicsView(parent)
{
    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    this->setBackgroundBrush(QBrush(Qt::lightGray));
    scene=new QGraphicsScene(this);
    PickerElement::setScene(this->scene); //to set the same scene to the rectItem and textItem in the pickerelement
    this->setScene(scene);

    this->setMouseTracking(true);

    screenWidth=QApplication::desktop()->screenGeometry().width();
    screenHeight=QApplication::desktop()->screenGeometry().height();

    this->setGeometry(0,screenHeight-screenHeight/10,screenWidth,screenHeight/10);
    tab=this->scene->addRect(0,0,screenWidth,screenHeight/6,QPen(Qt::transparent),QBrush(Qt::lightGray));
    tab->setZValue(0);


    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    activeTabId=0;


    cptMaxCat=0;
    for(int i=0;i<10;i++)
    {
        int cpt=0;
        for(int j=0;j<ConstantBuilding::getNbBuildings();j++)
        {
            if(ConstantBuilding::get(j).getCategory()==i)cpt++;
        }
        if(cpt>cptMaxCat)cptMaxCat=cpt;
    }


    tabText=new std::vector<PickerElement*>(cptMaxCat,nullptr);

    for(int i=0;i<cptMaxCat;i++)
    {
        tabText->at(i)=new PickerElement();
        this->scene->addItem(tabText->at(i));
        tabText->at(i)->setZValue(1);
    }


    tabCat=new std::vector<PickerElement*>(9,nullptr);
    for(int i=0;i<9;i++)
    {
        tabCat->at(i)= new PickerElement();
        switch(i)
        {
        case 0:
            tabCat->at(i)->setText(QString("Houses [F%1]").arg(i+1));
            break;
        case 1:
            tabCat->at(i)->setText(QString("Public function [F%1]").arg(i+1));
            break;
        case 2:
            tabCat->at(i)->setText(QString("Healthcare [F%1]").arg(i+1));
            break;
        case 3:
            tabCat->at(i)->setText(QString("Emergency [F%1]").arg(i+1));
            break;
        case 4:
            tabCat->at(i)->setText(QString("Security [F%1]").arg(i+1));
            break;
        case 5:
            tabCat->at(i)->setText(QString("Education [F%1]").arg(i+1));
            break;
        case 6:
            tabCat->at(i)->setText(QString("Public transport [F%1]").arg(i+1));
            break;
        case 7:
            tabCat->at(i)->setText(QString("Entertainement [F%1]").arg(i+1));
            break;
        case 8:
            tabCat->at(i)->setText(QString("Monuments [F%1]").arg(i+1));
            break;
        }

        tabCat->at(i)->setBrush(QBrush(Qt::transparent));

        QColor color=GraphicService::getColorFromBuildingCategory(i);

        tabCat->at(i)->setBrush(QBrush(Qt::transparent));
        tabCat->at(i)->setPen(QPen(Qt::transparent));
        tabCat->at(i)->changeTextColor(color);
        tabCat->at(i)->moveThis(screenWidth/10*i,3);
        tabCat->at(i)->setRect(0,0, tabCat->at(i)->getText().length()*8,20);
        tabCat->at(i)->setBId(-10-i*10);

        this->scene->addItem(tabCat->at(i));
    }

    road=new PickerElement();
    road->setText("Road [Q]");
    road->setBId(0);
    road->moveThis(3,tab->boundingRect().height()-70);
    road->setRect(0,0,80,75);

    this->scene->addItem(road);


    remove=new PickerElement();

    remove->setText("Remove [R]");
    remove->moveThis(screenWidth-110,tab->boundingRect().height()-70);
    remove->setRect(0,0,80,75);
    remove->setBId(-1);
    this->scene->addItem(remove);


    QGraphicsPixmapItem *removePix=new QGraphicsPixmapItem(QPixmap(":ressources/remove.png")); //https://www.freepik.com
    scene->addItem(removePix);
    removePix->setZValue(5);
    removePix->moveBy(screenWidth-105,tab->boundingRect().height()-55);

    QGraphicsPixmapItem *roadPix=new QGraphicsPixmapItem(QPixmap(":ressources/roadIcon.png")); //https://www.freepik.com
    scene->addItem(roadPix);
    roadPix->setZValue(5);
    roadPix->moveBy(0,tab->boundingRect().height()-55);


    showBuildingPickerMenu(0);

}

GuiView* GuiView::guiViewInstance=nullptr;


GuiView::~GuiView()
{

}



/**
* showBuildingPickerMenu
* Show the building picker menu for a specified category
*
* @param int tabId : id of the category
*/
void GuiView::showBuildingPickerMenu(int tabId)
{
    for(int i=0;i<9;i++)
    {
        tabCat->at(i)->setPen(QPen(Qt::transparent));
    }


    QColor color=GraphicService::getColorFromBuildingCategory(tabId);
    activeTabId=tabId;
    tabCat->at(tabId)->setPen(QPen(color));


    int nbElem=0;
    for(int i=0;i<ConstantBuilding::getNbBuildings();i++)
    {
        if(ConstantBuilding::get(i).getCategory()==activeTabId+1) {
            nbElem++;
        }


        int ptrElem=0;
        for(int i=0;i<ConstantBuilding::getNbBuildings();i++)
        {
            if(ConstantBuilding::get(i).getCategory()==activeTabId+1) {

                if(ptrElem<9)tabText->at(ptrElem)->setText(ConstantBuilding::get(i).getDisplayName()+QString(" [%1]\n").arg(ptrElem+1)+QString::number(ConstantBuilding::get(i).getPrice())+" $\n"+QString::number(ConstantBuilding::get(i).getPricePerSeconds())+" $/s");
                else if(ptrElem==9)tabText->at(ptrElem)->setText(ConstantBuilding::get(i).getDisplayName()+"[0]\n"+QString::number(ConstantBuilding::get(i).getPrice())+" $\n"+QString::number(ConstantBuilding::get(i).getPricePerSeconds())+" $/s");
                else tabText->at(ptrElem)->setText(ConstantBuilding::get(i).getDisplayName()+"\n"+QString::number(ConstantBuilding::get(i).getPrice())+" $\n"+QString::number(ConstantBuilding::get(i).getPricePerSeconds())+" $/s");
                if(nbElem==0)tabText->at(ptrElem)->moveThis(screenWidth/(nbElem+1)*ptrElem+3,50);
                else tabText->at(ptrElem)->moveThis(screenWidth/(nbElem)*ptrElem+3,50);

                tabText->at(ptrElem)->setRect(0,0,(ConstantBuilding::get(i).getDisplayName().count())*9+8,60);

                tabText->at(ptrElem)->setBId(i);


                if(!BuildingManagementService::getBuildingManagementService()->isBuildingAddable(i)||GameManagementService::getGameManagementService()->getMoney()-ConstantBuilding::get(i).getPrice()<0 && ConstantBuilding::get(i).getCategory()>2)
                {
                    tabText->at(ptrElem)->changeTextColor(Qt::red);
                }else{
                    tabText->at(ptrElem)->changeTextColor(Qt::black);
                }

                ptrElem++;
            }
        }
        for(int i=ptrElem;i<cptMaxCat;i++)
        {
            tabText->at(i)->setText("");
            tabText->at(i)->setRect(0,0,0,0);
        }
    }
}

void GuiView::selectBuilding(int n)
{
    if(BuildingManagementService::getBuildingManagementService()->isBuildingAddable(tabText->at(n)->getBId())&& (tabText->at(n)->getText()!="" && (GameManagementService::getGameManagementService()->getMoney()-ConstantBuilding::get(tabText->at(n)->getBId()).getPrice()>=0 ||ConstantBuilding::get(tabText->at(n)->getBId()).getCategory()<=2) ))
    {
        MapView::getMapView()->callPicker(tabText->at(n)->getBId());
    }
}



/**
* mousePressEvent
* Handles the clicks on a building or a category
*/

void GuiView::mousePressEvent(QMouseEvent *event)
{
    QList<QGraphicsItem *> itemsList=items(event->pos());

    if(!itemsList.isEmpty() && itemsList.size()>1)
    {

        if(PickerElement *pick=dynamic_cast<PickerElement*>(itemsList.at(1)))
        {
            if(pick->getBId()>=-1 && (pick->getBId()==-1 || (BuildingManagementService::getBuildingManagementService()->isBuildingAddable(pick->getBId()) && GameManagementService::getGameManagementService()->getMoney()-ConstantBuilding::get(pick->getBId()).getPrice()>=0 ) || ConstantBuilding::get(pick->getBId()).getCategory()<=2) )
            { //test if we have enough money to add the building or if we want to add a house/townhall (they are free)
                MapView::getMapView()->callPicker(pick->getBId());

            }else if(pick->getBId()<-1)
            {
                showBuildingPickerMenu(-1*(pick->getBId()/10)-1);
            }
        }
    }
}

void GuiView::keyPressEvent(QKeyEvent *event)
{
    GraphicService::getGraphicService()->setKeyboardShortcuts(event->key());
}

GuiView* GuiView::getGuiView()
{
    if(GuiView::guiViewInstance==nullptr)
    {
        GuiView::guiViewInstance=new GuiView();
    }

    return GuiView::guiViewInstance;

}
