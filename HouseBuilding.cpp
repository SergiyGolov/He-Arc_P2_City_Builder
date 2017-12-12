#include "HouseBuilding.h"

int HouseBuilding::globalPopulation=0;
int HouseBuilding::globalPopulationMax=0;
int HouseBuilding::houseBuildingsCount=0;

HouseBuilding::HouseBuilding(QPoint coordinates, int buildingTypeId, int angle) : Building(coordinates, angle)
{
    this->id =1 ;
    this->coordinates = coordinates;
    this->buildingTypeId = buildingTypeId;
    this->angle = angle;
    this->population = 2; ///À discuter : 1 ou 2 comme population de départ ?
    //populationMax = ConstantBuilding::getMaximumPopulation(buildingTypeId);
    houseBuildingsCount++;
    globalPopulation+=population;
    globalPopulationMax+=populationMax;
}

HouseBuilding::~HouseBuilding()
{
    houseBuildingsCount--;
    globalPopulation-=population;
    globalPopulationMax-=populationMax;
}

void HouseBuilding::remove()
{
    ///TODO : Supression graphique de l'élément
    delete this;
}

int HouseBuilding::getMaximumPopulationFromBuilding(int buildingTypeId)
{
    ///TODO
    return 1;
}
