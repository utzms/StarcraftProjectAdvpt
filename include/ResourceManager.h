#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include "GameState.h"
#include "Entity.h"

/** Manager class responsible for updating the resource count at the end of each timestep.
 *
 * The ResourceManager updates the mineral, gas, energy and supply counts according to GameState after each timestep
 * by calling the timeStep() function which itself calls update().
 * To be safe that the update is only done after all changes to the GameState took place, 
 * it is ensured that all Entities are synchronized prior to this.
 */

class ResourceManager
{
	private:
		std::shared_ptr<GameState>	_gameState;
		double 				_vespinGasIncrementPerWorker; 
		double 				_mineralsIncrementPerWorker;
		void 				updateGameState(double vespinGasToAdd, double mineralsToAdd);
	public:
        /** Function that updates the GameStates resource count before preceding to the next timestep*/
		ResourceManager(double initialVespinGasIncrement, double initialMineralsIncrement);
		void timeStep();

};

#endif
