#ifndef TICKSERVICE_H
#define TICKSERVICE_H

#include <QTimer>

class GameManagementService;
class BuildingManagementService;

class TickService : QTimer
{
private:
//methods
    int updatePopulation();
    double updateHappiness();
    double updateMoney();

    int oldPopulation;
    double oldHappiness;
    double oldMoney;

    int newPopulation;
    double newHappiness;
    double newMoney;
public:
//constructors
	TickService(int tickrate);
//destructors
	~TickService();
//methods
    void timerEvent(QTimerEvent *);
    int getPopulationDelta() {return newPopulation - oldPopulation;}
    int getHappinessDelta() {return newHappiness - oldHappiness;}
    int getMoneyDelta() {return newMoney - oldMoney;}
//getters

//setters
};

#endif
