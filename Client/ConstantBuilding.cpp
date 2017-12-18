#include "ConstantBuilding.h"

int ConstantBuilding::nbBuildings = 0;
ConstantBuilding* ConstantBuilding::tabGet = nullptr;

//constructors
ConstantBuilding::ConstantBuilding()
{

}

ConstantBuilding::ConstantBuilding (
    QString displayName,
    int category,
    double price,
    int tileWidth,
    int tileHeight,
    QList<QPoint>* ignoredTile,
    bool requirementType,
    QSet<int>* requirements,
    int sumRequirements
    )
{
    this->DISPLAY_NAME = displayName;
    this->CATEGORY = category;
    this->PRICE = price;
    this->TILE_WIDTH = tileWidth;
    this->TILE_HEIGHT = tileHeight;
    this->IGNORED_TILE = ignoredTile;
    this->REQUIREMENTS_TYPE = requirementType;
    this->REQUIREMENTS = requirements;
    this->SUM_REQUIREMENTS = sumRequirements;

    nbBuildings++; //To keep a track of the number of buildings
}

//destructors
ConstantBuilding::~ConstantBuilding()
{

}

//static
ConstantBuilding ConstantBuilding::get(int i)
{
    generate();
    return tabGet[i];
}

int ConstantBuilding::getNbBuildings()
{
    generate();
    return nbBuildings;
}


int ConstantBuilding::getPopulationFromHouseType(int i){

    switch (i){
    case bID::HouseLowDensityPoorClass:
    case bID::HouseLowDensityMiddleClass:
    case bID::HouseLowDensityWealthyClass:
            return 1;
    case bID::HouseAverageDensityPoorClass:
    case bID::HouseAverageDensityMiddleClass:
    case bID::HouseAverageDensityWealthyClass:
            return 2;
    case bID::HouseHighDensityPoorClass:
    case bID::HouseHighDensityMiddleClass:
    case bID::HouseHighDensityWealthyClass:
            return 4;
    default:
            return 0;
    }
}

void ConstantBuilding::printTab()
{
    generate();
    qDebug() << "displayname" << "\t"
             << "categories" << "\t"
             << "efficiancy" << "\t"
             << "price" << "\t"
             << "radius";
    for(int i = 0; i < getNbBuildings(); i++)
    {
        qDebug() << get(i).getDisplayName() << "\t"
                 << get(i).getCategory() << "\t"
                 << get(i).getEfficiancy() << "\t\t"
                 << get(i).getPrice() << "\t"
                 << get(i).getRadius();
    }
}

/*
 * Generate the whole 'object array of constants'
 */
