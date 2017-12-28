#ifndef BUILDING_H
#define BUILDING_H
#include <QWidget>

class ConstantBuilding;
//Last updated : 21.11.2017 by DP
/**
 * @brief The Building class
 */
class Building
{
    static int uid_cpt;
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
    int getUid(){return uid;}
    int getId(){return id;}
    int getAngle(){return angle;}
    int getX(){return x;}
    int getY(){return y;}
    int getPopulation(){return population;}
    double getHappiness();
};



#endif // BUILDING_H
