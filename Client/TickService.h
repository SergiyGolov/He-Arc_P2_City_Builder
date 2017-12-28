#ifndef TICKSERVICE_H
#define TICKSERVICE_H

#include <QTimer>

class GameManagementService;
class BuildingManagementService;

class TickService : QTimer
{
public:
    static TickService* getTickService();
    int getPopulationDelta() {return newPopulation - oldPopulation;}
    int getHappinessDelta() {return newHappiness - oldHappiness;}
    int getMoneyDelta() {return newMoney - oldMoney;}
private:
    static TickService *tickService;
    TickService();
    ~TickService();

    void timerEvent(QTimerEvent *);

    int updatePopulation();
    double updateHappiness();
    double updateMoney();

    int oldPopulation;
    double oldHappiness;
    double oldMoney;

    int newPopulation;
    double newHappiness;
    double newMoney;
};

#endif
