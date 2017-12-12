#ifndef LOADSAVESERVICE_H
#define LOADSAVESERVICE_H

#include <QList>
class Building;
class Road;

class LoadSaveService
{
private:
    QList<Building>* listBuilding;
    QList<Road>* listRoad;
public:
//constructors
	LoadSaveService();
//destructor
	~LoadSaveService();
//methods
    void loadGame(QString filename);
    void saveGame(QString filename);
	void loadConstants();
	void saveOptions();
	void loadOptions();
//getters
    QList<Building>* getBuildingList(){return listBuilding;}
    QList<Road>* getRoadList(){return listRoad;}
//setters
};

#endif
