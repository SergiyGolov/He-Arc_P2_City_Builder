/*
 * ************************************* Project *************************************
 * School              : Haute Ecole Arc
 * Module              : Projet P2
 * Teachers            : - Beurret Stéphane <Stephane.Beurret@he-arc.ch>
 *                       - Grunenwald David <David.Grunenwald@he-arc.ch>
 * Group               : CityBuilder
 * Group participant   : - Goloviatinski Sergiy <sergiy.goloviatinski@he-arc.ch>
 *                       - Margueron Raphael <raphael.margueron@he-arc.ch>
 *                       - Petroff Damian <damian.petroff@he-arc.ch>
 * ************************************ File spec ************************************
 * Workpackage manager :
 * Description         :
 * ***********************************************************************************
 */
#pragma once

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
