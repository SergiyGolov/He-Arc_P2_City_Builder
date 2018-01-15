#include "Building.h"

#include "ConstantBuilding.h"

int Building::uIDCpt=0;

Building::Building(int id, int x, int y, int angle)
{
    this->uid = ++uIDCpt;
    this->id = id;
    this->x = x;
    this->y = y;
    this->angle = angle;
    this->population = ConstantBuilding::getDefaultPopulationFromHouseType(id);
}

Building::~Building()
{
    //Do dot decrease the uid_cpt, because the uid wont be unique anymore !
}
