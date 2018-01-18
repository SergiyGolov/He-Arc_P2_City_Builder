#include "Client/MainWidget.h"

#include "Client/TopView.h"
#include "Client/GraphicService.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QKeyEvent>


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

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(TopView::getTopView(),1);
    vbox->addWidget( MapView::getMapView(),50);
    vbox->addWidget(GuiView::getGuiView(),12);
}

MainWidget::~MainWidget()
{

}

void MainWidget::showBuildingPickerMenu(int tabId)
{
    GuiView::getGuiView()->showBuildingPickerMenu(tabId);
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
   GraphicService::getGraphicService()->setKeyboardShortcuts(event->key());
}
