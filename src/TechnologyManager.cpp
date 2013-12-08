#include <stdexcept>
#include "TechnologyManager.h"

inline bool template <class Race> TechnologyManager::checkRequirement(Technology& requirement)
{
    if(requirement.exists() && requirement.getMineralsCost() <= (*gameState).getMinerals() && requirement.getGasCost() <= (*gameState).getGas() && requirement.getSupplyCost() <= (*gameState).getSupply()) 
    {
        return true;
    }
    return false;
}


template <class Race> TechnologyManager::TechnologyManager(std::shared_ptr<GameState> initialGameState)
{
    if(initialGameState == nullptr) {
        throw invalid_argument("Can not pass nullptr as initial GameState");
    }

    gameState = initialGameState;
    if(!InitTechTree().initTechTree(techList)) {
        throw exception("TechnologyList initialization failed. Something went terribly wrong!");
    }
}

template <class T> bool template <class Race> TechnologyManager<Race>::request(std::shared_ptr<T> entity) {
    Technology& technology = *(techList.findUnit((*entity).getName()));
    std::vector<std::string> requirements = technology.getRequirements();
    for(std::string name : requirements )
    {
        if(!checkRequirement(*techList.findUnit(name))) return false;
    }

    return true; 
}

template <class T> void template <class Race> TechnologyManager::notifyCreation(std::shared_ptr<T> entity) 
{
    Technology&& technology = *(techList.findUnit((*entity).getName()));
    technology.setExistence(true);
}


template <class T> void template <class Race> TechnologyManager<Race>::notifyDestruction(std::shared_ptr<T> entity) 
{
    Technology&& technology = *(techList.findUnit((*entity).getName()));
    technology.setExistence(false);
}
