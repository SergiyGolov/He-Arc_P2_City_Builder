#include <QDebug>
#include <QVBoxLayout>
#include <QKeyEvent>

#include "MainWidget.h"
#include "TopView.h"
#include "GraphicService.h"

//variables statics
MainWidget* MainWidget::mainWidgetInstance=nullptr;

MainWidget* MainWidget::getMainWidget()
{
    if(mainWidgetInstance == nullptr)
        mainWidgetInstance = new MainWidget();
    return mainWidgetInstance;
}

MainWidget::MainWidget(QWidget *parent): QWidget(parent)
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

MainWidget::~MainWidget()
{

}

void MainWidget::showBuildingPickerMenu(int tabId)
{
    gui->showBuildingPickerMenu(tabId);
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
   GraphicService::getGraphicService()->setKeyboardShortcuts(event->key());
}
