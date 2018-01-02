#include "OursMaths.h"

OursMaths::OursMaths()
{

}

double OursMaths::roundExcel(double val, int dec)
{
    return round(val*qPow(10,dec))/qPow(10,dec);
}


double OursMaths::map(double value, double start1, double stop1, double start2, double stop2)
{
    double gap1 = stop1 - start1;
    double gap2 = stop2 - start2;

    double ratio = gap2 / gap1;

    return value * ratio;
}
