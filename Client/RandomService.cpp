#include "RandomService.h"
#include "OursMaths.h"
#include "PerlinNoise.h"

int RandomService::seed = 0;

Cell* RandomService::generateMap(int sizex, int sizey)
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
            //double n = p->noise(xoff, yoff, 0); //TODO: use octave
            double n = p->OctavePerlinMap(xoff,yoff,0,4,0.35);
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

 Cell* RandomService::generateSky(int sizex, int sizey){
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
            //double n = p->noise(xoff, yoff, 0); //TODO: use octave
            double n = p->OctavePerlinSky(xoff,yoff,0,4,0.5);
            int index = x + y * sizex;

            if (!(n > 0.70 || n < 0.30))
            {
                //Grass
                cells[index].id = 0;
                cells[index].color.setRed(255);
                cells[index].color.setGreen(255);
                cells[index].color.setBlue(255);
            }
            else
            {
                //Water
                if (n > 0.70)
                {
                    n = 1 - n;
                }
                cells[index].id = 1;

//                cells[index].color.setRed(OursMaths::map(n, 0, 0.25, 0, 15));
//                cells[index].color.setGreen(OursMaths::map(n, 0, 0.30, 0, 125));
//                cells[index].color.setBlue(OursMaths::map(n, 0, 0.35, 100, 255));
                    cells[index].color.setHslF(0.56,0.92,0.755);
            }

            xoff += dx;
        }
        yoff += dy;
    }

    return cells;
}
