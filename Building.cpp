#include "building.h"

int Building::buildingsCount=0;

Building::Building(QPoint coordinates, int angle)
{
    this->id = buildingsCount++;
    this->coordinates = coordinates;
    this->angle = angle;
}

Building::~Building()
{
    buildingsCount--;
}
