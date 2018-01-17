#include "Client/BuildingManagementService.h"

#include "Client/ConstantBuilding.h"
#include "Client/GameManagementService.h"
#include "Client/GuiView.h"
#include "Client/AudioService.h"

#include "general/OurMath.h"

#include <QJsonObject>

BuildingManagementService* BuildingManagementService::buildingManagementService = nullptr;

BuildingManagementService* BuildingManagementService::getBuildingManagementService()
{
    if(buildingManagementService == nullptr)
        buildingManagementService = new BuildingManagementService();
    return buildingManagementService;
}

BuildingManagementService::BuildingManagementService()
{
    bSumPricePerSeconds = true;
    bSumPopulation = true;
    bAverageHappiness = true;

    vectorBuildings = new QVector<Building*>();
}

//methods
void BuildingManagementService::addBuilding(int id, int x, int y, int angle)
{
    bool requirements = isBuildingAddable(id);
    if(requirements)
    {
        GameManagementService::getGameManagementService()->setMoney(GameManagementService::getGameManagementService()->getMoney()-(int)ConstantBuilding::get(id).getPrice());
        if(ConstantBuilding::get(id).getPrice()>0)GuiView::getGuiView()->showBuildingPickerMenu(ConstantBuilding::get(id).getCategory()-1); // to update building that we can afford (if they are too expensive their names become red)
        vectorBuildings->append(new Building(id, x, y, angle));

        bSumPricePerSeconds = true;
        bSumPopulation = true;
        bAverageHappiness = true;

        AudioService::getAudioService()->playSfx(sfxID::buildingAdded);
    }
}

void BuildingManagementService::addBuildingFromSave(int id, int x, int y, int angle)
{

        vectorBuildings->append(new Building(id, x, y, angle));

}

void BuildingManagementService::removeBuilding(int uid)
{
    int idInVector=getVectorId(uid,vectorBuildings);
    if(idInVector!=-1)
    {
        //75% refund of building price
        GameManagementService::getGameManagementService()->setMoney( GameManagementService::getGameManagementService()->getMoney()+ (int)ConstantBuilding::get(vectorBuildings->at(idInVector)->getId()).getPrice()*0.75);
        GuiView::getGuiView()->showBuildingPickerMenu(GuiView::getGuiView()->getCurrentTabId()); // to update building that we can afford (if they are too expensive their names become red)


        vectorBuildings->removeAt(idInVector);

        bSumPricePerSeconds = true;
        bSumPopulation = true;
        bAverageHappiness = true;

        AudioService::getAudioService()->playSfx(sfxID::buildingRemoved);
    }
}

bool BuildingManagementService::isBuildingAddable(int id) //TODO : we have to test it, it should be ok and it's possible to optimise it but this is fairly good
{
    //Check money
    int price = ConstantBuilding::get(id).getPrice();

    if(price > GameManagementService::getGameManagementService()->getMoney() && price <= 0)
        return false;

    //The constantbuilding construtor array isnt set to handle the requirements
    int sumRequirements = ConstantBuilding::get(id).getSumRequirements();
    if(sumRequirements <= 0)
        return true;

    bool requirementsType = ConstantBuilding::get(id).getRequirementsType();
    QSet<int>* setRequirements = ConstantBuilding::get(id).getRequirements();

    bool* tabSetRequirements = new bool[setRequirements->size()];
    for(int i = 0; i < setRequirements->size(); i++)
    {
        tabSetRequirements[i] = false;
    }

    int k = 0;
    foreach (const int &i, *setRequirements)
    {
        for(int j = 0; j < vectorBuildings->size(); j++)
        {
            if(i == vectorBuildings->at(j)->getId())
            {
                tabSetRequirements[k] = true;
                sumRequirements -= ConstantBuilding::get(vectorBuildings->at(j)->getId()).getRequirementsWeight();
                if(sumRequirements <= 0 && !requirementsType) //OR
                {
                    return true;
                }
            }
        }
        k++;
    }

    if(!requirementsType) //OR
    {
        return false;
    }
    else //AND
    {
        for(int i = 0; i < setRequirements->size(); i++)
        {
            if(tabSetRequirements[i] == false)
                return false;
        }
        return true;
    }
}

double BuildingManagementService::getSumPricePerSeconds(bool everything)
{
    if(bSumPricePerSeconds || everything)
    {
        pricePerSeconds = 0.0;
        for(int i = 0; i < vectorBuildings->size(); i++)
            pricePerSeconds += ConstantBuilding::get(vectorBuildings->at(i)->getId()).getPricePerSeconds();
        bSumPricePerSeconds = !bSumPricePerSeconds;
    }
    return pricePerSeconds;
}

int BuildingManagementService::getSumPopulation(bool everything)
{
    if(bSumPopulation || everything)
    {
        sumPopulation = 0;
        for(int i = 0; i < vectorBuildings->size(); i++)
            sumPopulation += vectorBuildings->at(i)->getPopulation();
        bSumPopulation = !bSumPopulation;
    }
    return sumPopulation;
}

QJsonObject BuildingManagementService::getJsonBuildings()
{
    QJsonObject obj;
    for(int i = 0; i < vectorBuildings->size(); i++)
    {
        QJsonObject obji;
        obji.insert(QString("id"), QJsonValue(vectorBuildings->at(i)->getId()));
        obji.insert(QString("x"), QJsonValue(vectorBuildings->at(i)->getX()));
        obji.insert(QString("y"), QJsonValue(vectorBuildings->at(i)->getY()));
        obji.insert(QString("population"), QJsonValue(vectorBuildings->at(i)->getPopulation()));
        obji.insert(QString("angle"), QJsonValue(vectorBuildings->at(i)->getAngle()));
        obj.insert(QString::number(vectorBuildings->at(i)->getUid()), obji);
    }
    return obj;
}

double BuildingManagementService::getHappiness(Building* b)
{
    double sumHappiness = 0;
    for(int i = 0; i < vectorBuildings->size(); i++)
    {
        Building* bi = vectorBuildings->at(i);
        if(b->getUid() != bi->getUid())
        {
            ConstantBuilding c = ConstantBuilding::get(bi->getId());
            int dx = b->getX() - bi->getX();
            int dy = b->getY() - bi->getY();
            int d = sqrt(dx*dx+dy*dy);
            if(d < c.getRadius())
            {
                double ratioProximity = OursMaths::map(d ,0, c.getRadius(), 1, 0);
                sumHappiness += ConstantBuilding::get(bi->getId()).getEfficiancy() * ratioProximity;
            }
        }
    }
    if(sumHappiness > 200)
        sumHappiness = 200;
    return sumHappiness;
}

double BuildingManagementService::getAverageHappiness(bool everything)
{
    if(bAverageHappiness || everything)
    {
        int houseCount = 0;
        double averageHappiness_loc = 0.0;
        for(int i = 0; i < vectorBuildings->size(); i++)
        {
            Building* bi = vectorBuildings->at(i);
            if(ConstantBuilding::isHouse(bi->getId()))
            {
                houseCount++;
                averageHappiness_loc += getHappiness(bi);
            }
        }
        if(houseCount != 0)
            averageHappiness_loc /= houseCount;
        averageHappiness = averageHappiness_loc;
        bAverageHappiness = !bAverageHappiness;
    }
    return averageHappiness;
}

int BuildingManagementService::getVectorId(int uid,QVector<Building*> *v)
{
    int idInVector=-1;
    for(int i = 0; i < v->size() && idInVector==-1; i++)
    {
        int iuid = v->at(i)->getUid();
        if(iuid == uid){
            idInVector=i;
        }
    }

    return idInVector;
}
