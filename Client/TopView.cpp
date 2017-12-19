#include "TopView.h"
#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>
#include "GraphicService.h"
#include "GameManagementService.h"

TopView* TopView::topViewInstance=nullptr;

TopView* TopView::getTopView()
{
    if(TopView::topViewInstance==nullptr)
        TopView::topViewInstance=new TopView();
    return TopView::topViewInstance;
}

TopView::TopView(QWidget *parent) : QGraphicsView(parent)
{
this->scene=new QGraphicsScene(this);
    this->setScene(scene);

    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );


    this->setBackgroundBrush(QBrush(Qt::lightGray));

    this->setMouseTracking(true);
    screenWidth=QApplication::desktop()->screenGeometry().width();
    screenHeight=QApplication::desktop()->screenGeometry().height();

    this->setGeometry(0,0,screenWidth,50);

    money=new QLabel("Money: 1000",this);
    money->move(10,20);
    money->setFixedSize(150,15);

    happiness=new QLabel("Happiness: 0",this);
    happiness->move(150,20);

    quit=new QPushButton("&Quit game",this);
    quit->move(screenWidth-120,20);

    population=new QLabel("Population: 0",this);
    population->setFixedSize(200,15);
    population->move(275,20);
    QLabel *labelTaxes=new QLabel("Taxes: ",this);
    labelTaxes->move(400,20);
    taxes = new QDoubleSpinBox(this);
    taxes->move(450, 20);
    taxes->setValue(7.0);

    //TODO : syntaxe...
     QObject::connect(taxes, SIGNAL(valueChanged(double)), this, SLOT(taxesChanged(void)));

     taxesChanged(); //we must call it the first time before the user changes the taxes

    QObject::connect(quit, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);
}

TopView::~TopView()
{

}

void TopView::setMoney(int money)
{
    this->money->setText(QString("Money: %1").arg(money));
}

void TopView::setHappiness(int happiness)
{
    this->happiness->setText(QString("Happiness: %1").arg(happiness));
}

void TopView::setPopulation(int population)
{
    this->population->setText(QString("Population: %1").arg(population));
}

void TopView::taxesChanged()
{
    GameManagementService::getGameManagementService()->setTaxes(taxes->value());
}

void TopView::keyPressEvent(QKeyEvent *event){
    GraphicService::getGraphicService()->setKeyboardShortcuts(event->key());


}
