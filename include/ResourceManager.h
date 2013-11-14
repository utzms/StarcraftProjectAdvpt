#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include "GameState.h"
#include "Entity.h"

/** Manager class responsible for updating the GameState at the end of each timestep.
 *
 * The ResourceManager updates the Resource counters according to GameState after each timestep
 * by calling the timeStep() which itself calls update().
 * To be safe that the update is only done after all changes to the GameState took place, 
 * it is ensured that all Entities are synchronized prior to this.
 */

class ResourceManager
{
	private:
		GameState * gameState;
		void update();
	public:
        /** Function that updates the GameStates Resource count before preceding to the next timestep*/
		void timeStep();

};

#endif
