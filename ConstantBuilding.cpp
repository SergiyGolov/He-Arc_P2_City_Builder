#include "ConstantBuilding.h"

//constructors
ConstantBuilding::ConstantBuilding()
{

}

ConstantBuilding::ConstantBuilding (
    QString* displayName,
    int category,
    double price,
    int tileWidth,
    int tileHeight,
    QList<QPoint>* ignoredTile,
    bool requirementType,
    QList<int>* requirements,
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
}

//destructors
ConstantBuilding::~ConstantBuilding()
{

}


ConstantBuilding* ConstantBuilding::getBuildingArray()
{
    //I have to fill it with the data of the excel sheet
    ConstantBuilding* tabConstantBuilding = new ConstantBuilding[bID::Fountains-1]; //Last element of the enum minus one

    tabConstantBuilding[bID::Road] = ConstantBuilding(new QString("Road"),0,10,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::HouseLowDensityPoorClass] = ConstantBuilding(new QString("House Low Density $"),1,0,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::HouseLowDensityMiddleClass] = ConstantBuilding(new QString("House Low Density $$"),1,0,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::HouseLowDensityWealthyClass] = ConstantBuilding(new QString("House Low Density $$$"),1,0,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::HouseAverageDensityPoorClass] = ConstantBuilding(new QString("House Average Density $"),1,0,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::HouseAverageDensityMiddleClass] = ConstantBuilding(new QString("House Average Density $$"),1,0,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::HouseAverageDensityWealthyClass] = ConstantBuilding(new QString("House Average Density $$$"),1,0,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::HouseHighDensityPoorClass] = ConstantBuilding(new QString("House Height Density $"),1,0,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::HouseHighDensityMiddleClass] = ConstantBuilding(new QString("House Height Density $$"),1,0,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::HouseHighDensityWealthyClass] = ConstantBuilding(new QString("House Height Density $$$"),1,0,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::TownHall] = ConstantBuilding(new QString("Town Hall"),2,0,2,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Clinic] = ConstantBuilding(new QString("Clinic"),3,100,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Hospital] = ConstantBuilding(new QString("Hospital"),3,500,2,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Graveyards] = ConstantBuilding(new QString("Graveyards"),3,200,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Crematory] = ConstantBuilding(new QString("Crematory"),3,50,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Sauna] = ConstantBuilding(new QString("Sauna"),3,50,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::MedicalLaboratory] = ConstantBuilding(new QString("Medical Laboratory"),3,200,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::MedicalHelicopterAirport] = ConstantBuilding(new QString("Medical Helicopter Airport"),3,300,2,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Firehouse] = ConstantBuilding(new QString("Firehouse"),4,150,2,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::FirehouseHelicopterAirport] = ConstantBuilding(new QString("Firehouse Helicopter Airport"),4,300,2,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::FirehouseTower] = ConstantBuilding(new QString("Firehouse Tower"),4,50,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::CatastropheTeam] = ConstantBuilding(new QString("Catastrophe Team"),4,50,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Bunker] = ConstantBuilding(new QString("Bunker"),4,100,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::RadioAntenna] = ConstantBuilding(new QString("Radio Antenna"),4,50,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::PoliceOffice] = ConstantBuilding(new QString("Police Office"),5,100,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::PoliceHeadquarter] = ConstantBuilding(new QString("Police Headquarter"),5,500,2,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Jail] = ConstantBuilding(new QString("Jail"),5,200,2,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::PoliceHelicopterAirport] = ConstantBuilding(new QString("Police Helicopter Airport"),5,300,2,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::SecurityAgency] = ConstantBuilding(new QString("Security Agency"),5,50,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::ElementarySchool] = ConstantBuilding(new QString("Elementary School"),6,100,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::MiddleSchool] = ConstantBuilding(new QString("Middle School"),6,200,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::TechnicalCollege] = ConstantBuilding(new QString("Technical College"),6,300,2,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::HighSchool] = ConstantBuilding(new QString("High School"),6,400,3,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::College] = ConstantBuilding(new QString("College"),6,500,2,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Faculty] = ConstantBuilding(new QString("Faculty"),6,500,2,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::TaxiStation] = ConstantBuilding(new QString("Taxi Station"),7,100,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::BusDepot] = ConstantBuilding(new QString("Bus Depot"),7,200,2,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::BusStation] = ConstantBuilding(new QString("Bus Station"),7,500,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::BusStop] = ConstantBuilding(new QString("Bus Stop"),7,20,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Restaurant] = ConstantBuilding(new QString("Restaurant"),8,100,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Mall] = ConstantBuilding(new QString("Mall"),8,300,3,3, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Library] = ConstantBuilding(new QString("Library"),8,200,1,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Park] = ConstantBuilding(new QString("Park"),8,100,4,4, NULL, false, NULL, 0);
    tabConstantBuilding[bID::PlayArea] = ConstantBuilding(new QString("Play Area"),8,200,1,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::BotanicalGarden] = ConstantBuilding(new QString("Botanical Garden"),8,300,2,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::AmusementPark] = ConstantBuilding(new QString("Amusement Park"),8,2000,5,7, NULL, false, NULL, 0);
    tabConstantBuilding[bID::BasketField] = ConstantBuilding(new QString("Basket Field"),8,200,1,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::BeachVolleyField] = ConstantBuilding(new QString("Beach Volleyball Field"),8,200,2,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::FootballField] = ConstantBuilding(new QString("Football Field"),8,200,3,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::FishingField] = ConstantBuilding(new QString("Fishing Field"),8,100,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Barn] = ConstantBuilding(new QString("Barn"),8,50,1,1, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Skatepark] = ConstantBuilding(new QString("Skatepark"),8,200,1,2, NULL, false, NULL, 0);
    tabConstantBuilding[bID::FootballStadium] = ConstantBuilding(new QString("Football Stadium"),9,2000,4,3, NULL, false, NULL, 0);
    tabConstantBuilding[bID::OlympiqueStatium] = ConstantBuilding(new QString("Olympique Statium"),9,2000,3,4, NULL, false, NULL, 0);
    tabConstantBuilding[bID::EiffelTower] = ConstantBuilding(new QString("Eiffel Tower"),9,2000,5,5, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Statue] = ConstantBuilding(new QString("Statue"),9,1000,4,4, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Memorial] = ConstantBuilding(new QString("Memorial"),9,2000,5,5, NULL, false, NULL, 0);
    tabConstantBuilding[bID::Fountains] = ConstantBuilding(new QString("Fountains"),9,1000,4,4, NULL, false, NULL, 0);

    return tabConstantBuilding;
}
