#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <vector>

#include "BuildList.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "TechnologyManager.h"
#include "Entity.h"

/** Main class designed for managing and controlling the Simulation.
 * 
 * During each timestep the Simulation decided which actions to execute and 
 * requests the TechnologyManager if the requirements for this action are fulfilled. 
 * If this applies, the actions are executed by changing the GameState object and the Entities that are part of it.
 * If not the Simulation decides itself what to do else.
 * After the Simulation is done with all activities for the current timestep, it procedes to the next one by calling timeStep().
 * The Simulation itself is started and executed by calling the startSimulation() function,
 * which creates all required objects before entering the first timestep.
 *
 */

class Simulation
{
	private:
        GameState*         gameState; /**< State of the whole game, containing all Entities and parameters */
        ResourceManager*   resourceManager; /**< Object that is responsible for updating the all resources at the end of each timestep */

        void timeStep(); /**< Calling this function proceeds to the next timestep */

        BuildList* _buildList;

	public:
        /** Function starting the Simulation.
         *
         * After calling this function all required objects, including the whole GameState, the TechnologyManager and ResourceManager, are created.
         * Next the Simulation is started by entering the first timestep and proceded 
         * by executing all necessary actions in an arbitrary amount of timesteps until the GameState fits to the specifed goal.
         */
		void startSimulation(); 
	Simulation(std::string buildListFilename);
        ~Simulation()
        {
            delete gameState;
            delete resourceManager;
        }

};

#endif
