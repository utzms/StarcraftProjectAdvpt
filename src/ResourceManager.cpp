#include "../include/ResourceManager.h"

// Anmerkung:
// Idee für die Erweiterung des Resource Managers: Wie bei den Builings
// Race-Policies für das Update verwenden. Auf diese Weise wäre es moeglich
// die beruechtige Larva Produktion der Zerg zu realisieren.

ResourceManager::ResourceManager(std::shared_ptr<GameState> initialGameState, float initialVespinGasIncrement, float initialMineralsIncrement)
{
    // initialize gamestate and increments
    _gameState = initialGameState;

    if(!_gameState)
	{
        throw std::invalid_argument("ResourceManager::ResourceManager: initialGameState points to null object");
	}

    _vespinGasIncrementPerWorker 	= initialVespinGasIncrement;
	_mineralsIncrementPerWorker 	= initialMineralsIncrement;
}

void ResourceManager::timeStep()
{
    // get workerList of current gamestate
    std::vector<std::shared_ptr<Worker> >& workerList = _gameState->workerList;

    // check if there are no workers
    // and return if thats true
    if(workerList.empty())
	{	
		return;
	}

    float vespinGasToAdd 	= 0.f;
    float mineralsToAdd 	= 0.f;

	//determine overall increase of resources depending on harvesting workers
    for(auto currentWorker = workerList.begin();
             currentWorker != workerList.end();
             ++currentWorker)
	{
        if((*currentWorker)->state == Worker::State::CollectingMinerals)
		{
            mineralsToAdd += _mineralsIncrementPerWorker;
		}

        if((*currentWorker)->state == Worker::State::CollectingMinerals)
		{
            mineralsToAdd += _vespinGasIncrementPerWorker;
		}	
	}
	//update gamestate
	updateGameState(vespinGasToAdd, mineralsToAdd);
}

void ResourceManager::updateGameState(float vespinGasToAdd, float mineralsToAdd)
{
    _gameState->addGas(vespinGasToAdd);
    _gameState->addMinerals(mineralsToAdd);
}

