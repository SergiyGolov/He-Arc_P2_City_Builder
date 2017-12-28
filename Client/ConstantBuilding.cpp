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
    double sumRequirements
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


int ConstantBuilding::getDefaultPopulationFromHouseType(int i){

    switch (i)
    {
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

void ConstantBuilding::addRequirements(QSet<int>* requirements, int r1, int r2, int r3, int r4, int r5)
{
    requirements->insert(r1);
    if(r2 != -1)
        requirements->insert(r2);
    if(r3 != -1)
        requirements->insert(r3);
    if(r4 != -1)
        requirements->insert(r4);
    if(r5 != -1)
        requirements->insert(r5);
}

/*
 * Generate the whole 'object array of constants'
 */
void ConstantBuilding::generate()
{
    if(tabGet == nullptr) //Singleton
    {
        tabGet = new ConstantBuilding[bID::Fountains+1]; //Last element of the enum plus one

        QString displayName;
        int category;
        double price;
        int tileWidth;
        int tileHeight;
        QList<QPoint>* ignoredTile;
        bool requirementType;
        QSet<int>* requirements;
        double sumRequirements;

        /*
         * Categories are set on the top of the block
         * The array is filled just after declaring each properties of the building,
         * it allows us to reuse the same variables names,
         * for the pointer it also works
         * there is an exception for the displayname the QString object is passed by copy, w/e
        */

        category = 0;
        {
            /// Road ///
            displayName     = QString("Road");
            price           = 10;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::Road] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);
        }

        category = 1;
        {
            /// HouseLowDensityPoorClass ///
            displayName     = QString("House Low Density $");
            price           = 0;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::HouseLowDensityPoorClass] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// HouseLowDensityMiddleClass ///
            displayName     = QString("House Low Density $$");
            price           = 0;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::HouseLowDensityMiddleClass] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// HouseLowDensityWealthyClass ///
            displayName     = QString("House Low Density $$$");
            price           = 0;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::HouseLowDensityWealthyClass] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// HouseAverageDensityPoorClass ///
            displayName     = QString("House Average Density $");
            price           = 0;
            tileWidth       = 2;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::HouseAverageDensityPoorClass] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// HouseAverageDensityMiddleClass ///
            displayName     = QString("House Average Density $$");
            price           = 0;
            tileWidth       = 2;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::HouseAverageDensityMiddleClass] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// HouseAverageDensityWealthyClass ///
            displayName     = QString("House Average Density $$$");
            price           = 0;
            tileWidth       = 2;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::HouseAverageDensityWealthyClass] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// HouseHighDensityPoorClass ///
            displayName     = QString("House High Density $");
            price           = 0;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::HouseHighDensityPoorClass] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// HouseHighDensityMiddleClass ///
            displayName     = QString("House High Density $$");
            price           = 0;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::HouseHighDensityMiddleClass] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// HouseHighDensityWealthyClass ///
            displayName     = QString("House High Density $$$");
            price           = 0;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::HouseHighDensityWealthyClass] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);
        }

        category = 2;
        {
            /// TownHall ///
            displayName     = QString("Town Hall");
            price           = 0;
            tileWidth       = 2;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::TownHall] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);
        }

        category = 3;
        {
            /// Clinic ///
            displayName     = QString("Clinic");
            price           = 100;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::Clinic] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Hospital ///
            displayName     = QString("Hospital");
            price           = 500;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::Hospital] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Graveyards ///
            displayName     = QString("Graveyards");
            price           = 200;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::Graveyards] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Crematory ///
            displayName     = QString("Crematory");
            price           = 50;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::Graveyards);
            sumRequirements = tabGet[bID::Graveyards].getRequirementsWeight();
            tabGet[bID::Crematory] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Sauna ///
            displayName     = QString("Sauna");
            price           = 50;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::Sauna] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// MedicalLaboratory ///
            displayName     = QString("Medical Laboratory");
            price           = 200;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::Clinic, bID::Hospital);
            sumRequirements = 1 * tabGet[bID::Clinic].getRequirementsWeight() + 1 * tabGet[bID::Hospital].getRequirementsWeight();;
            tabGet[bID::MedicalLaboratory] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// MedicalHelicopterAirport ///
            displayName     = QString("Medical Helicopter Airport");
            price           = 300;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::Hospital);
            sumRequirements = 5;
            tabGet[bID::MedicalHelicopterAirport] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);
        }

        category = 4;
        {
            /// Firehouse ///
            displayName     = QString("Firehouse");
            price           = 150;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::Firehouse] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// FirehouseHelicopterAirport ///
            displayName     = QString("Firehouse Helicopter Airport");
            price           = 300;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::Firehouse);
            sumRequirements = 1 * tabGet[bID::Firehouse].getRequirementsWeight();
            tabGet[bID::FirehouseHelicopterAirport] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// FirehouseTower ///
            displayName     = QString("Firehouse Tower");
            price           = 50;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::Firehouse);
            sumRequirements = 1 * tabGet[bID::Firehouse].getRequirementsWeight();
            tabGet[bID::FirehouseTower] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// CatastropheTeam ///
            displayName     = QString("Catastrophe Team");
            price           = 50;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::Firehouse, bID::FirehouseHelicopterAirport, bID::FirehouseTower, bID::Bunker);
            sumRequirements = 3;
            tabGet[bID::CatastropheTeam] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Bunker ///
            displayName     = QString("Bunker");
            price           = 100;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::Firehouse, bID::FirehouseHelicopterAirport, bID::FirehouseTower, bID::CatastropheTeam);
            sumRequirements = 3;
            tabGet[bID::Bunker] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// RadioAntenna ///
            displayName     = QString("Radio Antenna");
            price           = 50;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::Firehouse);
            sumRequirements = tabGet[bID::Firehouse].getRequirementsWeight();
            tabGet[bID::RadioAntenna] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);
        }

        category = 5;
        {
            /// PoliceOffice ///
            displayName     = QString("Police Office");
            price           = 100;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::PoliceOffice] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// PoliceHeadquarter ///
            displayName     = QString("Police Headquarters");
            price           = 500;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::PoliceHeadquarter] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Jail ///
            displayName     = QString("Jail");
            price           = 200;
            tileWidth       = 2;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::PoliceOffice, bID::PoliceHeadquarter);
            sumRequirements = tabGet[bID::PoliceOffice].getRequirementsWeight() + tabGet[bID::PoliceHeadquarter].getRequirementsWeight();
            tabGet[bID::Jail] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// PoliceHelicopterAirport ///
            displayName     = QString("Police Helicopter Airport");
            price           = 300;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::PoliceHeadquarter);
            sumRequirements = tabGet[bID::PoliceHeadquarter].getRequirementsWeight();
            tabGet[bID::PoliceHelicopterAirport] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// SecurityAgency ///
            displayName     = QString("Security Agency");
            price           = 50;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::Jail, bID::PoliceHelicopterAirport);
            sumRequirements = tabGet[bID::Jail].getRequirementsWeight() + tabGet[bID::PoliceHelicopterAirport].getRequirementsWeight();
            tabGet[bID::SecurityAgency] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);
        }

        category = 6;
        {
            /// ElementarySchool ///
            displayName     = QString("Elementary School");
            price           = 100;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::ElementarySchool] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// MiddleSchool ///
            displayName     = QString("Middle School");
            price           = 200;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::ElementarySchool);
            sumRequirements = tabGet[bID::ElementarySchool].getRequirementsWeight();
            tabGet[bID::MiddleSchool] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// TechnicalCollege ///
            displayName     = QString("Technical College");
            price           = 300;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::MiddleSchool);
            sumRequirements = tabGet[bID::MiddleSchool].getRequirementsWeight();
            tabGet[bID::TechnicalCollege] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// HighSchool ///
            displayName     = QString("High School");
            price           = 400;
            tileWidth       = 3;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::TechnicalCollege);
            sumRequirements = tabGet[bID::TechnicalCollege].getRequirementsWeight();
            tabGet[bID::HighSchool] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// College ///
            displayName     = QString("College");
            price           = 500;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::HighSchool);
            sumRequirements = tabGet[bID::HighSchool].getRequirementsWeight();
            tabGet[bID::College] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Faculty ///
            displayName     = QString("Faculty");
            price           = 500;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::HighSchool);
            sumRequirements = tabGet[bID::HighSchool].getRequirementsWeight();
            tabGet[bID::Faculty] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);
        }

        category = 7;
        {
            /// TaxiStation ///
            displayName     = QString("Taxi Station");
            price           = 100;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::TaxiStation] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// BusDepot ///
            displayName     = QString("Bus Depot");
            price           = 200;
            tileWidth       = 2;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::BusDepot] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// BusStation ///
            displayName     = QString("Bus Station");
            price           = 500;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::BusStation] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// BusStop ///
            displayName     = QString("Bus Stop");
            price           = 20;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::BusDepot, bID::BusStation);
            sumRequirements = tabGet[bID::BusDepot].getRequirementsWeight() + tabGet[bID::BusStation].getRequirementsWeight();
            tabGet[bID::BusStop] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);
        }

        category = 8;
        {
            /// Restaurant ///
            displayName     = QString("Restaurant");
            price           = 100;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::Restaurant] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Mall ///
            displayName     = QString("Mall");
            price           = 300;
            tileWidth       = 3;
            tileHeight      = 3;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::Mall] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Library ///
            displayName     = QString("Library");
            price           = 200;
            tileWidth       = 1;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::Library] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Park ///
            displayName     = QString("Park");
            price           = 100;
            tileWidth       = 4;
            tileHeight      = 4;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::Park] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// PlayArea ///
            displayName     = QString("Play Area");
            price           = 200;
            tileWidth       = 1;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::PlayArea] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// BotanicalGarden ///
            displayName     = QString("Botanical Garden");
            price           = 300;
            tileWidth       = 2;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::Park);
            sumRequirements = 3 * tabGet[bID::Park].getRequirementsWeight();
            tabGet[bID::BotanicalGarden] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// AmusementPark ///
            displayName     = QString("Amusement Park");
            price           = 2000;
            tileWidth       = 5;
            tileHeight      = 7;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::PlayArea);
            sumRequirements = 3 * tabGet[bID::PlayArea].getRequirementsWeight();
            tabGet[bID::AmusementPark] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// BasketField ///
            displayName     = QString("Basket Field");
            price           = 200;
            tileWidth       = 1;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::BasketField] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// BeachVolleyField ///
            displayName     = QString("Beach Volley Field");
            price           = 200;
            tileWidth       = 2;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::BeachVolleyField] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// FootballField ///
            displayName     = QString("Football Field");
            price           = 200;
            tileWidth       = 3;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::FootballField] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// FishingField ///
            displayName     = QString("Fishing Field");
            price           = 100;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::FishingField] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Barn ///
            displayName     = QString("Barn");
            price           = 50;
            tileWidth       = 1;
            tileHeight      = 1;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::Barn] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Skatepark ///
            displayName     = QString("Skatepark");
            price           = 200;
            tileWidth       = 1;
            tileHeight      = 2;
            ignoredTile     = nullptr;
            requirementType = false;
            requirements    = nullptr;
            sumRequirements = 0;
            tabGet[bID::Skatepark] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);
        }

        category = 9;
        {
            /// FootballStadium ///
            displayName     = QString("Football Stadium");
            price           = 2000;
            tileWidth       = 4;
            tileHeight      = 3;
            ignoredTile     = nullptr;
            requirementType = true;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::BasketField, bID::BeachVolleyField, bID::FootballField);
            sumRequirements = tabGet[bID::BasketField].getRequirementsWeight() + tabGet[bID::BeachVolleyField].getRequirementsWeight() + tabGet[bID::FootballField].getRequirementsWeight();
            tabGet[bID::FootballStadium] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// OlympiqueStatium ///
            displayName     = QString("Olympique Statium");
            price           = 2000;
            tileWidth       = 3;
            tileHeight      = 4;
            ignoredTile     = nullptr;
            requirementType = true;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::BasketField, bID::BeachVolleyField, bID::FootballField);
            sumRequirements = tabGet[bID::BasketField].getRequirementsWeight() + tabGet[bID::BeachVolleyField].getRequirementsWeight() + tabGet[bID::FootballField].getRequirementsWeight();
            tabGet[bID::OlympiqueStatium] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// EiffelTower ///
            displayName     = QString("Eiffel Tower");
            price           = 2000;
            tileWidth       = 5;
            tileHeight      = 5;
            ignoredTile     = nullptr;
            requirementType = true;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::Hospital, bID::Firehouse, bID::PoliceHeadquarter);
            sumRequirements = tabGet[bID::Hospital].getRequirementsWeight() + tabGet[bID::Firehouse].getRequirementsWeight() + tabGet[bID::PoliceHeadquarter].getRequirementsWeight();
            tabGet[bID::EiffelTower] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Statue ///
            displayName     = QString("Statue");
            price           = 1000;
            tileWidth       = 4;
            tileHeight      = 4;
            ignoredTile     = nullptr;
            requirementType = true;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::College, bID::BusStation, bID::Park);
            sumRequirements = tabGet[bID::College].getRequirementsWeight() + tabGet[bID::BusStation].getRequirementsWeight() + tabGet[bID::Park].getRequirementsWeight();
            tabGet[bID::Statue] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Memorial ///
            displayName     = QString("Memorial");
            price           = 2000;
            tileWidth       = 5;
            tileHeight      = 5;
            ignoredTile     = nullptr;
            requirementType = true;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::College, bID::BusStation, bID::Park);
            sumRequirements = tabGet[bID::College].getRequirementsWeight() + tabGet[bID::BusStation].getRequirementsWeight() + tabGet[bID::Park].getRequirementsWeight();
            tabGet[bID::Memorial] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);

            /// Fountains ///
            displayName     = QString("Fountains");
            price           = 1000;
            tileWidth       = 4;
            tileHeight      = 4;
            ignoredTile     = nullptr;
            requirementType = true;
            requirements    = new QSet<int>();
            addRequirements(requirements, bID::MedicalHelicopterAirport, bID::FirehouseHelicopterAirport, bID::PoliceHelicopterAirport);
            sumRequirements = tabGet[bID::MedicalHelicopterAirport].getRequirementsWeight() + tabGet[bID::FirehouseHelicopterAirport].getRequirementsWeight() + tabGet[bID::PoliceHelicopterAirport].getRequirementsWeight();
            tabGet[bID::Fountains] = ConstantBuilding(displayName, category, price, tileWidth, tileHeight, ignoredTile, requirementType, requirements, sumRequirements);
        }
    }
}
