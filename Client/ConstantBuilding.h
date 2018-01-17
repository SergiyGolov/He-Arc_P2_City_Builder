/*
 * ************************************* Project *************************************
 * School              : Haute Ecole Arc
 * Module              : Projet P2
 * Teachers            : - Beurret Stéphane <Stephane.Beurret@he-arc.ch>
 *                       - Grunenwald David <David.Grunenwald@he-arc.ch>
 * Group               : CityBuilder
 * Group participant   : - Goloviatinski Sergiy <sergiy.goloviatinski@he-arc.ch>
 *                       - Margueron Raphael <raphael.margueron@he-arc.ch>
 *                       - Petroff Damian <damian.petroff@he-arc.ch>
 * ************************************ File spec ************************************
 * Workpackage manager : Margueron Raphael
 * Description         :
 * This class is used for the template of the buildings. It contains every attributs
 * that are related to every building of the same type. An enum is declared in the
 * namespace bID.
 * ***********************************************************************************
 */

#ifndef CONSTANTBUILDING_H
#define CONSTANTBUILDING_H

#include <QString>
#include <QList>
#include <QPoint>
#include <QDebug>
#include <QSet>
#include "OursMaths.h"

class ConstantBuilding
{
private:
//attributs
    QString DISPLAY_NAME;
    int CATEGORY;
	
    double PRICE;
	
    int TILE_WIDTH;
    int TILE_HEIGHT;
    QList<QPoint>* IGNORED_TILE;
	
    bool REQUIREMENTS_TYPE; //False : OR, True : AND
    QSet<int>* REQUIREMENTS;
    double SUM_REQUIREMENTS;
	
    //Calculated attributs
    double PRICE_PER_SECONDS;
    double EFFICIANCY;
    double RADIUS;
    double REQUIREMENTS_WEIGHT;

	//TODO : Texture...

    static void addRequirements(QSet<int>* requirements, int r1, int r2 = -1, int r3 = -1, int r4 = -1, int r5 = -1);

    static int nbBuildings;
    static ConstantBuilding* tabGet;

//constructors
    ConstantBuilding();
    ConstantBuilding (
        QString displayName,
        int category,
        double price,
        int tileWidth,
        int tileHeight,
        QList<QPoint>* ignoredTile,
        bool requirementType,
        QSet<int>* requirements,
        double sumRequirements
        );

public:
//destructors
    ~ConstantBuilding();
//getters
    QString         getDisplayName()        {return DISPLAY_NAME;}

    int             getCategory()           {return CATEGORY;}

    double          getPrice()              {return PRICE;}
    double          getPricePerSeconds()    {return PRICE_PER_SECONDS;}
    double          getEfficiancy()         {return EFFICIANCY;}
    double          getRadius()             {return RADIUS;}
    double          getRequirementsWeight() {return REQUIREMENTS_WEIGHT;}

    int             getTileWidth()          {return TILE_WIDTH;}
    int             getTileHeight()         {return TILE_HEIGHT;}
    QList<QPoint>*  getIgnoredTile()        {return IGNORED_TILE;}

    bool            getRequirementsType()   {return REQUIREMENTS_TYPE;}
    QSet<int>*      getRequirements()       {return REQUIREMENTS;}
    double          getSumRequirements()    {return SUM_REQUIREMENTS;}

//methods statics
    static bool isHouse(int id);
    static void generate();

    static ConstantBuilding get(int i);
    static int getNbBuildings();

    static int getDefaultPopulationFromHouseType(int i);
};

namespace bID
{
    enum
    {
        Road,
        HouseLowDensityPoorClass,
        HouseLowDensityMiddleClass,
        HouseLowDensityWealthyClass,
        HouseAverageDensityPoorClass,
        HouseAverageDensityMiddleClass,
        HouseAverageDensityWealthyClass,
        HouseHighDensityPoorClass,
        HouseHighDensityMiddleClass,
        HouseHighDensityWealthyClass,
        TownHall,
        Clinic,
        Hospital,
        Graveyards,
        Crematory,
        Sauna,
        MedicalLaboratory,
        MedicalHelicopterAirport,
        Firehouse,
        FirehouseHelicopterAirport,
        FirehouseTower,
        CatastropheTeam,
        Bunker,
        RadioAntenna,
        PoliceOffice,
        PoliceHeadquarter,
        Jail,
        PoliceHelicopterAirport,
        SecurityAgency,
        ElementarySchool,
        MiddleSchool,
        TechnicalCollege,
        HighSchool,
        College,
        Faculty,
        TaxiStation,
        BusDepot,
        BusStation,
        BusStop,
        Restaurant,
        Mall,
        Library,
        Park,
        PlayArea,
        BotanicalGarden,
        AmusementPark,
        BasketField,
        BeachVolleyField,
        FootballField,
        FishingField,
        Barn,
        Skatepark,
        FootballStadium,
        OlympiqueStatium,
        EiffelTower,
        Statue,
        Memorial,
        Fountains,
    };
}

#endif
