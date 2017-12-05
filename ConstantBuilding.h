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

class QString;
#include <QList>
class QPoint;

class ConstantBuilding
{
private:
//attributs
    QString* DISPLAY_NAME;
    int CATEGORY;
	
    double PRICE;
	
    int TILE_WIDTH;
    int TILE_HEIGHT;
    QList<QPoint>* IGNORED_TILE;
	
    bool REQUIREMENTS_TYPE; //True : AND, False : OR
    QList<int>* REQUIREMENTS;
    int SUM_REQUIREMENTS;
	
	//TODO : Texture...

//constructors
    ConstantBuilding();
    ConstantBuilding(QString* displayName);
    ConstantBuilding (
        QString* displayName,
        int category,
        double price,
        int tileWidth,
        int tileHeight,
        QList<QPoint>* ignoredTile,
        bool requirementType,
        QList<int>* requirements,
        int sumRequirements
        );

public:

//destructors
	~ConstantBuilding();
//getters
    QString* getDisplayName(){return DISPLAY_NAME;}
    int getCategory(){return CATEGORY;}
	
    //These getter have to be updated cf. excel sheet
    double getPrice(){return PRICE;}
    double getPricePerSeconds(){return PRICE;}
    double getEfficiancy(){return PRICE;}
    double getRadius(){return PRICE;}
    double getRequirementsWeight(){return PRICE;}

    int getTileWidth(){return TILE_WIDTH;}
    int getTileHeight(){return TILE_HEIGHT;}
    QList<QPoint>* getIgnoredTile(){return IGNORED_TILE;}
	
    bool getRequirementsType(){return REQUIREMENTS_TYPE;}
    QList<int>* getRequirements(){return REQUIREMENTS;}
    int getSumRequirements(){return SUM_REQUIREMENTS;}
//methods
    static ConstantBuilding* getBuildingArray();
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
