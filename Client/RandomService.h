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
