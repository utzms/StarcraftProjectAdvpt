#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <vector>

#include "BuildList.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "TechnologyManager.h"
#include "StartingConfiguration.h"
#include "Entity.h"
#include "GameStateUpdate.h"
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
		BuildList& _buildList;

        std::shared_ptr<GameState>       _gameState; /**< State of the whole game, containing all Entities and parameters */
		std::shared_ptr<ResourceManager> _resourceManager; /**< Object that is responsible for updating the all resources at the end of each timestep */
		std::shared_ptr<TechnologyManager> _technologyManager; /**< Object that is responsible for updating the all technologies at the end of each timestep */
		std::shared_ptr<StartingConfiguration> _startingConfiguration;
        std::shared_ptr<GameStateUpdate> _gameStateUpdate;
        /**< Calling this function proceeds to the next timestep */

        void buildBuilding(std::shared_ptr<Worker> workerForBuilding, std::string name ,int time)
        {

            std::shared_ptr<Building> buildingToBuild = std::shared_ptr<Building>(new Building(name, time));
            _gameState->buildingList.push_back(buildingToBuild);
            workerForBuilding->timer = time;
            workerForBuilding->buildingName = name;
            workerForBuilding->state = Worker::State::Constructing;

        }

        void produceUnit(std::string name, int time,Building::ProductionType type)
        {
            std::vector< std::shared_ptr<Building> >&    buildingList = _gameState->buildingList;
            std::shared_ptr<Building> BuildingForProduction = NULL;
             for (auto buildingIterator : buildingList)
             {
                 if(buildingIterator->state == Building::State::Ready)
                 {
                        BuildingForProduction = buildingIterator;
                        break;
                 }
             }

             if(BuildingForProduction == NULL)
             {
                 throw std::runtime_error("GameStateUpdate::produceUnit: Wrong Order placed");
             }

            BuildingForProduction->productionType = type;
            BuildingForProduction->productionUnitName = name;
            BuildingForProduction->timer = time;
            BuildingForProduction->state = Building::State::Producing;
        }

        void timeStep()
        {
            std::vector< std::shared_ptr<Worker> >&      workerList   = _gameState->workerList;
            std::vector< std::shared_ptr<Building> >&    buildingList = _gameState->buildingList;
            std::vector< std::shared_ptr<Unit> >&        unitList     = _gameState->unitList;

            for (auto workerIterator : workerList)
            {
                workerIterator->timeStep();
            }

            for (auto buildingIterator : buildingList)
            {
                buildingIterator->timeStep();
            }
            _resourceManager->timeStep();
            _gameStateUpdate->timeStep();
        }

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
											   std::shared_ptr<TechnologyManager> technologyManager,
                                               std::shared_ptr<StartingConfiguration> startingConfiguration,
                                               std::shared_ptr<GameStateUpdate> gameStateUpdate
                                               )
			:_buildList(*(new BuildList(buildListFilename)))
            ,_gameState(gameState)
            ,_resourceManager(resourceManager)
            ,_technologyManager(technologyManager)
			,_startingConfiguration(startingConfiguration)
            ,_gameStateUpdate(gameStateUpdate)
		{

            std::vector< std::shared_ptr<Worker>>& workerList = gameState->workerList;
            for(int currentWorker = 0; currentWorker < _startingConfiguration->getInitialWorkerCount(); ++currentWorker)
            {
                workerList.push_back(std::shared_ptr<Worker>(new Worker("Konrad")));
                workerList[currentWorker]->state = Worker::State::CollectingMinerals;
            }
            buildBuilding(_gameState->workerList[0] , "Nexus", 1);
            gameState->addMinerals(_startingConfiguration->getInitialMinerals());
            gameState->addGas(_startingConfiguration->getInitialVespeneGas());
        }

        void run()
        {

            ////add here custom SIMULATION////
            int timesteps = 0;
            //just some time limit
            while(timesteps < 200 )
            {
                //check for ready workers
                std::vector< std::shared_ptr<Worker> >&      workerList   = _gameState->workerList;
                for(auto workerIterator : workerList)
                {
                    if(workerIterator->state == Worker::State::Ready)
                    {
                        //send ready workers to the mineralfields
                           workerIterator->state = Worker::State::CollectingMinerals;
                    }
                }

                // test production of one worker
                if( !(_gameState->buildingList.empty()) != 0 && timesteps == 120)
                {
                    produceUnit(std::string("Worker"), 10, Building::ProductionType::WorkerOrder );
                }


                timeStep();
                timesteps++;
            }
            std::cout << "Produced Minerals: " << _gameState->getMinerals() << std::endl;
        }

        void startSimulation()
        {
            do
            {
				std::cout <<  _buildList.current() << std::endl;
            }
			while( _buildList.advance() != BuildList::State::Finished );
            return;
        }

        ~Simulation()
        {
            //delete gameState;
            //delete resourceManager;
        }

};

#endif
