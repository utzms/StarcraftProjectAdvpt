#include "TechnologyManager.h"


TechnologyManager::TechnologyManager(std::string race, std::shared_ptr<GameState> initialGameState)
{
    gameState = initialGameState;
    techList.initUnitList(race);
}

template <class T> bool TechnologyManager::request(std::shared_ptr<T> entity) {

    return false; 
}

template <class T> void TechnologyManager::notifyCreation(std::shared_ptr<T> entity) {


}


template <class T> void TechnologyManager::notifyDestruction(std::shared_ptr<T> entity) {



}
