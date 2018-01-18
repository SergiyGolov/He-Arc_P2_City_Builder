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
 * This class is used to manage the global array of Building objects it contains
 * specifics function to handle the array and have so calculus with it
 * ***********************************************************************************
 */
#pragma once

class ConstantBuilding;
class QJsonObject;

#include "Client/Building.h"
#include <QVector>

class BuildingManagementService
{
public:
    static BuildingManagementService* getBuildingManagementService();
    void addBuilding(int id, int x, int y, int angle);
    void removeBuilding(int uid);
    bool isBuildingAddable(int id);
    int countBuildings() {return vectorBuildings->count();}
    double getSumPricePerSeconds(bool everything = false);
    int getSumPopulation(bool everything = false);
    QJsonObject getJsonBuildings();
    double getAverageHappiness(bool everything = false);
    int getVectorId(int uid,QVector<Building*> *v);
    void addBuildingFromSave(int id, int x, int y, int angle);
    void setAngleFromId(int uid,int newAngle){vectorBuildings->at(getVectorId(uid,vectorBuildings))->setAngle(newAngle);}
    QVector<Building*> *getEffectBuildings();
	
private:
    static BuildingManagementService* buildingManagementService;
    BuildingManagementService();
    QVector<Building*>* vectorBuildings;
    QVector<Building*>* vectorEffectBuildings;

    double getHappiness(Building* b);

    bool bSumPricePerSeconds;
    bool bSumPopulation;
    bool bAverageHappiness;

    double pricePerSeconds;
    int sumPopulation;
    double averageHappiness;
};
