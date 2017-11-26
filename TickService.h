#ifndef TICKSERVICE_H
#define TICKSERVICE_H

#include <QTimer>

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
	void timerEvent();
//getters

//setters
};

#endif
