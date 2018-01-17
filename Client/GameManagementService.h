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

#include <QList>
#include "Client/TopView.h"

class ConstantBuilding;
class TickService;

class GameManagementService
{
private:
    //constructor
    static GameManagementService* gameManagementServiceInstance;
    GameManagementService();
    int money;
    int totalPopulation;
    int happiness;
    double taxes;
    QString cityName;
    int saveCount;
    int difficulty;
public:
    static GameManagementService* getGameManagementService();
    //destructor
    ~GameManagementService();
    //methods
    void exitGame();
    //getters
    int getMoney(){return money;}
    int getHappiness(){return happiness;}
    int getTotalPopulation(){return totalPopulation;}
    int getTaxes(){return taxes;}
    int getDifficulty() {return difficulty; }
    int incrementAndGetSaveCount() {return ++saveCount;}
    QString getCityName(){return cityName;}
    //setters
    void setHappiness(int newHappiness);
    void setMoney(int newMoney);
    void setTotalPopulation(int newTotalPopulation);
    void setTaxes(double newTaxes);
    void setCityName(QString cityName) { this->cityName = cityName;
                                      TopView::getTopView()->setCityName();}
    void setSaveCount(int saveCount) { this->saveCount = saveCount; }
    void setDifficulty(int difficulty);

};
