#include <exception>
#include <stdexcept>
#include "../include/TechnologyManager.h"


template <class Race> inline bool TechnologyManager<Race>::check(std::shared_ptr<Technology> tech)
{
    if(tech->getMineralsCost() > gameState->getMinerals() || tech->getGasCost() > gameState->getGas() || tech->getSupplyCost() > gameState->getSupply()) return false;
    
    std::vector<std::vector<std::shared_ptr<Technology>>> requirements = tech->getRequirements();
    bool fulfilled;
    for(auto redundantRequirements : requirements) 
    {   
        fulfilled = false;
        for(std::shared_ptr<Technology> requirement : redundantRequirements)
        {
            if(requirement->exists())
            {
                fulfilled = true;
                break;
            }
        }
        if(!fulfilled) return false;
    }
    return true;
}


//template<class Race> inline bool TechnologyManager<Race>::getNeededRequirements(std::shared_ptr<Technology> tech, Requirements& neededReqs)
//{
//    bool retVal = true;
//    if(tech->getMineralsCost() > gameState->getMinerals() || tech->getGasCost() > gameState->getGas() || tech->getSupplyCost() > gameState->getSupply())
//    {
//        retVal = false;
//    }
    
//    // TODO
//    return retVal;

//}
/* TODO adapt these helper functions or replace them in order to be able to get the entitys name as argument and still be able to determine according to the template parameter if the entity is a Building, Unit or Worker
 */
template <class Race> inline std::vector<std::shared_ptr<Technology> > TechnologyManager<Race>::findTechnology(std::shared_ptr<Unit> unit)
{
    return techList->findUnitVec(unit->getName());
}

template <class Race> inline std::vector<std::shared_ptr<Technology> > TechnologyManager<Race>::findTechnology(std::shared_ptr<Building> building)
{
    return techList->findBuildingVec(building->getName());
}

template <class Race> inline std::vector<std::shared_ptr<Technology> > TechnologyManager<Race>::findTechnology(std::shared_ptr<Worker> worker)
{
    return techList->findUnitVec(worker->getName());
}
   
template <class Race> TechnologyManager<Race>::TechnologyManager(std::shared_ptr<GameState> initialGameState, std::shared_ptr<TechnologyList> techListPtr)
{
    if(initialGameState == nullptr || techListPtr == nullptr) 
    {
        throw std::invalid_argument("Can not pass nullptr as initial argument");
    }

    gameState = initialGameState;
    techList = techListPtr;
    if(!InitTechTree<Race>(techList).initTechTree()) 
    {
        throw std::exception("TechnologyList initialization failed. Something went terribly wrong!");
    }
}

template <class Race> template <class EntityType> bool TechnologyManager<Race>::checkRequirements(std::shared_ptr<EntityType> entity) 
{ 
    // TODO change argument to std::string and adapt the implementation

    std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager<Race>::findTechnology(entity); 
    if(techVec.size() == 0) 
    {
        throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
    }
    for(auto tech : techVec) 
    {
        if(TechnologyManager<Race>::check(tech))
        {
            return true;
        }
    }
    return false;
}

template <class Race> template <class EntityType> std::shared_ptr<RequirementsVec> TechnologyManager<Race>::requestRequirements(std::shared_ptr<EntityType> entity) 
{
    // TODO implementation, and adaption to std::string as argument
    std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager<Race>::findTechnology(entity); 
    if(techVec.size() == 0) 
    {
        throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
    }
    auto res = std::shared_ptr<RequirementsVec>(new RequirementsVec(techVec.size()));
    for(size_t i = 0; i != techVec.size(); ++i) 
    {
        //TODO 
    }
    return res;

}
template <class Race> template <class EntityType> void TechnologyManager<Race>::notifyCreation(std::shared_ptr<EntityType> entity) 
{   
    
    std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager<Race>::findTechnology(entity);
    if(techVec.size() == 0)
    {
        throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
    }
    for(auto tech : techVec) 
    {
        tech->setExistence(true);   
    }

}


template <class Race> template <class EntityType> void TechnologyManager<Race>::notifyDestruction(std::shared_ptr<EntityType> entity) 
{
    std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager<Race>::findTechnology(entity);
    if(techVec.size() == 0)
    {
        throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
    }
    for(auto tech : techVec) 
    {
        tech->setExistence(false); 
    }

}
