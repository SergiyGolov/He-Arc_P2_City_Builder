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

struct Params
{
    int size;
    int difficulty;
    int seed;
    QString name;
    Params()
    {
        size = 100;
        difficulty = 1;
        srand(time(NULL));
        seed = rand() % 1000;
        name = "No name specified";
    }
};

class LoadSaveService : public QObject
{
public:
public slots:
    static void loadGameUI();
    static void saveGameUI();

    static void loadGame(int argc, char *argv[]);
    static void loadGame(QString filename);

private:
    static void saveGame(QFile* filename);
    static void saveOptions();
    static void loadOptions();
};
