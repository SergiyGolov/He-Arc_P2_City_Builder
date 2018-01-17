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
 * Workpackage manager : Damian Petroff
 * Description         :
 * This class is used to create a single Buidling with his properties and is mainly
 * mainly used in the BuildingManagmentService
 * ***********************************************************************************
 */
#pragma once
#include <QWidget>

class ConstantBuilding;
//Last updated : 21.11.2017 by DP
/**
 * @brief The Building class
 */
class Building
{
    static int uIDCpt;
protected:
//attributes
    int uid;        //Identifier of the building (unique)
    int id;         //Identifier of the building type (not unique)
    int angle;      //Defines the 4 possibilities of rotation on the building (0, 1, 2 or 3. 0 by default)
    int x;
    int y;
    int population;
public:
//constructor
    Building(int id, int x, int y, int angle);
//destructor
    ~Building();
//methods
    //getters
    static void setUIDCpt(int uIDCpt) { uIDCpt = uIDCpt; }
    static int getUIDCpt() {return uIDCpt; }

    int getUid(){return uid;}
    int getId(){return id;}
    int getAngle(){return angle;}
    int getX(){return x;}
    int getY(){return y;}
    int getPopulation(){return population;}
    void setAngle(int newAngle){this->angle=newAngle;}
};
