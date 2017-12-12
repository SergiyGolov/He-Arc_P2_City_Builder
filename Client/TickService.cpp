#include "TickService.h"

//constructors
TickService::TickService(int tickrate)
{
    this->setInterval(tickrate);
    this->start();
}

//destructors
TickService::~TickService()
{

}

//methods
void TickService::updateHappiness()
{

}

void TickService::updateMoney()
{

}

void TickService::updatePopulation()
{

}

//methods
void TickService::timerEvent()
{
    updateHappiness();
    updateMoney();
    updatePopulation();
}
