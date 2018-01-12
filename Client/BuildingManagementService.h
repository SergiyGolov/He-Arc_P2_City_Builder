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

#ifndef BUILDINGMANAGEMENTSERVICE_H
#define BUILDINGMANAGEMENTSERVICE_H

class ConstantBuilding;
class QJsonObject;
#include "Building.h"
#include <QVector>

class BuildingManagementService
{
public:
    static BuildingManagementService* getBuildingManagementService();
    void addBuilding(int id, int x, int y, int angle);
    void removeBuilding(int uid);
    bool isBuildingAddable(int id);
    int countBuildings() {return vectorBuildings->count();}
    double getSumPricePerSeconds();
    int getSumPopulation();
    QJsonObject getJsonBuildings();
    double getAverageHappiness();
    int getVectorId(int uid,QVector<Building*> *v);

    void setAngleFromId(int uid,int newAngle){vectorBuildings->at(uid)->setAngle(newAngle);}
	
private:
    static BuildingManagementService* buildingManagementService;
    BuildingManagementService();
    QVector<Building*>* vectorBuildings;

    double getHappiness(Building* b);

    bool bSumPricePerSeconds;
    bool bSumPopulation;
    bool bAverageHappiness;

    double pricePerSeconds;
    int sumPopulation;
    double averageHappiness;
};

#endif // BUILDINGMANAGEMENTSERVICE_H
