#include <exception>
#include <stdexcept>
#include "../include/TechnologyManager.h"


template <class TechTree> inline bool TechnologyManager<TechTree>::check(std::shared_ptr<Technology> tech)
{
    if(tech->getMineralsCost() > _gameState->getMinerals() || tech->getGasCost() > _gameState->getGas() || tech->getSupplyCost() > _gameState->getSupply()) return false;
    
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


//template<class TechTree> inline bool TechnologyManager<TechTree>::getNeededRequirements(std::shared_ptr<Technology> tech, Requirements& neededReqs)
//{
//    bool retVal = true;
//    if(tech->getMineralsCost() > _gameState->getMinerals() || tech->getGasCost() > _gameState->getGas() || tech->getSupplyCost() > _gameState->getSupply())
//    {
//        retVal = false;
//    }
    
//    // TODO
//    return retVal;

//}
/* TODO adapt these helper functions or replace them in order to be able to get the entitys name as argument and still be able to determine according to the template parameter if the entity is a Building, Unit or Worker
 */
template <class TechTree> inline std::vector<std::shared_ptr<Technology> > TechnologyManager<TechTree>::findTechnology(std::shared_ptr<Unit> unit)
{
    return _techList->findUnitVec(unit->getName());
}

template <class TechTree> inline std::vector<std::shared_ptr<Technology> > TechnologyManager<TechTree>::findTechnology(std::shared_ptr<Building> building)
{
    return _techList->findBuildingVec(building->getName());
}

template <class TechTree> inline std::vector<std::shared_ptr<Technology> > TechnologyManager<TechTree>::findTechnology(std::shared_ptr<Worker> worker)
{
    return _techList->findUnitVec(worker->getName());
}

template <class TechTree> TechnologyManager<TechTree>::TechnologyManager(std::shared_ptr<GameState> initialGameState, std::shared_ptr<TechnologyList> techList)
    :_gameState(initialGameState)
    ,_techList(techList)
{
    if(!_gameState || !_techList)
    {
        throw std::invalid_argument("Can not pass nullptr as initial argument");
    }

    if(!InitTechTree<TechTree>(_techList).initTechTree())
    {
        throw std::exception("TechnologyList initialization failed. Something went terribly wrong!");
    }
}

template <class TechTree> template <class EntityType> bool TechnologyManager<TechTree>::checkRequirements(std::shared_ptr<EntityType> entity)
{ 
    // TODO change argument to std::string and adapt the implementation

    std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager<TechTree>::findTechnology(entity);
    if(techVec.size() == 0) 
    {
        throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
    }
    for(auto tech : techVec) 
    {
        if(TechnologyManager<TechTree>::check(tech))
        {
            return true;
        }
    }
    return false;
}

template <class TechTree> template <class EntityType> std::shared_ptr<RequirementsVec> TechnologyManager<TechTree>::requestRequirements(std::shared_ptr<EntityType> entity)
{
    // TODO implementation, and adaption to std::string as argument
    std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager<TechTree>::findTechnology(entity);
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
template <class TechTree> template <class EntityType> void TechnologyManager<TechTree>::notifyCreation(std::shared_ptr<EntityType> entity)
{   
    
    std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager<TechTree>::findTechnology(entity);
    if(techVec.size() == 0)
    {
        throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
    }
    for(auto tech : techVec) 
    {
        tech->setExistence(true);   
    }

}


template <class TechTree> template <class EntityType> void TechnologyManager<TechTree>::notifyDestruction(std::shared_ptr<EntityType> entity)
{
    std::vector<std::shared_ptr<Technology>> techVec = TechnologyManager<TechTree>::findTechnology(entity);
    if(techVec.size() == 0)
    {
        throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
    }
    for(auto tech : techVec) 
    {
        tech->setExistence(false); 
    }

}
