#ifndef TICKSERVICE_H
#define TICKSERVICE_H

#include <QTimer>

class QDateTime;
class GameManagementService;
class BuildingManagementService;

class TickService : QTimer
{
public:
    static TickService* getTickService();
    int getPopulationDelta() {return newPopulation - oldPopulation;}
    int getHappinessDelta() {return newHappiness - oldHappiness;}
    int getMoneyDelta() {return newMoney - oldMoney;}
    //Dont add setters for these variables. To set them get the pointer and set it with functions (like setHMS)
    QDateTime* getStartTime() { return startDateTime; }
    QDateTime* getGameTime() { return gameDateTime; }
    void triggerUpdate(bool everything = false);
private:
    static TickService *tickService;
    TickService();
    ~TickService();

    void timerEvent(QTimerEvent *);

    int updatePopulation(bool everything = false);
    double updateHappiness(bool everything = false);
    double updateMoney(bool everything = false);

    int oldPopulation;
    double oldHappiness;
    double oldMoney;

    int newPopulation;
    double newHappiness;
    double newMoney;

    QDateTime* startDateTime;
    QDateTime* gameDateTime;
};

#endif
