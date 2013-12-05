#include "TechnologyManager.h"


TechnologyManager::TechnologyManager(std::string race, std::shared_ptr<GameState> initialGameState)
{
    gameState = initialGameState;
    techList.initUnitList(race);
}

template <class T> bool TechnologyManager::request(const T& entity) {

    return false; 
}

template <class T> void TechnologyManager::notify(const T& entity) {


}
