#include "OursMaths.h"

OursMaths::OursMaths()
{

}

double OursMaths::roundExcel(double val, int dec)
{
    return round(val*qPow(10,dec))/qPow(10,dec);
}
