#ifndef LAUNCHERSERVICE_H
#define LAUNCHERSERVICE_H

class QString;
#include <QVector>

class LauncherService
{
private:
//methods
    void newGame(QString gameName, QString cityName, QVector<int> mapSize);
	void showOptionsMenu();
public:
//constructors
	LauncherService();
//destructors
    ~LauncherService();
//getters

//setters
		
};

#endif
