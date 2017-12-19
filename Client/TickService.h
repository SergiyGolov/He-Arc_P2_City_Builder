#ifndef TICKSERVICE_H
#define TICKSERVICE_H

#include <QTimer>

class GameManagementService;
class BuildingManagementService;

class TickService : QTimer
{
private:
//methods
	void updateHappiness();
	void updateMoney();
	void updatePopulation();

    int oldPopulation;
    int oldHappiness;
    int oldMoney;

    int newPopulation;
    int newHappiness;
    int newMoney;
public:
//constructors
	TickService(int tickrate);
//destructors
	~TickService();
//methods
    void timerEvent(QTimerEvent *);
    int getMoneyDelta() {return newMoney - oldMoney;}
    int getHappinessDelta() {return newHappiness - oldHappiness;}
    int getPopulationDelta() {return newPopulation - oldPopulation;}
//getters

//setters
};

#endif
