#include "RandomService.h"
#include "OursMaths.h"
#include "PerlinNoise.h"

Cell* RandomService::generateMap(int seed, int sizex, int sizey)
{
    Cell* cells = new Cell[sizex * sizey];

    const double dx = 0.01;
    const double dy = 0.01;

    double xoff;
    double yoff = 0;

    PerlinNoise *p = new PerlinNoise(seed);

    for(int y = 0; y < sizey; y++)
    {
        xoff = 0;
        for(int x = 0; x < sizex; x++)
        {
            double n = p->noise(xoff, yoff, 0);
            int index = x + y * sizex;

            if (!(n > 0.70 || n < 0.30))
            {
                //Grass
                cells[index].id = 0;
                cells[index].color.setRed(OursMaths::map(n, 0.30, 0.70, 22, 39));
                cells[index].color.setGreen(OursMaths::map(n, 0.30, 0.70, 102, 178));
                cells[index].color.setBlue(OursMaths::map(n, 0.30, 0.70, 12, 21));
            }
            else
            {
                //Water
                if (n > 0.70)
                {
                    n = 1 - n;
                }
                cells[index].id = 1;
                cells[index].color.setRed(OursMaths::map(n, 0, 0.30, 0, 0));
                cells[index].color.setGreen(OursMaths::map(n, 0, 0.30, 0, 204));
                cells[index].color.setBlue(OursMaths::map(n, 0, 0.30, 102, 255));
            }

            xoff += dx;
        }
        yoff += dy;
    }

    return cells;
}
