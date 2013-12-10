#include "ResourceManager.h"

// Anmerkung:
// Idee für die Erweiterung des Resource Managers: Wie bei den Builings
// Race-Policies für das Update verwenden. Auf diese Weise wäre es möglich
// die berüchtige Larva Produktion der Zerg zu realisieren.

RecourceManager::RecourceManager( std::shared_ptr<GameState> initialGameState, double initialVespinGasIncrement, double initialMineralsIncrement )
{
	//initialize gamestate and increments
	_gameState 			= initialGameState;
	if(_gameState == NULL)
	{
	 	throw std::invalid_argument("RecourceManager::ResourceManager: initialGameState is a NULL-pointer");
		return;
	}
	_vespinGasIncrementPerWorer 	= initialVespinGasIncrement;
	_mineralsIncrementPerWorker 	= initialMineralsIncrement;
}


void RecourceManager::timestep()
{
	//get workerList of current gamestate
	std::shared_ptr<std::vector<std::shared_ptr<Worker>>> workerListPtr = _gamestate.workerList;
	if(workerListPtr == NULL)
	{
		throw std::invalid_argument("RecourceManager::timeStep: worker list is not initialized"):
		return;
	}
	//check if there are no worker
	int workerListLength = (gamestate.workerList).size();
	if(workerListLength == 0)
	{	
		return;
	}

	double vespinGasToAdd 	= 0.0;
	double mineralsToAdd 	= 0.0;

	//determine overall increase of resources depending on harvesting workers
	for(std::vector<Worker>::iterator currentWorker = workerListPtr->begin();
			currentWorker=!workerListPtr->end(); ++currentWorker )
	{
		if(currentWorker.state == currentWorker.State::CollectingMinerals )
		{
			mineralsToAdd += _mineralsIncrementPerWorker ;
		}
		if(currentWorker.state == currentWorker.State::CollectingMinerals )
		{
			mineralsToAdd += _vespinIncrementPerWorker ;
		}	
	}
	//update gamestate
	updateGameState(vespinGasToAdd, mineralsToAdd);
}

void RecourceManager::updateGameState(double vespinGasToAdd, double mineralsToAdd)
{
	_gamestate.vespinGas += vespinGasToAdd;
	_gamestate.minerals  += mineralsGasToAdd;
}

