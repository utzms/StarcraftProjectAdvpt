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
template
    <
        class TechTree
    >
class Simulation
{
	private:
        std::shared_ptr<GameState>       _gameState; /**< State of the whole game, containing all Entities and parameters */
        std::shared_ptr<ResourceManager> _resourceManager; /**< Object that is responsible for updating the all resources at the end of each timestep */
        std::shared_ptr<TechnologyManager<TechTree> > _technologyManager; /**< Object that is responsible for updating the all technologies at the end of each timestep */

        /**< Calling this function proceeds to the next timestep */

        BuildList* _buildList;

	public:
        /** Function starting the Simulation.
         *
         * After calling this function all required objects, including the whole GameState, the TechnologyManager and ResourceManager, are created.
         * Next the Simulation is started by entering the first timestep and proceded 
         * by executing all necessary actions in an arbitrary amount of timesteps until the GameState fits to the specifed goal.
         */
        Simulation(std::string buildListFilename,
                                               std::shared_ptr<GameState> gameState,
                                               std::shared_ptr<ResourceManager> resourceManager,
                                               std::shared_ptr<TechnologyManager<TechTree> > technologyManager)
            :_buildList(new BuildList(buildListFilename))
            ,_gameState(gameState)
            ,_resourceManager(resourceManager)
            ,_technologyManager(technologyManager)
        {

        }

        void run()
        {

        }

        void startSimulation()
        {
            do
            {
                std::cout <<  _buildList->current() << std::endl;
            }
            while( _buildList->advance() != BuildList::State::Finished );
            return;
        }

        void timeStep()
        {
            std::string currentEntity = _buildList->current();
        }

        ~Simulation()
        {
            //delete gameState;
            //delete resourceManager;
        }

};

#endif
