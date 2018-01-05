#include "TopView.h"

#include "TickService.h"
#include "GraphicService.h"
#include "GameManagementService.h"
#include "Services.h"
#include "AudioService.h"

#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QSlider>

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

    money = new QLabel("Money: 1000",this);
    money->setFixedSize(150,15);
    money->move(10,20);

    moneyDelta = new QLabel("+0",this);
    moneyDelta->setFixedSize(150,15);
    moneyDelta->move(10,40);

    happiness = new QLabel("Happiness: 0",this);
    happiness->setFixedSize(200,15);
    happiness->move(150,20);

    happinessDelta = new QLabel("+0",this);
    happinessDelta->setFixedSize(150,15);
    happinessDelta->move(150,40);

    population=new QLabel("Population: 0",this);
    population->setFixedSize(200,15);
    population->move(275,20);

    populationDelta = new QLabel("+0",this);
    populationDelta->setFixedSize(150,15);
    populationDelta->move(275,40);

    labelTaxes = new QLabel("Taxes: ",this);
    labelTaxes->move(400,20);

    taxes = new QDoubleSpinBox(this);
    taxes->move(450, 20);

    save = new QPushButton("&Save",this);
    save->move(screenWidth-330,20);

    load = new QPushButton("&Load",this);
    load->move(screenWidth-230,20);

    quit=new QPushButton("&Quit game",this);
    quit->move(screenWidth-130,20);

    labelMaster = new QLabel("Music", this);
    labelMaster->move(screenWidth-730, 20);

    labelMusic = new QLabel("Music", this);
    labelMusic->move(screenWidth-630, 20);

    labelSfx = new QLabel("Sfx", this);
    labelSfx->move(screenWidth-530, 20);

    master = new QSlider(Qt::Horizontal, this);
    master->move(screenWidth-730, 40);
    master->setMinimum(0);
    master->setMaximum(100);

    music = new QSlider(Qt::Horizontal, this);
    music->move(screenWidth-630, 40);
    music->setMinimum(0);
    music->setMaximum(100);

    sfx = new QSlider(Qt::Horizontal, this);
    sfx->move(screenWidth-530, 40);
    sfx->setMinimum(0);
    sfx->setMaximum(100);

    QObject::connect(master, &QSlider::valueChanged, this, &TopView::volumeMasterChanged);
    QObject::connect(music, &QSlider::valueChanged, this, &TopView::volumeMusicChanged);
    QObject::connect(sfx, &QSlider::valueChanged, this, &TopView::volumeSfxChanged);

    QObject::connect(taxes, SIGNAL(valueChanged(double)), this, SLOT(taxesChanged(void)));

    QObject::connect(quit, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);
    QObject::connect(save, &QPushButton::clicked, LoadSaveService::saveGameUI);
    QObject::connect(load, &QPushButton::clicked, LoadSaveService::loadGameUI);

    //Default values
    taxes->setValue(7.0);
    master->setValue(100);
    music->setValue(50);
    sfx->setValue(50);

    //Applying default values
    taxesChanged();
    volumeMasterChanged();
    volumeMusicChanged();
    volumeSfxChanged();
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

void TopView::setPopulationDelta(int delta)
{
    this->populationDelta->setText(deltaFormat((double)delta));
}

void TopView::setHappinessDelta(double delta)
{
    this->happinessDelta->setText(deltaFormat(delta));
}

void TopView::setMoneyDelta(double delta)
{
    this->moneyDelta->setText(deltaFormat(delta));
}

void TopView::update()
{
    setMoney(GameManagementService::getGameManagementService()->getMoney());
    setHappiness(GameManagementService::getGameManagementService()->getHappiness());
    setPopulation(GameManagementService::getGameManagementService()->getTotalPopulation());
    setPopulationDelta(TickService::getTickService()->getPopulationDelta());
    setHappinessDelta(TickService::getTickService()->getHappinessDelta());
    setMoneyDelta(TickService::getTickService()->getMoneyDelta());
}

QString TopView::deltaFormat(double delta)
{
    QString s;
    if(delta >= 0)
        s.append('+');
    s.append(QString("%1").arg(delta));
    return s;
}

void TopView::taxesChanged()
{
    GameManagementService::getGameManagementService()->setTaxes(taxes->value());
}

void TopView::volumeMasterChanged()
{
    labelMaster->setText(QString("Master (%1\%)").arg(master->value()));
    AudioService::getAudioService()->setVolumeMaster(master->value() / 100.0);
}

void TopView::volumeMusicChanged()
{
    labelMusic->setText(QString("Music (%1\%)").arg(music->value()));
    AudioService::getAudioService()->setVolumeMusic(music->value() / 100.0);
}

void TopView::volumeSfxChanged()
{
    labelSfx->setText(QString("Sfx (%1\%)").arg(sfx->value()));
    AudioService::getAudioService()->setVolumeSfx(sfx->value() / 100.0);
}

void TopView::keyPressEvent(QKeyEvent *event)
{
    GraphicService::getGraphicService()->setKeyboardShortcuts(event->key());
}
