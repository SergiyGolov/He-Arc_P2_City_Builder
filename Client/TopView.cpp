#include "TopView.h"

#include "Services.h"
#include "ClickableLabel.h"

#include <QTime>
#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>
#include <QtWidgets>

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

    time = new QLabel("00:00 - 00.00.0000", this);
    time->setFixedSize(200,15);
    time->move(10,0);

    cityNameLabel = new ClickableLabel("ClickHereToEditTheCityName", this);
    cityNameLabel->setFixedSize(200,15);
    cityNameLabel->move((width() - cityNameLabel->width())/2,20);
    cityNameLabel->setAlignment(Qt::AlignCenter);

    cityName = new QLineEdit("ClickHereToEditTheCityName", this);
    cityName->setFixedSize(200,15);
    cityName->move((width() - cityName->width())/2,20);
    cityName->setAlignment(Qt::AlignCenter);
    cityName->setVisible(false);

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

    labelMaster = new QLabel(QString("Master"), this);
    labelMaster->move(screenWidth-630,20);

    master = new QSlider(Qt::Horizontal, this);
    master->move(screenWidth-630, 40);
    master->setMinimum(0);
    master->setMaximum(100);

    labelMusic = new QLabel(QString("Music"), this);
    labelMusic->move(screenWidth-530,20);

    music = new QSlider(Qt::Horizontal, this);
    music->move(screenWidth-530, 40);
    music->setMinimum(0);
    music->setMaximum(100);

    labelSfx = new QLabel(QString("Sfx"), this);
    labelSfx->move(screenWidth-430,20);

    sfx = new QSlider(Qt::Horizontal, this);
    sfx->move(screenWidth-430, 40);
    sfx->setMinimum(0);
    sfx->setMaximum(100);

    save = new QPushButton("&Save",this);
    save->move(screenWidth-330,20);

    load = new QPushButton("&Load",this);
    load->move(screenWidth-230,20);

    quit=new QPushButton("&Quit game",this);
    quit->move(screenWidth-130,20);

    seed = new QLabel("Seed : ", this);
    seed->setFixedWidth(200);
    seed->move(screenWidth - seed->width(), 0);

    QObject::connect(taxes, SIGNAL(valueChanged(double)), this, SLOT(taxesChanged(void)));
    QObject::connect(quit, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);
    QObject::connect(save, &QPushButton::clicked, LoadSaveService::saveGameUI);
    QObject::connect(load, &QPushButton::clicked, LoadSaveService::loadGameUI);

    QObject::connect(cityName, &QLineEdit::editingFinished, [=]() {
        GameManagementService::getGameManagementService()->setCityName(cityName->text());
        cityNameLabel->setText(cityName->text());
        cityName->setVisible(false);
        cityNameLabel->setVisible(true);
    });

    QObject::connect(cityNameLabel, &ClickableLabel::clicked, [=]() {
        cityName->setVisible(true);
        cityNameLabel->setVisible(false);
    });

    QObject::connect(master, &QSlider::valueChanged, this, &TopView::volumeMasterChanged);
    QObject::connect(music, &QSlider::valueChanged, this, &TopView::volumeMusicChanged);
    QObject::connect(sfx, &QSlider::valueChanged, this, &TopView::volumeSfxChanged);

    //Default values
    taxes->setValue(7.0);
    master->setValue(100);
    music->setValue(50);
    sfx->setValue(50);

    //Apply default values
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

void TopView::setTime(QDateTime* time)
{
    this->time->setText(time->toString(QString("hh:mm %1 dd.MM.yyyy").arg("on")));
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

void TopView::setCityName(){
    cityNameLabel->setText(GameManagementService::getGameManagementService()->getCityName());
}

void TopView::update()
{
    setMoney(GameManagementService::getGameManagementService()->getMoney());
    setHappiness(GameManagementService::getGameManagementService()->getHappiness());
    setPopulation(GameManagementService::getGameManagementService()->getTotalPopulation());
    setTime(TickService::getTickService()->getGameTime());
    setPopulationDelta(TickService::getTickService()->getPopulationDelta());
    setHappinessDelta(TickService::getTickService()->getHappinessDelta());
    setMoneyDelta(TickService::getTickService()->getMoneyDelta());
    seed->setText(QString("Seed : %1").arg(RandomService::getSeed()));


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
    AudioService::getAudioService()->setVolumeMaster(master->value() / 100.00);
}

void TopView::volumeMusicChanged()
{
    labelMusic->setText(QString("Music (%1\%)").arg(music->value()));
    AudioService::getAudioService()->setVolumeMusic(music->value() / 100.00);
}

void TopView::volumeSfxChanged()
{
    labelSfx->setText(QString("Sfx (%1\%)").arg(sfx->value()));
    AudioService::getAudioService()->setVolumeSfx(sfx->value() / 100.00);
}

void TopView::keyPressEvent(QKeyEvent *event)
{
    GraphicService::getGraphicService()->setKeyboardShortcuts(event->key());
}
