#include <exception>
#include <stdexcept>
#include "TechnologyManager.h"

template <class Race> inline bool TechnologyManager<Race>::checkRequirement(std::shared_ptr<Technology> requirement)
{
    return false;
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
    using std::vector;
    using std::shared_ptr;
    shared_ptr<Technology> tech = nullptr;
    if(tech == nullptr) {
        throw std::invalid_argument("The requested Entity is not existent in the Tech Tree");
    }

    vector<vector<shared_ptr<Technology>>> requirements = tech.getRequirements();

    for(vector<shared_ptr<Technology>> req : requirements )
    {
        if(!checkRequirement(req)) return false;
    }

    return true; 
}

 template <class Race> template <class EntityType> void TechnologyManager<Race>::notifyCreation(std::shared_ptr<EntityType> entity) 
{
    Technology&& technology = *(techList.findUnit((*entity).getName()));
    technology.setExistence(true);
}


template <class Race> template <class EntityType> void TechnologyManager<Race>::notifyDestruction(std::shared_ptr<EntityType> entity) 
{
    Technology&& technology = *(techList.findUnit((*entity).getName()));
    technology.setExistence(false);
}
