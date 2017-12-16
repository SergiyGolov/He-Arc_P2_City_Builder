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
public:
//constructors
	TickService(int tickrate);
//destructors
	~TickService();
//methods
    void timerEvent(QTimerEvent *);
//getters

//setters
};

#endif
