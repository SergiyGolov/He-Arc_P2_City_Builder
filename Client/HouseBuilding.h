#ifndef HOUSEBUILDING_H
#define HOUSEBUILDING_H
#include "Building.h"

//Last updated : 21.11.2017 by DP

class HouseBuilding : public Building
{
private:
    static int houseBuildingsCount;
    static int globalPopulation;
    static int globalPopulationMax;

//attributes
    int population;         //actual population of the house.
    int populationMax;      //maximum population of the house.

public:
//constructors
    HouseBuilding(QPoint coordinates, int buildingTypeId, int angle);
//destructor
    ~HouseBuilding();

//methods
    void remove();
    void increasePopulation(int i); //TODO : test max
    static getMaximumPopulationFromBuilding(int buildingTypeId);

    //getters
    static int getHouseBuildingCount(){return houseBuildingsCount;}
    int getPopulation(){return population;}
    int getPopulationMax(){return populationMax;}

    //setters
    /*
    void setPopulation(int value){this->population=value;}
    void setPopulationMax(int value){this->populationMax=value;}
    */
};

#endif // HOUSEBUILDING_H
