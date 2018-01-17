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

#include <QColor>

struct Cell
{
    int id;
    QColor color;
};


class RandomService
{
private:
    static int seed;
public:
    static Cell* generateMap(int sizex, int sizey);
    static Cell* generateSky(int sizex, int sizey);
    static int getSeed() {return seed;}
    static int setSeed(int newSeed) {RandomService::seed = newSeed;}
};
