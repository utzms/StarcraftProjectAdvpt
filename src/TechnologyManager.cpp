#include <string>
#include <memory>
#include "TechnologyManager.h"


TechnologyManager::TechnologyManager(std::string race, shared_ptr<GameState> initialGameState)
{
    gameState = initialGameState;
    techList.initUnitList(race);
}

TechnologyManager::template <class T> bool request(const T& entity) {


}

TechnologyManager::template <class T> void notify(const T& entity) {


}
