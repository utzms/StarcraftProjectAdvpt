#include <exception>
#include <stdexcept>
#include "TechnologyManager.h"

template <class Race> inline bool TechnologyManager<Race>::checkRequirement(std::shared_ptr<Technology> tech)
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

template <class Race> inline std::shared_ptr<Technology> TechnologyManager<Race>::findTechnology(std::shared_ptr<Unit> unit) 
{
    return techList.findUnit(unit->getName());
}

template <class Race> inline std::shared_ptr<Technology> TechnologyManager<Race>::findTechnology(std::shared_ptr<Building> building) 
{
    return techList.findUnit(building->getName());
}

template <class Race> inline std::shared_ptr<Technology> TechnologyManager<Race>::findTechnology(std::shared_ptr<Worker> worker) 
{
    return techList.findUnit(worker->getName());
}


template <class Race> TechnologyManager<Race>::TechnologyManager(std::shared_ptr<GameState> initialGameState)
{
    if(initialGameState == nullptr) 
    {
        throw std::invalid_argument("Can not pass nullptr as initial GameState");
    }

    gameState = initialGameState;
    if(!InitTechTree<Race>().initTechTree(techList)) 
    {
        throw std::exception("TechnologyList initialization failed. Something went terribly wrong!");
    }
}

 template <class Race> template <class EntityType> bool TechnologyManager<Race>::request(std::shared_ptr<EntityType> entity) 
{   

    std::shared_ptr<Technology> tech = nullptr;
    tech = TechnologyManager<Race>::findTechnology(entity); 
    if(tech == nullptr) 
    {
        throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
    }

    do 
    {
        if(TechnologyManager<Race>::checkRequirements(tech)) return true;
        tech = TechnologyManager<Race>::findTechnology(entity);
    } 
    while(tech != nullptr);
    
    return false;
}

 template <class Race> template <class EntityType> void TechnologyManager<Race>::notifyCreation(std::shared_ptr<EntityType> entity) 
{
    std::shared_ptr<Technology> technology = nullptr;
    technology = TechnologyManager<Race>::findTechnology(entity);
    if(technology == nullptr) 
    {
        throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
    }
    technology->setExistence(true);

}


template <class Race> template <class EntityType> void TechnologyManager<Race>::notifyDestruction(std::shared_ptr<EntityType> entity) 
{
    std::shared_ptr<Technology> technology = nullptr;
    technology = TechnologyManager<Race>::findTechnology(entity);
    if(technology == nullptr) 
    {
        throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
    }
    technology->setExistence(false);
}
