#include "ConstantBuilding.h"
#include "GameManagementService.h"
#include "BuildingManagementService.h"
#include "GuiView.h"

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
    if(isBuildingAddable(id))
    {
        //qDebug() << "Adding building " << id << "(" << x << ";" << y << ")";

        GameManagementService::getGameManagementService()->setMoney(GameManagementService::getGameManagementService()->getMoney()-(int)ConstantBuilding::get(id).getPrice());
        GuiView::getGuiView()->showBuildingPickerMenu(ConstantBuilding::get(id).getCategory()-1); // to update building that we can afford (if they are too expensive their names become red)
        vectorBuildings->append(new Building(id, x, y, angle,ConstantBuilding::getPopulationFromHouseType(id))); //if a house (category==1), starts with 1 population

        bSumPricePerSeconds = true;
        bSumPopulation = true;
        bAverageHappiness = true;
    }
}

void BuildingManagementService::removeBuilding(int uid)
{
    //qDebug() << "Removing building " << uid;
    int idInVector=-1;
    for(int i = 0; i < vectorBuildings->size(); i++)
    {
        int iuid = vectorBuildings->at(i)->getUid();
        if(iuid == uid){
            idInVector=i;




        }
    }

    if(idInVector!=-1){
        //75% refund of building price
    GameManagementService::getGameManagementService()->setMoney( GameManagementService::getGameManagementService()->getMoney()+ (int)ConstantBuilding::get(vectorBuildings->at(idInVector)->getId()).getPrice()*0.75);
    GuiView::getGuiView()->showBuildingPickerMenu(GuiView::getGuiView()->getCurrentTabId()); // to update building that we can afford (if they are too expensive their names become red)


    vectorBuildings->removeAt(idInVector);

    bSumPricePerSeconds = true;
    bSumPopulation = true;
    bAverageHappiness = true;
    }
}

bool BuildingManagementService::isBuildingAddable(int id) //TODO : we have to test it, it should be ok and it's possible to optimise it but this is fairly good
{
    //The constantbuilding construtor array isnt set to handle the requirements
    int sumRequirements = ConstantBuilding::get(id).getSumRequirements();

    if(sumRequirements == 0)
        return true;

    bool requirementsType = ConstantBuilding::get(id).getRequirementsType();
    QSet<int>* setRequirements = ConstantBuilding::get(id).getRequirements();
    //Copy of the list for the boolean AND
    QSet<int>* setRequirementsFlags = new QSet<int>(); //I dont really like doing this but there is no copy constructor, so before every return i have to delete the new set

    foreach (const int &i, *setRequirements) //Fill it with the copy of setRequirements
    {
        setRequirementsFlags->insert(i);
    }

    foreach (const int &i, *setRequirements)
    {
        for(int j = 0; j < vectorBuildings->size(); j++)
        {
            if(i == vectorBuildings->at(j)->getId())
            {
                sumRequirements -= ConstantBuilding::get(id).getRequirementsWeight();

                if(!requirementsType)
                {
                    //AND
                    setRequirementsFlags->remove(i);
                }
            }
        }

        if(sumRequirements <= 0)
        {
            if(!requirementsType)
            {
                //AND
                if(setRequirementsFlags->empty())
                {
                    delete setRequirementsFlags;
                    return true;
                }
            }
            else
            {
                //OR
                delete setRequirementsFlags;
                return true;
            }
        }
    }
    delete setRequirementsFlags;
    return false;
}

double BuildingManagementService::getSumPricePerSeconds()
{
    if(bSumPricePerSeconds)
    {
        pricePerSeconds = 0.0;
        for(int i = 0; i < vectorBuildings->size(); i++)
            pricePerSeconds += ConstantBuilding::get(vectorBuildings->at(i)->getId()).getPricePerSeconds();
        bSumPricePerSeconds = !bSumPricePerSeconds;
    }
    return pricePerSeconds;
}

int BuildingManagementService::getSumPopulation()
{
    if(bSumPopulation)
    {
        sumPopulation = 0;
        for(int i = 0; i < vectorBuildings->size(); i++)
            sumPopulation += vectorBuildings->at(i)->getPopulation();
        bSumPopulation = !bSumPopulation;
    }
    return sumPopulation;
}

double BuildingManagementService::getAverageHappiness()
{
    if(bAverageHappiness)
    {
        averageHappiness = 0.0;
        for(int i = 0; i < vectorBuildings->size(); i++)
            averageHappiness += vectorBuildings->at(i)->getHappiness();

        if(vectorBuildings->size() != 0)
            averageHappiness /= vectorBuildings->size();
        bAverageHappiness = !bAverageHappiness;
    }
    return averageHappiness;
}
