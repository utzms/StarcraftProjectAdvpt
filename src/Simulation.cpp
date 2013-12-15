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

void Simulation::produceUnit(std::vector<std::string> buildingNames, std::string unitName, int time,Building::ProductionType type)
{
            std::vector< std::shared_ptr<Building> >& buildingList = _gameState->buildingList;
            std::shared_ptr<Building> BuildingForProduction(nullptr);

             for (auto buildingIterator : buildingList)
             {
				 // at the moment, we take the first building we can get our hands on
				 // maybe later we'll want to choose which one we take...
				 if(( std::find(buildingNames.begin(), buildingNames.end(), buildingIterator->getName()) != buildingNames.end()) && (buildingIterator->state == Building::State::Ready))
                 {
                        BuildingForProduction = buildingIterator;
                        break;
                 }
             }

             if(!BuildingForProduction)
             {
                 throw std::runtime_error("GameStateUpdate::produceUnit: Wrong Order placed");
             }

            BuildingForProduction->productionType = type;
            BuildingForProduction->productionUnitName = unitName;
            BuildingForProduction->timer = time;
            BuildingForProduction->state = Building::State::Producing;
}

void Simulation::timeStep()
{
            std::vector< std::shared_ptr<Worker> >&      workerList   = _gameState->workerList;
            std::vector< std::shared_ptr<Building> >&    buildingList = _gameState->buildingList;
        //    std::vector< std::shared_ptr<Unit> >&        unitList     = _gameState->unitList;

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

        /** Function starting the Simulation.
         *
         * After calling this function all required objects, including the whole GameState, the TechnologyManager and ResourceManager, are created.
         * Next the Simulation is started by entering the first timestep and proceded 
         * by executing all necessary actions in an arbitrary amount of timesteps until the GameState fits to the specifed goal.
         */
Simulation::Simulation(	std::string buildListFilename,
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
            std::vector< std::shared_ptr<Worker> >& workerList = gameState->workerList;

            for(int currentWorker = 0; currentWorker < _startingConfiguration->getInitialWorkerCount(); ++currentWorker)
            {
                workerList.push_back(std::shared_ptr<Worker>(new Worker("Probe")));
                workerList[currentWorker]->state = Worker::State::CollectingMinerals;
            }

			_gameState->buildingList.push_back(std::shared_ptr<Building>(new Building("Nexus", 0)));
			_gameState->buildingList.back()->state = Building::State::Ready;

			int supplyToAdd = _technologyManager->getEntityCosts("Nexus").supply - _startingConfiguration->getInitialWorkerCount();

            _gameState->addMinerals(_startingConfiguration->getInitialMinerals());
            _gameState->addGas(_startingConfiguration->getInitialVespeneGas());
			_gameState->addSupply(supplyToAdd);

			_technologyManager->notifyCreation("Nexus");
}

void Simulation::run()
{
			PROGRESS("Simulation::run() starts ");

			std::vector< std::shared_ptr<Worker> >& workerList = _gameState->workerList;
			bool vespeneHarvestingBuildingExists = false;

			BuildList::State buildListState = BuildList::State::InProgress;
			int time = 0;

			PROGRESS("Simulation::run() Starting main loop");
			while (!_buildList.allItemsOk()/*&& missing: check if no items are under construction and no building is producing anymore */)
			{
				_buildList.reset();
				buildListState = BuildList::State::InProgress;

				PROGRESS("Simulation::run() Time " << time);
				PROGRESS("Simulation::run() [Current resources] Minerals: " << _gameState->getMinerals() << " Gas: " << _gameState->getGas() << " Energy: " << _gameState->getEnergy() << " Supply: " << _gameState->getSupply());

				PROGRESS("Simulation::run() Resetting workers to collect resources");

				// temporary hack to check for existence of vespene harvesting building
				vespeneHarvestingBuildingExists =	_technologyManager->buildingExists("Assimilator");/* ||
													_technologyManager->buildingExists("Refinery") ||
													_technologyManager->buildingExists("Extractor");*/

				for(auto workerIterator : workerList)
				{
					if( workerIterator->state == Worker::State::Ready ||
						workerIterator->state == Worker::State::CollectingMinerals ||
						workerIterator->state == Worker::State::CollectingVespene)
					{
						//send ready workers to the mineralfields
						if (time%2==0 && vespeneHarvestingBuildingExists)
						{
							workerIterator->state = Worker::State::CollectingVespene;
						}
						else
						{
							workerIterator->state = Worker::State::CollectingMinerals;
						}
					}
				}

				while (buildListState != BuildList::State::Finished)
				{
					if (_buildList.allItemsOk())
					{
						break;
					}

					// go to the first element thats not ok
					BuildList::State innerListState = BuildList::State::InProgress;
					while (innerListState != BuildList::State::Finished)
					{
						if (!_buildList.isCurrentItemOk())
						{
							break;
						}

						innerListState = _buildList.advance();						
					}

					// it seems we tried every item on the list,
					// now its better to go to the next time step.
					// we won't be able to build anything in this one.
					if (innerListState == BuildList::State::Finished)
					{
						break;
					}
						
					std::string currentItem = _buildList.current();

					// check the requirements
					bool techRequirements = _technologyManager->checkEntityRequirements(currentItem);
				
					// if we do not meet the requirements ...
					if (!techRequirements)
					{
						PROGRESS("Simulation::run() Requirements of " << currentItem << " not met, advancing to next item.");						
					}
					else
					{
						PROGRESS("Simulation::run() Requirements of " << currentItem << " OK");

						// get the costs for game state manipulation and build time
						Costs entityCosts = _technologyManager->getEntityCosts(currentItem);

						// reduce minerals and gas by costs
						_gameState->subMinerals(entityCosts.minerals);
						_gameState->subGas(entityCosts.gas);

						// otherwise, we find out if the item is a unit or a building
						if (_technologyManager->checkIfNameIsBuilding(currentItem))
						{
							// it is a building, so we need a worker
							std::shared_ptr<Worker> ourWorker(nullptr);

							// first check for ready workers, so we only
							// use workers who are collecting if necessary
							for (auto workerIterator : workerList)
							{
								if (workerIterator->state == Worker::State::Ready)
								{
									ourWorker = workerIterator;
									break;
								}
							}
							// if we haven't found a ready worker, we 
							// have to take a minerals or vespene guy
							if (!ourWorker)
							{
								for (auto workerIterator : workerList)
								{
									if (workerIterator->state == Worker::State::CollectingMinerals || workerIterator->state == Worker::State::CollectingVespene)
									{
										ourWorker = workerIterator;
										break;
									}
								}
							}

							// we have a worker, let's build something!
							if (ourWorker)
							{
								PROGRESS("Simulation::run() Ordering building " << currentItem);
								buildBuilding(ourWorker, currentItem, entityCosts.buildTime);
								_buildList.setCurrentItemOk();
								std::cout << currentItem << " (" << time/60 << ":" << time%60 << ")" << std::endl;
							}

							// if we still haven't got a worker
							// we need to try again at a later point							
						}
						else if (_technologyManager->checkIfNameIsUnit(currentItem))
						{
							// it is a unit, so we need to decide if it is a 
							// worker or another unit
							PROGRESS("Simulation::run() Ordering unit " << currentItem);

							// get the buildings this unit can be built from
							std::vector<std::string> buildings = _technologyManager->getBuildingsForUnitProduction(currentItem);

							// check if any of them are ready
							bool buildingReady = false;
							for (auto buildingIterator : _gameState->buildingList)
							{							
								if(( std::find(buildings.begin(), buildings.end(), buildingIterator->getName()) != buildings.end()) && (buildingIterator->state == Building::State::Ready))
								{
									buildingReady = true;
									break;
								}
							}

							// if a building is ready, we produce a unit
							if (buildingReady)
							{
								// this way is temporary, maybe we find a nice
								// design to do this better
								if ((currentItem.compare("Probe") == 0) ||
									(currentItem.compare("SCV")   == 0) ||
									(currentItem.compare("Drone") == 0))
								{
									produceUnit(buildings, currentItem, entityCosts.buildTime, Building::ProductionType::WorkerOrder);
								}
								else
								{
									produceUnit(buildings, currentItem, entityCosts.buildTime, Building::ProductionType::UnitOrder);
								}

								_buildList.setCurrentItemOk();
								std::cout << currentItem << " (" << time/60 << ":" << time%60 << ")" << std::endl;
							}
							else
							{
								PROGRESS("Simulation::run() Buildings not yet ready for unit production");
							}
						}
						else
						{
							// the item doesn't exist in the tech list
							throw std::runtime_error("Simulation: Item in build list was not found in tech tree.");
						}
					}

					// we go to the next item
					buildListState = _buildList.advance();
				}

				time++;
				timeStep();
			}


            std::cout << "Produced Minerals: " << _gameState->getMinerals() << std::endl;
}

void Simulation::startSimulation()
{
}

Simulation::~Simulation()
{
}
