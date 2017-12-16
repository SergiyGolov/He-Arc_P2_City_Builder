#include "MainWidget.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include "QDebug"
#include "TopView.h"


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    this->setPalette(pal);


    gui=GuiView::getGuiView();
    map=MapView::getMapView();

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(TopView::getTopView(),1);
    vbox->addWidget(map,50);
    vbox->addWidget(gui,12);


}

MainWidget* MainWidget::mainWidgetInstance=nullptr;

MainWidget::~MainWidget()
{

}

void MainWidget::showBuildingPickerMenu(int tabId){
    gui->showBuildingPickerMenu(tabId);
}


void MainWidget::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_1:
        showBuildingPickerMenu(0);
        break;
    case Qt::Key_2:
        showBuildingPickerMenu(1);
        break;
    case Qt::Key_3:
        showBuildingPickerMenu(2);
        break;
    case Qt::Key_4:
        showBuildingPickerMenu(3);
        break;
    case Qt::Key_5:
        showBuildingPickerMenu(4);
        break;
    case Qt::Key_6:
        showBuildingPickerMenu(5);
        break;
    case Qt::Key_7:
        showBuildingPickerMenu(6);
        break;
    case Qt::Key_8:
        showBuildingPickerMenu(7);
        break;
    case Qt::Key_9:
        showBuildingPickerMenu(8);
        break;
    }
}


MainWidget* MainWidget::getMainWidget(){
    if(MainWidget::mainWidgetInstance==nullptr){
        MainWidget::mainWidgetInstance=new MainWidget();
    }

    return MainWidget::mainWidgetInstance;

}
