#pragma once

#include "QColor"

struct Cell
{
    int id;
    QColor color;
};


class RandomService
{
public:
    static Cell* generateMap(int seed, int sizex, int sizey);
};
