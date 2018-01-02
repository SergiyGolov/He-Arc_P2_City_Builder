#ifndef OURSMATHS_H
#define OURSMATHS_H

#include <QtMath>
#include "math.h"

class OursMaths
{
public:
    OursMaths();
    static double roundExcel(double val, int dec);
    static double map(double value, double start1, double stop1, double start2, double stop2);
};

#endif // OURSMATHS_H
