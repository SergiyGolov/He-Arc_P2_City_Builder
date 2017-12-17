#include "Building.h"


int Building::uid_cpt=0;

Building::Building(int id, int x, int y, int angle, int population)
{
    this->uid = ++uid_cpt;
   // qDebug()<<"unique id from Building.cpp: "<<uid;
    this->id = id;
    this->x = x;
    this->y = y;
    this->angle = angle;
    this->population = population;
}

Building::~Building()
{
    //Do dot decrease the uid_cpt, because the uid wont be unique anymore !
}