void ConstantBuilding::generate()
{
    if(tabGet == nullptr) //Singleton
    {
        tabGet = new ConstantBuilding[bID::Fountains+1]; //Last element of the enum plus one
        //Only prices, categories and sizes are implemented

        tabGet[bID::Road] = ConstantBuilding(QString("Road"),0,10,1,1, NULL, false, NULL, 0);
        tabGet[bID::HouseLowDensityPoorClass] = ConstantBuilding(QString("House Low Density $"),1,0,1,1, NULL, false, NULL, 0);
        tabGet[bID::HouseLowDensityMiddleClass] = ConstantBuilding(QString("House Low Density $$"),1,0,1,1, NULL, false, NULL, 0);
        tabGet[bID::HouseLowDensityWealthyClass] = ConstantBuilding(QString("House Low Density $$$"),1,0,1,1, NULL, false, NULL, 0);
        tabGet[bID::HouseAverageDensityPoorClass] = ConstantBuilding(QString("House Average Density $"),1,0,2,1, NULL, false, NULL, 0);
        tabGet[bID::HouseAverageDensityMiddleClass] = ConstantBuilding(QString("House Average Density $$"),1,0,2,1, NULL, false, NULL, 0);
        tabGet[bID::HouseAverageDensityWealthyClass] = ConstantBuilding(QString("House Average Density $$$"),1,0,2,1, NULL, false, NULL, 0);
        tabGet[bID::HouseHighDensityPoorClass] = ConstantBuilding(QString("House High Density $"),1,0,2,2, NULL, false, NULL, 0);
        tabGet[bID::HouseHighDensityMiddleClass] = ConstantBuilding(QString("House High Density $$"),1,0,2,2, NULL, false, NULL, 0);
        tabGet[bID::HouseHighDensityWealthyClass] = ConstantBuilding(QString("House High Density $$$"),1,0,2,2, NULL, false, NULL, 0);
        tabGet[bID::TownHall] = ConstantBuilding(QString("Town Hall"),2,0,2,1, NULL, false, NULL, 0);
        tabGet[bID::Clinic] = ConstantBuilding(QString("Clinic"),3,100,1,1, NULL, false, NULL, 0);
        tabGet[bID::Hospital] = ConstantBuilding(QString("Hospital"),3,500,2,2, NULL, false, NULL, 0);
        tabGet[bID::Graveyards] = ConstantBuilding(QString("Graveyard"),3,200,1,1, NULL, false, NULL, 0);
        tabGet[bID::Crematory] = ConstantBuilding(QString("Crematory"),3,50,1,1, NULL, false, NULL, 0);
        tabGet[bID::Sauna] = ConstantBuilding(QString("Sauna"),3,50,1,1, NULL, false, NULL, 0);
        tabGet[bID::MedicalLaboratory] = ConstantBuilding(QString("Medical Laboratory"),3,200,1,1, NULL, false, NULL, 0);
        tabGet[bID::MedicalHelicopterAirport] = ConstantBuilding(QString("Medical Helicopter Airport"),3,300,2,2, NULL, false, NULL, 0);
        tabGet[bID::Firehouse] = ConstantBuilding(QString("Firehouse"),4,150,2,2, NULL, false, NULL, 0);
        tabGet[bID::FirehouseHelicopterAirport] = ConstantBuilding(QString("Firehouse Helicopter Airport"),4,300,2,2, NULL, false, NULL, 0);
        tabGet[bID::FirehouseTower] = ConstantBuilding(QString("Firehouse Tower"),4,50,1,1, NULL, false, NULL, 0);
        tabGet[bID::CatastropheTeam] = ConstantBuilding(QString("Catastrophe Team"),4,50,1,1, NULL, false, NULL, 0);
        tabGet[bID::Bunker] = ConstantBuilding(QString("Bunker"),4,100,1,1, NULL, false, NULL, 0);
        tabGet[bID::RadioAntenna] = ConstantBuilding(QString("Radio Antenna"),4,50,1,1, NULL, false, NULL, 0);
        tabGet[bID::PoliceOffice] = ConstantBuilding(QString("Police Office"),5,100,1,1, NULL, false, NULL, 0);
        tabGet[bID::PoliceHeadquarter] = ConstantBuilding(QString("Police Headquarters"),5,500,2,2, NULL, false, NULL, 0);
        tabGet[bID::Jail] = ConstantBuilding(QString("Jail"),5,200,2,1, NULL, false, NULL, 0);
        tabGet[bID::PoliceHelicopterAirport] = ConstantBuilding(QString("Police Helicopter Airport"),5,300,2,2, NULL, false, NULL, 0);
        tabGet[bID::SecurityAgency] = ConstantBuilding(QString("Security Agency"),5,50,1,1, NULL, false, NULL, 0);
        tabGet[bID::ElementarySchool] = ConstantBuilding(QString("Elementary School"),6,100,1,1, NULL, false, NULL, 0);
        tabGet[bID::MiddleSchool] = ConstantBuilding(QString("Middle School"),6,200,1,1, NULL, false, NULL, 0);
        tabGet[bID::TechnicalCollege] = ConstantBuilding(QString("Technical College"),6,300,2,2, NULL, false, NULL, 0);
        tabGet[bID::HighSchool] = ConstantBuilding(QString("High School"),6,400,3,1, NULL, false, NULL, 0);
        tabGet[bID::College] = ConstantBuilding(QString("College"),6,500,2,2, NULL, false, NULL, 0);
        tabGet[bID::Faculty] = ConstantBuilding(QString("Faculty"),6,500,2,2, NULL, false, NULL, 0);
        tabGet[bID::TaxiStation] = ConstantBuilding(QString("Taxi Station"),7,100,1,1, NULL, false, NULL, 0);
        tabGet[bID::BusDepot] = ConstantBuilding(QString("Bus Depot"),7,200,2,2, NULL, false, NULL, 0);
        tabGet[bID::BusStation] = ConstantBuilding(QString("Bus Station"),7,500,1,1, NULL, false, NULL, 0);
        tabGet[bID::BusStop] = ConstantBuilding(QString("Bus Stop"),7,20,1,1, NULL, false, NULL, 0);
        tabGet[bID::Restaurant] = ConstantBuilding(QString("Restaurant"),8,100,1,1, NULL, false, NULL, 0);
        tabGet[bID::Mall] = ConstantBuilding(QString("Mall"),8,300,3,3, NULL, false, NULL, 0);
        tabGet[bID::Library] = ConstantBuilding(QString("Library"),8,200,1,2, NULL, false, NULL, 0);
        tabGet[bID::Park] = ConstantBuilding(QString("Park"),8,100,4,4, NULL, false, NULL, 0);
        tabGet[bID::PlayArea] = ConstantBuilding(QString("Play Area"),8,200,1,2, NULL, false, NULL, 0);
        tabGet[bID::BotanicalGarden] = ConstantBuilding(QString("Botanical Garden"),8,300,2,1, NULL, false, NULL, 0);
        tabGet[bID::AmusementPark] = ConstantBuilding(QString("Amusement Park"),8,2000,5,7, NULL, false, NULL, 0);
        tabGet[bID::BasketField] = ConstantBuilding(QString("Basket Field"),8,200,1,2, NULL, false, NULL, 0);
        tabGet[bID::BeachVolleyField] = ConstantBuilding(QString("Beach Volleyball Field"),8,200,2,1, NULL, false, NULL, 0);
        tabGet[bID::FootballField] = ConstantBuilding(QString("Football Field"),8,200,3,2, NULL, false, NULL, 0);
        tabGet[bID::FishingField] = ConstantBuilding(QString("Fishing Field"),8,100,1,1, NULL, false, NULL, 0);
        tabGet[bID::Barn] = ConstantBuilding(QString("Barn"),8,50,1,1, NULL, false, NULL, 0);
        tabGet[bID::Skatepark] = ConstantBuilding(QString("Skatepark"),8,200,1,2, NULL, false, NULL, 0);
        tabGet[bID::FootballStadium] = ConstantBuilding(QString("Football Stadium"),9,2000,4,3, NULL, false, NULL, 0);
        tabGet[bID::OlympiqueStatium] = ConstantBuilding(QString("Olympic Statium"),9,2000,3,4, NULL, false, NULL, 0);
        tabGet[bID::EiffelTower] = ConstantBuilding(QString("Eiffel Tower"),9,2000,5,5, NULL, false, NULL, 0);
        tabGet[bID::Statue] = ConstantBuilding(QString("Statue"),9,1000,4,4, NULL, false, NULL, 0);
        tabGet[bID::Memorial] = ConstantBuilding(QString("Memorial"),9,2000,5,5, NULL, false, NULL, 0);
        tabGet[bID::Fountains] = ConstantBuilding(QString("Fountains"),9,1000,4,4, NULL, false, NULL, 0);
    }
}
