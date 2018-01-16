#include "LoadSaveService.h"
#include "Services.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>

#include <QFileDialog>

#include <QDateTime>

//methods
void LoadSaveService::loadGameUI()
{
    QFile* loadfile = new QFile();
    QString filename = QFileDialog::getOpenFileName(nullptr, tr("Load file"), QString("."), tr("City Builder Saves (*.cbsave *.save *.cb *.json)"));
    loadfile->setFileName(filename);
    loadfile->open(QIODevice::ReadOnly);

    GraphicService::getGraphicService()->enableLoadingMessage();
    loadGame(loadfile);
    GraphicService::getGraphicService()->disableLoadingMessage();
    loadfile->close();
    delete loadfile;
}

void LoadSaveService::saveGameUI()
{
    QFile* savefile = new QFile();
    QString filename = QFileDialog::getSaveFileName(nullptr, tr("Save file"), QString("."), tr("City Builder Saves (*.cbsave *.save *.cb *.json)"));
    savefile->setFileName(filename);
    savefile->open(QIODevice::WriteOnly);

    saveGame(savefile);

    savefile->close();
    delete savefile;
}


void LoadSaveService::loadGame(QFile* loadFile)
{

    QString s = loadFile->readAll();

    if(s.length()>0){
        QJsonDocument jd = QJsonDocument::fromJson(s.toUtf8());
        QJsonObject jo = jd.object();
        QJsonObject generation = jo.value(QString("generation")).toObject();
        QJsonObject mapsize = generation.value(QString("mapsize")).toObject();
        QJsonObject igconfig = jo.value(QString("igconfig")).toObject();
        QJsonObject camera = igconfig.value(QString("camera")).toObject();
        QJsonObject position = camera.value(QString("position")).toObject();

        QJsonObject gamedata = jo.value(QString("gamedata")).toObject();
        QJsonObject savedata = jo.value(QString("savedata")).toObject();

        MapView::getMapView()->setNbTiles(mapsize.value(QString("h")).toInt());
        MapView::getMapView()->setZoomFactor(camera.value(QString("zoom")).toDouble());
        MapView::getMapView()->setCameraX(position.value(QString("x")).toInt());
        MapView::getMapView()->setCameraY(position.value(QString("y")).toInt());


        GameManagementService::getGameManagementService()->setCityName(gamedata.value(QString("cityname")).toString());
        GameManagementService::getGameManagementService()->setMoney(gamedata.value(QString("money")).toInt());
        GameManagementService::getGameManagementService()->setTaxes(gamedata.value(QString("taxes")).toDouble());

        Building::setUIDCpt(gamedata.value(QString("lastuid")).toInt());

        RandomService::setSeed(generation.value(QString("mapseed")).toInt());

        MapView::getMapView()->generateMap();

        QJsonObject buildings=gamedata.value(QString("uid")).toObject();

        foreach(const QString& key,buildings.keys())
        {
            QJsonObject building=buildings.value(key).toObject();
            int bId=building.value(QString("id")).toInt();
            int x=building.value(QString("x")).toInt();
            int y=building.value(QString("y")).toInt();
            int angle=building.value(QString("angle")).toInt();
            int population=building.value(QString("population")).toInt();

            BuildingManagementService::getBuildingManagementService()->addBuildingFromSave(bId,x,y,angle);
            MapView::getMapView()->addBuildingFromSave(bId,x,y,angle);

        }
    }else{
        MapView::getMapView()->generateMap(); //default map generated
    }

}

void LoadSaveService::saveGame(QFile* savefile)
{
    //Already open qfile

    //We have to call the right getter to save corrects datas

    QJsonDocument jd;
    QJsonObject jo;
    QJsonObject generation;
    QJsonObject mapsize;
    QJsonObject igconfig;
    QJsonObject camera;
    QJsonObject position;
    QJsonObject gamedata;
    QJsonObject savedata;

    mapsize.insert(QString("w"), MapView::getMapView()->getNbTiles());
    mapsize.insert(QString("h"), MapView::getMapView()->getNbTiles());
    generation.insert(QString("mapsize"), mapsize);
    generation.insert(QString("mapseed"), RandomService::getSeed());

    gamedata.insert(QString("cityname"), GameManagementService::getGameManagementService()->getCityName());
    gamedata.insert(QString("money"), GameManagementService::getGameManagementService()->getMoney());
    gamedata.insert(QString("taxes"), GameManagementService::getGameManagementService()->getTaxes());
    gamedata.insert(QString("igtime"), TickService::getTickService()->getGameTime()->toString(Qt::ISODate));
    gamedata.insert(QString("lastuid"), Building::getUIDCpt());
    gamedata.insert(QString("uid"), BuildingManagementService::getBuildingManagementService()->getJsonBuildings());

    camera.insert(QString("zoom"), MapView::getMapView()->getZoomFactor());
    position.insert(QString("x"),  MapView::getMapView()->getCameraX());
    position.insert(QString("y"),  MapView::getMapView()->getCameraY());
    camera.insert(QString("position"), position);
    igconfig.insert(QString("camera"), camera);
    igconfig.insert(QString("timespeed"), 0); //not implemented

    savedata.insert(QString("firstsave"), TickService::getTickService()->getStartTime()->toString(Qt::ISODate));
    savedata.insert(QString("lastsave"), QDateTime::currentDateTime().toString(Qt::ISODate));
    savedata.insert(QString("countsave"), GameManagementService::getGameManagementService()->incrementAndGetSaveCount());

    jo.insert(QString("generation"), generation);
    jo.insert(QString("igconfig"), igconfig);
    jo.insert(QString("gamedata"), gamedata);
    jo.insert(QString("savedata"), savedata);

    jd.setObject(jo);
    QTextStream out(savefile);
    out << jd.toJson();

    //Already closed qfile
}

void LoadSaveService::saveOptions()
{
    //If needed one day
    /*
    QJsonDocument jd;
    QJsonObject settings;
        QJsonObject audio;

    audio.insert(QString("master"), 100);
    audio.insert(QString("music"), 50);
    audio.insert(QString("sfx"), 25);

    settings.insert(QString("audio"), audio);
    */
}

void LoadSaveService::loadOptions()
{
    //If needed one day
}
