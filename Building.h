#ifndef BUILDING_H
#define BUILDING_H
#include <QWidget>

//Last updated : 21.11.2017 by DP
/**
 * @brief The Building class
 */
class Building
{
public:
    static int buildingsCount;  //Total buildings counter
protected:
//attributes
    int id;               //Identifier of the building (unique)
    int angle;            //Defines the 4 possibilities of rotation on the building (0, 1, 2 or 3. 0 by default)
    int buildingTypeId;   //Identifier of the building type (not unique)
    QPoint coordinates;   //Map origin point of the building (top-left for angle=0)

public:
//constructor
    Building(QPoint coordinates, int angle);

//destructor
    ~Building();

//methods
    virtual void remove()=0;      //Removes the building in parameter

    //getters
    static int getBuildingCount(){return buildingsCount;} //Returns the total buildings count
    int getId(){return id;}
    int getBuildingTypeId(){return buildingTypeId;}
    int getAngle(){return angle;}
    QPoint getCoordinates(){return coordinates;}

    //setters
    /*
    void setId(int value){this->id = value;}
    void setCoordinates(QPoint value){this->coordinates = value;}
    void setBuidingTypeId(int value){this->buildingTypeId = value;}
    void setAngle(int value){this->angle = value;}
    */
};



#endif // BUILDING_H
