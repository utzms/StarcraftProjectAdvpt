#include "../include/Simulation.h"
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

void Simulation::buildBuilding(std::shared_ptr<Worker> workerForBuilding, std::string name ,int time)
{

	std::shared_ptr<Building> buildingToBuild = std::shared_ptr<Building>(new Building(name, time));
	_gameState->buildingList.push_back(buildingToBuild);
	workerForBuilding->timer = time;
	workerForBuilding->buildingName = name;
	workerForBuilding->state = Worker::State::Constructing;

}

void Simulation::produceUnit(std::string name, int time,Building::ProductionType type)
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

void Simulation::timeStep()
{
	PROGRESS("STEP1");
            std::vector< std::shared_ptr<Worker> >&      workerList   = _gameState->workerList;
	PROGRESS("STEP2");
            std::vector< std::shared_ptr<Building> >&    buildingList = _gameState->buildingList;
	PROGRESS("STEP3");
            std::vector< std::shared_ptr<Unit> >&        unitList     = _gameState->unitList;
	PROGRESS("STEP4");

            for (auto workerIterator : workerList)
            {
                workerIterator->timeStep();
            }
	PROGRESS("STEP5");

            for (auto buildingIterator : buildingList)
            {
                buildingIterator->timeStep();
            }
	PROGRESS("STEP6");
            _resourceManager->timeStep();
	PROGRESS("STEP7");
            _gameStateUpdate->timeStep();
	PROGRESS("STEP8");
}

        /** Function starting the Simulation.
         *
         * After calling this function all required objects, including the whole GameState, the TechnologyManager and ResourceManager, are created.
         * Next the Simulation is started by entering the first timestep and proceded 
         * by executing all necessary actions in an arbitrary amount of timesteps until the GameState fits to the specifed goal.
         */
Simulation::Simulation(std::string buildListFilename,
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

void Simulation::run()
{

            ////add here custom SIMULATION////
            int timesteps = 0;
            //just some time limit
            while(timesteps < 200 )
            {
                //check for ready workers
				PROGRESS("RUN1");
                std::vector< std::shared_ptr<Worker> >&      workerList   = _gameState->workerList;
				PROGRESS("RUN2");
                for(auto workerIterator : workerList)
                {
                    if(workerIterator->state == Worker::State::Ready)
                    {
                        //send ready workers to the mineralfields
                           workerIterator->state = Worker::State::CollectingMinerals;
                    }
                }
				PROGRESS("RUN3");

                // test production of one worker
				PROGRESS("RUN4");
                if( !(_gameState->buildingList.empty()) != 0 && timesteps == 120)
                {
                    produceUnit(std::string("Worker"), 10, Building::ProductionType::WorkerOrder );
                }
				PROGRESS("RUN5");


                timeStep();
				PROGRESS("RUN6");
                timesteps++;
            }
            std::cout << "Produced Minerals: " << _gameState->getMinerals() << std::endl;
}

void Simulation::startSimulation()
{
            do
            {
				std::cout <<  _buildList.current() << std::endl;
            }
			while( _buildList.advance() != BuildList::State::Finished );
            return;
}

Simulation::~Simulation()
{
            //delete gameState;
            //delete resourceManager;
}

