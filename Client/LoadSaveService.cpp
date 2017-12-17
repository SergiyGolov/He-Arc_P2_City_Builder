#include "LoadSaveService.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>

#include <QDebug>

#include "BuildingManagementService.h"

//constructors
LoadSaveService::LoadSaveService()
{

}

//destructor
LoadSaveService::~LoadSaveService()
{

}

//methods
void LoadSaveService::loadGame(QString filename)
{

}

void LoadSaveService::saveGame(QString filename)
{
    QFile saveFile(filename);

    if (!saveFile.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

    QJsonDocument jd;
    QJsonObject jo;
        QJsonObject settings;
            QJsonObject viewenable;
            QJsonObject audio;
        QJsonObject gamedata;
            QJsonObject levelsize;

    viewenable.insert(QString("view1"), QJsonValue(true));
    viewenable.insert(QString("view2"), QJsonValue(true));
    viewenable.insert(QString("view3"), QJsonValue(true));
    viewenable.insert(QString("view4"), QJsonValue(true));
    viewenable.insert(QString("view5"), QJsonValue(true));

    audio.insert(QString("master"), 100);
    audio.insert(QString("music"), 50);
    audio.insert(QString("sfx"), 25);

    levelsize.insert(QString("w"), 64);
    levelsize.insert(QString("h"), 128);

    settings.insert(QString("savename"), QJsonValue("Zooland1"));
    settings.insert(QString("mapseed"), QJsonValue(1));
    settings.insert(QString("zoom"), QJsonValue(2));
    settings.insert(QString("timespeed"), QJsonValue(3));
    settings.insert(QString("viewenable"), viewenable);
    settings.insert(QString("audio"), audio);

    gamedata.insert(QString("cityname"), QJsonValue("Zooland"));
    gamedata.insert(QString("money"), QJsonValue(123456));
    gamedata.insert(QString("igtime"), QJsonValue(2));
    gamedata.insert(QString("firstsave"), QJsonValue("2017-10-31T09:19:00Z"));
    gamedata.insert(QString("lastsave"), QJsonValue("2017-10-31T09:19:00Z"));
    gamedata.insert(QString("countsave"), QJsonValue(1));
    gamedata.insert(QString("levelsize"), levelsize);
    gamedata.insert(QString("items"), BuildingManagementService::getBuildingManagementService()->getJsonBuildings());

    jo.insert(QString("settings"), settings);
    jo.insert(QString("gamedata"), gamedata);

    jd.setObject(jo);
    QTextStream out(&saveFile);
    out << jd.toJson();
    //qDebug() << jd.toJson();
}

void LoadSaveService::saveOptions()
{

}

void LoadSaveService::loadOptions()
{

}
