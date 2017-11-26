#include "ConstantBuilding.h"

//constructors
ConstantBuilding::ConstantBuilding()
{

}

ConstantBuilding::ConstantBuilding(QString* displayName)
{
    this->DISPLAY_NAME = displayName;
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

    tabConstantBuilding[bID::Road]                              = ConstantBuilding(new QString("Route"), 0, 10, 1, 1, NULL, false, NULL, 0); //test values
    tabConstantBuilding[bID::HouseLowDensityPoorClass] 			= ConstantBuilding(new QString("Maison $"), 1, 0, 1, 1, NULL, false, NULL, 0); //test values
    tabConstantBuilding[bID::HouseLowDensityMiddleClass] 		= ConstantBuilding();
    tabConstantBuilding[bID::HouseLowDensityWealthyClass]		= ConstantBuilding();
    tabConstantBuilding[bID::HouseAverageDensityPoorClass]		= ConstantBuilding();
    tabConstantBuilding[bID::HouseAverageDensityMiddleClass]    = ConstantBuilding();
    tabConstantBuilding[bID::HouseAverageDensityWealthyClass]	= ConstantBuilding();
    tabConstantBuilding[bID::HouseHighDensityPoorClass]			= ConstantBuilding();
    tabConstantBuilding[bID::HouseHighDensityMiddleClass]		= ConstantBuilding();
    tabConstantBuilding[bID::HouseHighDensityWealthyClass]		= ConstantBuilding();
    tabConstantBuilding[bID::TownHall]							= ConstantBuilding();
    tabConstantBuilding[bID::Clinic]                            = ConstantBuilding();
    tabConstantBuilding[bID::Hospital]							= ConstantBuilding();
    tabConstantBuilding[bID::Graveyards]                        = ConstantBuilding();
    tabConstantBuilding[bID::Crematory]							= ConstantBuilding();
    tabConstantBuilding[bID::Sauna]								= ConstantBuilding();
    tabConstantBuilding[bID::MedicalLaboratory]					= ConstantBuilding();
    tabConstantBuilding[bID::MedicalHelicopterAirport] 			= ConstantBuilding();
    tabConstantBuilding[bID::Firehouse]                         = ConstantBuilding();
    tabConstantBuilding[bID::FirehouseHelicopterAirport] 		= ConstantBuilding();
    tabConstantBuilding[bID::FirehouseTower] 					= ConstantBuilding();
    tabConstantBuilding[bID::CatastropheTeam] 					= ConstantBuilding();
    tabConstantBuilding[bID::Bunker] 							= ConstantBuilding();
    tabConstantBuilding[bID::RadioAntenna] 						= ConstantBuilding();
    tabConstantBuilding[bID::PoliceOffice] 						= ConstantBuilding();
    tabConstantBuilding[bID::PoliceHeadquarter]                 = ConstantBuilding();
    tabConstantBuilding[bID::Jail] 								= ConstantBuilding();
    tabConstantBuilding[bID::PoliceHelicopterAirport] 			= ConstantBuilding();
    tabConstantBuilding[bID::SecurityAgency] 					= ConstantBuilding();
    tabConstantBuilding[bID::ElementarySchool] 					= ConstantBuilding();
    tabConstantBuilding[bID::MiddleSchool] 						= ConstantBuilding();
    tabConstantBuilding[bID::TechnicalCollege] 					= ConstantBuilding();
    tabConstantBuilding[bID::HighSchool] 						= ConstantBuilding();
    tabConstantBuilding[bID::College] 							= ConstantBuilding();
    tabConstantBuilding[bID::Faculty] 							= ConstantBuilding();
    tabConstantBuilding[bID::TaxiStation] 						= ConstantBuilding();
    tabConstantBuilding[bID::BusDepot] 							= ConstantBuilding();
    tabConstantBuilding[bID::BusStation] 						= ConstantBuilding();
    tabConstantBuilding[bID::BusStop] 							= ConstantBuilding();
    tabConstantBuilding[bID::Restaurant] 						= ConstantBuilding();
    tabConstantBuilding[bID::Mall] 								= ConstantBuilding();
    tabConstantBuilding[bID::Library] 							= ConstantBuilding();
    tabConstantBuilding[bID::Park] 								= ConstantBuilding();
    tabConstantBuilding[bID::PlayArea] 							= ConstantBuilding();
    tabConstantBuilding[bID::BotanicalGarden] 					= ConstantBuilding();
    tabConstantBuilding[bID::AmusementPark]             		= ConstantBuilding();
    tabConstantBuilding[bID::BasketField] 						= ConstantBuilding();
    tabConstantBuilding[bID::BeachVolleyField] 					= ConstantBuilding();
    tabConstantBuilding[bID::FootballField]                 	= ConstantBuilding();
    tabConstantBuilding[bID::FishingField] 						= ConstantBuilding();
    tabConstantBuilding[bID::Barn] 								= ConstantBuilding();
    tabConstantBuilding[bID::Skatepark]                     	= ConstantBuilding();
    tabConstantBuilding[bID::FootballStadium] 					= ConstantBuilding();
    tabConstantBuilding[bID::OlympiqueStatium] 					= ConstantBuilding();
    tabConstantBuilding[bID::EiffelTower] 						= ConstantBuilding();
    tabConstantBuilding[bID::Statue] 							= ConstantBuilding();
    tabConstantBuilding[bID::Memorial] 							= ConstantBuilding();
    tabConstantBuilding[bID::Fountains]							= ConstantBuilding();

    return tabConstantBuilding;
}
