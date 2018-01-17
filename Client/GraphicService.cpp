#include "Client/GraphicService.h"

#include "Client/LoadSaveService.h"

GraphicService::GraphicService(QString file){
    loading=new QMessageBox(QString("Loading"),QString("The game is loading..."),QMessageBox::Information,QMessageBox::NoButton,QMessageBox::NoButton,QMessageBox::NoButton);

    loading->setWindowFlag(Qt::WindowStaysOnTopHint);

    MainWidget::getMainWidget()->showFullScreen();


    loading->show();
    LoadSaveService::loadGame(file); //empty string to load a new game
    loading->hide();

}

GraphicService::~GraphicService(){

}

GraphicService* GraphicService::graphicServiceInstance=nullptr;



void GraphicService::setKeyboardShortcuts(int key){
    switch(key){
    case Qt::Key_Q:
        MapView::getMapView()->addRoadMode();
        break;
    case Qt::Key_R:
        MapView::getMapView()->removeBuildingMode();
        break;
    case Qt::Key_F1:
       GuiView::getGuiView()->showBuildingPickerMenu(0);
        break;
    case Qt::Key_F2:
        GuiView::getGuiView()->showBuildingPickerMenu(1);
        break;
    case Qt::Key_F3:
        GuiView::getGuiView()->showBuildingPickerMenu(2);
        break;
    case Qt::Key_F4:
        GuiView::getGuiView()->showBuildingPickerMenu(3);
        break;
    case Qt::Key_F5:
        GuiView::getGuiView()->showBuildingPickerMenu(4);
        break;
    case Qt::Key_F6:
        GuiView::getGuiView()->showBuildingPickerMenu(5);
        break;
    case Qt::Key_F7:
        GuiView::getGuiView()->showBuildingPickerMenu(6);
        break;
    case Qt::Key_F8:
        GuiView::getGuiView()->showBuildingPickerMenu(7);
        break;
    case Qt::Key_F9:
        GuiView::getGuiView()->showBuildingPickerMenu(8);
        break;
    case Qt::Key_1:
        GuiView::getGuiView()->selectBuilding(0);
        break;
    case Qt::Key_2:
        GuiView::getGuiView()->selectBuilding(1);
        break;
    case Qt::Key_3:
        GuiView::getGuiView()->selectBuilding(2);
        break;
    case Qt::Key_4:
        GuiView::getGuiView()->selectBuilding(3);
        break;
    case Qt::Key_5:
        GuiView::getGuiView()->selectBuilding(4);
        break;
    case Qt::Key_6:
        GuiView::getGuiView()->selectBuilding(5);
        break;
    case Qt::Key_7:
        GuiView::getGuiView()->selectBuilding(6);
        break;
    case Qt::Key_8:
        GuiView::getGuiView()->selectBuilding(7);
        break;
    case Qt::Key_9:
        GuiView::getGuiView()->selectBuilding(8);
        break;
    case Qt::Key_0:
        GuiView::getGuiView()->selectBuilding(9);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
         MapView::getMapView()->translateMeth(-2);
        break;
    case Qt::Key_Up:
    case Qt::Key_W:
         MapView::getMapView()->translateMeth(2);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
         MapView::getMapView()->translateMeth(1);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
         MapView::getMapView()->translateMeth(-1);
        break;
    case Qt::Key_Plus:
         MapView::getMapView()->zoomMeth(true);
        break;
    case Qt::Key_Minus:
         MapView::getMapView()->zoomMeth(false);
        break;
    case Qt::Key_G:
        MapView::getMapView()->toggleGrid();
        break;
    }
}




GraphicService* GraphicService::getGraphicService(QString file){
    if(GraphicService::graphicServiceInstance==nullptr){
        GraphicService::graphicServiceInstance=new GraphicService(file);
    }
    return GraphicService::graphicServiceInstance;
}
