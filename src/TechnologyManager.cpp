#include "TechnologyManager.h"

inline bool TechnologyManager::checkRequirement(Technology& requirement)
{
    if(requirement.exists() && requirement.getMineralsCost() <= (*gameState).getMinerals() && requirement.getGasCost() <= (*gameState).getGas() && requirement.getSupplyCost() <= (*gameState).getSupply()) 
    {
        return true;
    }
    return false;
}


TechnologyManager::TechnologyManager(std::string race, std::shared_ptr<GameState> initialGameState)
{
    gameState = initialGameState;
    techList.initUnitList(race);
}

template <class T> bool TechnologyManager::request(std::shared_ptr<T> entity) {
    Technology& technology = *(techList.findUnit((*entity).getName()));
    std::vector<std::string> requirements = technology.getRequirements();
    for(std::string name : requirements )
    {
        if(!checkRequirement(*techList.findUnit(name))) return false;
    }

    return true; 
}

template <class T> void TechnologyManager::notifyCreation(std::shared_ptr<T> entity) 
{
    Technology&& technology = *(techList.findUnit((*entity).getName()));
    technology.setExistence(true);
}


template <class T> void TechnologyManager::notifyDestruction(std::shared_ptr<T> entity) 
{
    Technology&& technology = *(techList.findUnit((*entity).getName()));
    technology.setExistence(false);
}
