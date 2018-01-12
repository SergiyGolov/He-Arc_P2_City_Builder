#include "OursMaths.h"

OursMaths::OursMaths()
{

}

double OursMaths::roundExcel(double val, int dec)
{
    return round(val*qPow(10,dec))/qPow(10,dec);
}


double OursMaths::map(double n, double start1, double stop1, double start2, double stop2)
{
    return ((n-start1)/(stop1-start1))*(stop2-start2)+start2;
}
