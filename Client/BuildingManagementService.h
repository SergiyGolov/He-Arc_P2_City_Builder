#ifndef BUILDINGMANAGEMENTSERVICE_H
#define BUILDINGMANAGEMENTSERVICE_H

class ConstantBuilding;
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
    double getAverageHappiness();


    int getVectorId(int uid,QVector<Building*> *v);
private:
    static BuildingManagementService* buildingManagementService;
    BuildingManagementService();
    QVector<Building*>* vectorBuildings;

    bool bSumPricePerSeconds;
    bool bSumPopulation;
    bool bAverageHappiness;

    double pricePerSeconds;
    int sumPopulation;
    double averageHappiness;
};

#endif // BUILDINGMANAGEMENTSERVICE_H
