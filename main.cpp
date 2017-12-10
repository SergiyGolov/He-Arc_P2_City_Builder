#include <QDebug>
#include "ConstantBuilding.h"

int main(int argc, char *argv[])
{
    qDebug() << ConstantBuilding::get(bID::Road).getCategory(); //This is the syntaxe to use the class ConstantBuilding

    return 0;
}
