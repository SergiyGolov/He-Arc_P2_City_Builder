#include "LoadSaveService.h"
#include "services.h"

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

    loadGame(loadfile);

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

    QJsonDocument jd = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject jo = jd.object();
        QJsonObject generation = jo.value(QString("generation")).toObject();
            QJsonObject levelsize = generation.value(QString("levelsize")).toObject();
        QJsonObject igconfig = jo.value(QString("igconfig")).toObject();
            QJsonObject camera = igconfig.value(QString("camera")).toObject();
                QJsonObject position = camera.value(QString("position")).toObject();
        QJsonObject gamedata = jo.value(QString("gamedata")).toObject();
        QJsonObject savedata = jo.value(QString("savedata")).toObject();

    qDebug() << levelsize.value(QString("h")).toInt();

    //We have to call the right setter restore corrects datas
}

void LoadSaveService::saveGame(QFile* savefile)
{
    //Already open qfile

    //We have to call the right getter to save corrects datas

    QJsonDocument jd;
    QJsonObject jo;
        QJsonObject generation;
            QJsonObject levelsize;
        QJsonObject igconfig;
            QJsonObject camera;
                QJsonObject position;
        QJsonObject gamedata;
        QJsonObject savedata;

            levelsize.insert(QString("w"), 0); //not implemented
            levelsize.insert(QString("h"), 0); //not implemented
        generation.insert(QString("levelsize"), levelsize); //not implemented
        generation.insert(QString("mapseed"), RandomService::getSeed());

        gamedata.insert(QString("cityname"), GameManagementService::getGameManagementService()->getCityName());
        gamedata.insert(QString("money"), GameManagementService::getGameManagementService()->getMoney());
        gamedata.insert(QString("taxes"), GameManagementService::getGameManagementService()->getTaxes());
        gamedata.insert(QString("igtime"), TickService::getTickService()->getGameTime()->toString(Qt::ISODate));
        gamedata.insert(QString("lastuid"), Building::getUIDCpt());
        gamedata.insert(QString("uid"), BuildingManagementService::getBuildingManagementService()->getJsonBuildings());

            camera.insert(QString("zoom"), 0); //not implemented
                position.insert(QString("x"), 0); //not implemented
                position.insert(QString("y"), 0); //not implemented
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
