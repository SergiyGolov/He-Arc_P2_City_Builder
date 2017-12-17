#ifndef LOADSAVESERVICE_H
#define LOADSAVESERVICE_H

#include <QList>

class LoadSaveService
{
private:

public:
//constructors
	LoadSaveService();
//destructor
	~LoadSaveService();
//methods
    static void loadGame(QString filename);
    static void saveGame(QString filename);
    static void saveOptions();
    static void loadOptions();
};

#endif
