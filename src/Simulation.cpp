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


	template <class RacePolicy>
void Simulation<RacePolicy>::buildBuilding(std::shared_ptr<Worker> workerForBuilding, std::string name ,int time)
{
	std::shared_ptr<Building> buildingToBuild = std::shared_ptr<Building>(new Building(name, time));
	_gameState->buildingList.push_back(buildingToBuild);
	if(workerForBuilding->getName().compare(Protoss::getWorker()) != 0)
	{
		workerForBuilding->timer = time;
		workerForBuilding->state = Worker::State::Constructing;
	}
	workerForBuilding->buildingName = name;

}

	template <class RacePolicy>
void Simulation<RacePolicy>::produceUnit(std::shared_ptr<Building> buildingForProduction, std::string unitName, int time,Building::ProductionType type, bool doubleProduction)
{
    _energizer->handleBoost(buildingForProduction);
	buildingForProduction->productionType = type;
	buildingForProduction->productionUnitName = unitName;
	buildingForProduction->timer = time;

	if (!doubleProduction)
	{
		buildingForProduction->state = Building::State::Producing;
	}
	else
	{
		buildingForProduction->state = Building::State::ProducingDouble;
	}
}

template <class RacePolicy>
void Simulation<RacePolicy>::removeWorker(std::shared_ptr<Worker> unitForRemoval, std::string unitName)
{
	// we know it's a Worker
	auto unitIterator = std::find(_gameState->workerList.begin(), _gameState->workerList.end(), unitForRemoval);

	if (unitIterator != _gameState->workerList.end())
	{
		_gameState->workerList.erase(unitIterator);
	}
	else
	{
		throw std::invalid_argument("Simulation::removeUnit() Worker for removal is not in worker list.");
	}

	_gameState->subSupply(1.f);

	_technologyManager->notifyDestruction(unitName);
}

template <class RacePolicy>
void Simulation<RacePolicy>::removeUnit(std::shared_ptr<Unit> unitForRemoval, std::string unitName)
{
	if (!_technologyManager->checkIfNameIsUnit(unitName))
	{
		throw std::invalid_argument("Simulation::removeUnit() Entity for removal is not a unit or worker.");
	}

	// we know it's a Unit
	auto unitIterator = std::find(_gameState->unitList.begin(), _gameState->unitList.end(), unitForRemoval);

	if (unitIterator != _gameState->unitList.end())
	{
		_gameState->unitList.erase(unitIterator);
	}
	else
	{
		throw std::invalid_argument("Simulation::removeUnit() Unit for removal is not in unit list.");
	}

	_technologyManager->notifyDestruction(unitName);
}

template <class RacePolicy>
void Simulation<RacePolicy>::morphUnit(std::shared_ptr<Unit> unitForMorphing, int morphTime, std::string morphTargetName)
{
	if (!_technologyManager->checkIfNameIsUnit(unitForMorphing->getName()))
	{
		throw std::invalid_argument("Simulation::morphUnit() Entity for morphing is not a unit.");
	}

	// we know it's a Unit
	auto unitIterator = std::find(_gameState->unitList.begin(), _gameState->unitList.end(), unitForMorphing);

	if (unitIterator != _gameState->unitList.end())
	{
		(*unitIterator)->state = Unit::State::Morphing;
		(*unitIterator)->timer = morphTime;
		(*unitIterator)->morphTargetName = morphTargetName;
	}
	else
	{
		throw std::invalid_argument("Simulation::morphUnit() Unit for morphing is not in unit list.");
	}

	PROGRESS("Simulation::morphUnit() started morphing " << unitForMorphing->getName() << " into " << morphTargetName);
}

	template <class RacePolicy>
void Simulation<RacePolicy>::timeStep()
{
	std::vector< std::shared_ptr<Worker> >&      workerList   = _gameState->workerList;
	std::vector< std::shared_ptr<Building> >&    buildingList = _gameState->buildingList;
	std::vector< std::shared_ptr<Unit> >&        unitList     = _gameState->unitList;

	for (auto workerIterator : workerList)
	{
		workerIterator->timeStep();
	}

	for (auto unitIterator : unitList)
	{
		unitIterator->timeStep();
	}

	for (auto buildingIterator : buildingList)
	{
		buildingIterator->timeStep();
	}
	_resourceManager->timeStep();
	_gameStateUpdate->timeStep();
    _energizer->update();
}

template <class RacePolicy> std::shared_ptr<Worker> Simulation<RacePolicy>::getAvailableWorker()
{
	std::shared_ptr<Worker> ourWorker(nullptr); 
	// first check for ready workers, so we only
	// use workers who are collecting if necessary
	for (auto workerIterator : _gameState->workerList)
	{
		if (workerIterator->state == Worker::State::Ready || workerIterator->state == Worker::State::CollectingMinerals)
		{
			ourWorker = workerIterator;
			break;
		}
	}
	// if we haven't found a ready worker, we 
	// have to take a minerals or vespene guy
/*	if (ourWorker == nullptr)
	{
		for (auto workerIterator : _gameState->workerList)
		{
			if (workerIterator->state == Worker::State::CollectingMinerals || workerIterator->state == Worker::State::CollectingVespene)
			{
				ourWorker = workerIterator;
				break;
			}
		}
	}
*/
	return ourWorker;
}
/** Function starting the Simulation.
  *
  * After calling this function all required objects, including the whole GameState, the TechnologyManager and ResourceManager, are created.
  * Next the Simulation is started by entering the first timestep and proceded 
  * by executing all necessary actions in an arbitrary amount of timesteps until the GameState fits to the specifed goal.
  */

template <class RacePolicy>
Simulation<RacePolicy>::Simulation(std::string buildListFilename)
{

	_buildList = std::shared_ptr<BuildList>(new BuildList(buildListFilename));


	std::shared_ptr<GameState> gameState(new GameState());
	std::shared_ptr<ResourceManager> resourceManager(new ResourceManager(gameState, 0.35f, 0.7f));
	std::shared_ptr<TechnologyManager<RacePolicy>> techManager(new TechnologyManager<RacePolicy>(gameState));
	std::shared_ptr<StartingConfiguration> startingConfiguration( new StartingConfiguration(std::string("./data/StartingConfiguration.txt")) );
	std::shared_ptr<GameStateUpdate<RacePolicy>> gameStateUpdate(new GameStateUpdate<RacePolicy>(gameState,techManager));
    std::shared_ptr<Energizer<RacePolicy>> energizer(new Energizer<RacePolicy>(gameState,techManager));


	if(techManager->isBuildListPossible(_buildList->getAsVector()) == false)
	{
		throw std::invalid_argument("BuildList is invalid. Other race is tested.");
	}

	_gameState = gameState;
	_resourceManager = resourceManager;
	_technologyManager = techManager;
	_startingConfiguration = startingConfiguration;
	_gameStateUpdate = gameStateUpdate;
    _energizer = energizer;


	std::vector< std::shared_ptr<Worker> >& workerList = gameState->workerList;

	for(int currentWorker = 0; currentWorker < _startingConfiguration->getInitialWorkerCount(); ++currentWorker)
	{
		workerList.push_back(std::shared_ptr<Worker>(new Worker(RacePolicy::getWorker())));
		workerList[currentWorker]->state = Worker::State::CollectingMinerals;
		_technologyManager->notifyCreation(RacePolicy::getWorker());
	}   

	_gameState->buildingList.push_back(std::shared_ptr<Building>(new Building(RacePolicy::getMainBuilding(), 0)));
	_gameState->buildingList.back()->state = Building::State::Ready;

    int supplyToAdd = _technologyManager->getCostsForName(RacePolicy::getMainBuilding()).supply;
	_technologyManager->notifyCreation(RacePolicy::getMainBuilding());

	// again special zerg handling
	if (RacePolicy::getMainBuilding().compare("Hatchery") == 0)
	{
		supplyToAdd += 8;

		_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Overlord")));
		_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Larva")));
		_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Larva")));
		_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Larva")));

		_technologyManager->notifyCreation("Overlord");
		_technologyManager->notifyCreation("Larva");
		_technologyManager->notifyCreation("Larva");
		_technologyManager->notifyCreation("Larva");
	}

	_gameState->addMinerals(_startingConfiguration->getInitialMinerals());
	_gameState->addGas(_startingConfiguration->getInitialVespeneGas());
	_gameState->addSupplyMax(supplyToAdd);
	_gameState->addSupply(_startingConfiguration->getInitialWorkerCount());


}
/* Constructor for use in BuildListOptimizer */
template<class RacePolicy>
Simulation<RacePolicy>::Simulation(std::shared_ptr<BuildList> buildList, const TechnologyList& techList)
{
	_buildList = buildList;

	std::shared_ptr<GameState> gameState(new GameState());
	std::shared_ptr<ResourceManager> resourceManager(new ResourceManager(gameState, 0.35f, 0.7f));
	std::shared_ptr<TechnologyManager<RacePolicy>> techManager(new TechnologyManager<RacePolicy>(gameState, techList));
	std::shared_ptr<StartingConfiguration> startingConfiguration( new StartingConfiguration(std::string("./data/StartingConfiguration.txt")) );
	std::shared_ptr<GameStateUpdate<RacePolicy>> gameStateUpdate(new GameStateUpdate<RacePolicy>(gameState,techManager));
    std::shared_ptr<Energizer<RacePolicy>> energizer(new Energizer<RacePolicy>(gameState,techManager));

	_gameState = gameState;
	_resourceManager = resourceManager;
	_technologyManager = techManager;
	_startingConfiguration = startingConfiguration;
	_gameStateUpdate = gameStateUpdate;
    _energizer = energizer;

	std::vector< std::shared_ptr<Worker> >& workerList = gameState->workerList;

	for(int currentWorker = 0; currentWorker < _startingConfiguration->getInitialWorkerCount(); ++currentWorker)
	{
		workerList.push_back(std::shared_ptr<Worker>(new Worker(RacePolicy::getWorker())));
		workerList[currentWorker]->state = Worker::State::CollectingMinerals;
		_technologyManager->notifyCreation(RacePolicy::getWorker());
	}   

	_gameState->buildingList.push_back(std::shared_ptr<Building>(new Building(RacePolicy::getMainBuilding(), 0)));
	_gameState->buildingList.back()->state = Building::State::Ready;

    int supplyToAdd = _technologyManager->getCostsForName(RacePolicy::getMainBuilding()).supply;
	_technologyManager->notifyCreation(RacePolicy::getMainBuilding());

	// again special zerg handling
	if (RacePolicy::getMainBuilding().compare("Hatchery") == 0)
	{
		supplyToAdd += 8;

		_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Overlord")));
		_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Larva")));
		_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Larva")));
		_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Larva")));

		_technologyManager->notifyCreation("Overlord");
		_technologyManager->notifyCreation("Larva");
		_technologyManager->notifyCreation("Larva");
		_technologyManager->notifyCreation("Larva");
	}

	_gameState->addMinerals(_startingConfiguration->getInitialMinerals());
	_gameState->addGas(_startingConfiguration->getInitialVespeneGas());
	_gameState->addSupplyMax(supplyToAdd);
	_gameState->addSupply(_startingConfiguration->getInitialWorkerCount());
}


/* run() method for use in BuildListOptimizer */

template <class RacePolicy>
std::multimap<int, std::string> Simulation<RacePolicy>::run(int timeLimit)
{
	PROGRESS("Simulation::run() starts ");

    std::multimap<int, std::string> resultMap;

	std::vector< std::shared_ptr<Worker> >& workerList = _gameState->workerList;

	BuildList::State buildListState = BuildList::State::InProgress;
	int time = 0;

	PROGRESS("Simulation::run() Starting main loop");
	while (!_buildList->allItemsOk() && time < timeLimit/*&& missing: check if no items are under construction and no building is producing anymore */)
	{
		_buildList->reset();
		buildListState = BuildList::State::InProgress;

		PROGRESS("Simulation::run() Time " << time);
		PROGRESS("Simulation::run() [Current resources] Minerals: " << _gameState->getMinerals() << " Gas: " << _gameState->getGas() << " Energy: " << _gameState->getEnergy() << " Supply: " << _gameState->getAvailableSupply());

		PROGRESS("Simulation::run() Resetting workers to collect resources");

		unsigned int neededVespeneGasWorkers = 0;
		for(auto buildingIterator : _gameState->buildingList)
		{	
			if(!(buildingIterator->getName().compare(RacePolicy::getGasHarvestBuilding())))
			{		
				neededVespeneGasWorkers += 3;
				if (neededVespeneGasWorkers >= workerList.size())
				{
					neededVespeneGasWorkers -= 3;
				}
			}
		}

		for(auto workerIterator : workerList)
		{
			if( workerIterator->state == Worker::State::Ready ||
					workerIterator->state == Worker::State::CollectingMinerals ||
					workerIterator->state == Worker::State::CollectingVespene)
			{
				//send ready workers to the mineralfields
				if(neededVespeneGasWorkers !=0)				
				{
					workerIterator->state = Worker::State::CollectingVespene;
					--neededVespeneGasWorkers;
				}
				else
				{
					workerIterator->state = Worker::State::CollectingMinerals;
				}
			}
		}

		// special case for zerg
		// update larvae
		static int larvaTimer = 0;
		if (RacePolicy::getRace() == RaceType::Zerg)
		{
			int larvaCount = 0;
			for (auto unitIterator : _gameState->unitList)
			{
				if (unitIterator->getName().compare("Larva") == 0)
				{
					++larvaCount;
				}
			}

			if (larvaCount < 3)
			{
				if (larvaTimer >= 15)
				{
                    std::vector<std::string> larvaBuildings = _technologyManager->getProductionBuildingsForName("Larva");

					std::shared_ptr<Building> larvaBuildingInGameState(nullptr);

					for (auto larvaBuilding : larvaBuildings)
					{
						for (auto buildingIterator : _gameState->buildingList)
						{
							if (buildingIterator->getName().compare(larvaBuilding) == 0)
							{
								larvaBuildingInGameState = buildingIterator;
								break;
							}
						}

						if (larvaBuildingInGameState)
						{
							break;
						}
					}

					if (!larvaBuildingInGameState)
					{
						throw std::invalid_argument("Simulation::run() No larva producing building :(");
					}
					else
					{
						_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit("Larva")));
						_technologyManager->notifyCreation("Larva");
					}

					larvaTimer = 0;
				}
				else
				{

				}

			}

			++larvaTimer;
		}

        //ENERGY handling!
        _energizer->handleAbility();

        while (buildListState != BuildList::State::Finished)
		{
			if (_buildList->allItemsOk())
			{
				break;
			}

			// go to the first element thats not ok
			BuildList::State innerListState = BuildList::State::InProgress;
			while (innerListState != BuildList::State::Finished)
			{
				if (!_buildList->isCurrentItemOk())
				{
					break;
				}

				innerListState = _buildList->advance();						
			}

			// it seems we tried every item on the list,
			// now its better to go to the next time step.
			// we won't be able to build anything in this one.
			if (innerListState == BuildList::State::Finished)
			{
				break;
			}

			std::string currentItem = _buildList->current();

			// check the requirements
			std::pair<bool, std::vector<std::string> > techRequirements;
            _technologyManager->checkEverythingAndGetVanishing(currentItem, techRequirements);

			// if we do not meet the requirements ...
			if (!techRequirements.first)
			{
				PROGRESS("Simulation::run() Requirements of " << currentItem << " not met, advancing to next item.");
				break;
			}
			else
			{
				std::vector<std::string> vanishingRequirements = techRequirements.second;

				PROGRESS("Simulation::run() Requirements of " << currentItem << " OK");

				// get the costs for game state manipulation and build time
                Costs entityCosts = _technologyManager->getCostsForName(currentItem);

				// otherwise, we find out if the item is a unit or a building
				if (_technologyManager->checkIfNameIsBuilding(currentItem))
				{
					bool vanishingZergWorker = false;

					if (!vanishingRequirements.empty())
					{
						bool isUpgrade = true;
						for (auto requirementsIterator : vanishingRequirements)
						{
							if (_technologyManager->checkIfNameIsUnit(requirementsIterator))
							{
								isUpgrade = false;
							}
						}

						if (isUpgrade)
						{
							// we have a building upgrade
							std::shared_ptr<Building> buildingToBeUpgraded;

							// we have to order the upgrade
							for (auto buildingIterator : _gameState->buildingList)
							{
								if (buildingIterator->getName().compare(vanishingRequirements[0]) == 0)
								{
									buildingToBeUpgraded = buildingIterator;
									break;
								}
							}

							if (!buildingToBeUpgraded)
							{
								throw std::runtime_error("Simulation::run() Couldn't find building in building list when trying to upgrade" +std::to_string(time) +" " + buildingToBeUpgraded->getName());
							}

							buildingToBeUpgraded->upgradeTimer = entityCosts.buildTime;
							buildingToBeUpgraded->upgradeState = Building::UpgradeState::Upgrading;
							buildingToBeUpgraded->targetUpgradeName = currentItem;

							if (buildingToBeUpgraded->getName().compare(RacePolicy::getMainBuilding()) == 0)
							{
								// we have a main building upgrade
								//RacePolicy::upgradeMainBuilding(currentItem);
							}

							// then we can set the item to ok
							_buildList->setCurrentItemOk();

							// reduce minerals and gas by costs
							_gameState->subMinerals(entityCosts.minerals);
							_gameState->subGas(entityCosts.gas);

                            //std::cout << currentItem << " (" << time/60 << ":" << time%60 << ")" << std::endl;

							// write current item to result map
                            resultMap.insert(std::make_pair(time, currentItem));

							// continue to the next item in the build list
							continue;
						}
						else
						{
							// Zerg worker wants to build something
							vanishingZergWorker = true;
						}


					}

					// it is a building, so we need a worker
					std::shared_ptr<Worker> ourWorker = getAvailableWorker();

								// we have a worker, let's build something!
					if (ourWorker)
					{
						PROGRESS("Simulation::run() Ordering building " << currentItem);
						buildBuilding(ourWorker, currentItem, entityCosts.buildTime);
						_buildList->setCurrentItemOk();
                        //std::cout << currentItem << " (" << time/60 << ":" << time%60 << ")" << std::endl;

						// write current item to result map
                        resultMap.insert(std::make_pair(time, currentItem));

						if (vanishingZergWorker)
						{
							// destroy zerg worker
							removeWorker(ourWorker, RacePolicy::getWorker());
						}

						// reduce minerals and gas by costs
						_gameState->subMinerals(entityCosts.minerals);
						_gameState->subGas(entityCosts.gas);
					}
					else
					{
						//std::cerr << "No Worker for building found, is that even possible?" << std::endl;
						break;
					}

				// if we still haven't got a worker
				// we need to try again at a later point		
				}
				else if (_technologyManager->checkIfNameIsUnit(currentItem))
				{
					PROGRESS("Simulation::run() Trying unit " << currentItem);

					// get the buildings this unit can be built from
                    std::vector<std::string> buildings = _technologyManager->getProductionBuildingsForName(currentItem);
					std::shared_ptr<Building> buildingForProduction;
					// check if any of them are ready
					bool buildingReady = false;
					for (auto buildingIterator : _gameState->buildingList)
					{							
						if(( std::find(buildings.begin(), buildings.end(), buildingIterator->getName()) != buildings.end()) && (buildingIterator->state == Building::State::Ready))
						{
							buildingForProduction = buildingIterator;
							buildingReady = true;
							break;
						}
					}

                    if (vanishingRequirements.size() > 0)
					{
						// we assume that units can have only other units as vanishing
						// requirements (not workers)
                        unsigned int morphCounter = 0;

                        for(auto vanishingRequirementsIterator : vanishingRequirements)
                        {
                            for (auto unitIterator : _gameState->unitList)
                            {
                                if ((unitIterator->getName().compare(vanishingRequirementsIterator) == 0) && (unitIterator->state == Unit::State::Ready) && morphCounter < vanishingRequirements.size())
                                {
                                    //PROGRESS("Simulation::run() Deleted unit " << vanishingRequirements[0] << " as vanishing requirement for unit " << currentItem);
                                    //removeUnit(unitIterator, requirementsIterator);
                                    morphUnit(unitIterator, entityCosts.buildTime, currentItem);
                                    morphCounter++;
                                    PROGRESS("morphCounter: " << morphCounter);
                                    if(morphCounter == vanishingRequirements.size())
                                    {
                                        _gameState->subMinerals(entityCosts.minerals);
                                        _gameState->subGas(entityCosts.gas);

                                        _buildList->setCurrentItemOk();
                                        //std::cout << currentItem << " (" << time/60 << ":" << time%60 << ")" << std::endl;

										// write current item to result map
                                        resultMap.insert(std::make_pair(time, currentItem));

                                        PROGRESS("morphCounter reached vanishingSize:" << vanishingRequirements.size() );
                                        break;
                                    }
                                }
                            }
                        }

                        if(morphCounter != vanishingRequirements.size())
                        {
                            PROGRESS("Simulation::run: morphCount != vanishingRequirements");
                            PROGRESS("morphCounter: " << morphCounter);
                            //throw std::runtime_error("Simulation::run: morphCount != vanishingRequirements");
                        }
					}
					// if a building is ready, we produce a unit
					else if (buildingReady)
					{
						bool doubleProduction = false;
						bool doubleProductionRequirements = false;

						// here comes the special case for producing two units simultaneously in a reactor-building for the terrans
						if (RacePolicy::getRace() == RaceType::Terran)
						{
							// we are terrans

							// first see if it is a reactor building
							// if not we dont need to bother
                            if (buildingForProduction->getName().find("Reactor") != std::string::npos)
							{
								// check next item in buildList
                                std::string preview = _buildList->sneakPreview();

                                if (preview.compare(currentItem) == 0)
                                {
                                    // we now know that there are 2 identical units after each other in the build list
                                    // and we have a reactor building
                                    // so we can produce both
                                    doubleProduction = true;
                                }
							}
						}

						// reduce minerals and gas by costs
						_gameState->subMinerals(entityCosts.minerals);
						_gameState->subGas(entityCosts.gas);
						
						if (doubleProduction)
						{
                            Costs nextCosts = _technologyManager->getCostsForName(currentItem);

							if (_gameState->getGas() >= nextCosts.gas &&
								_gameState->getMinerals() >= nextCosts.minerals &&
								_gameState->getAvailableSupply() >= nextCosts.supply * 2)
							{
								std::cout << "Double Production!" << std::endl;
								// we can produce the second unit
								doubleProductionRequirements = true;

								_gameState->subMinerals(entityCosts.minerals);
								_gameState->subGas(entityCosts.gas);

								_buildList->setCurrentItemOk();
								_buildList->advance();
                                resultMap.insert(std::make_pair(time, currentItem));

                                //std::cout << currentItem << " (" << time/60 << ":" << time%60 << ")" << std::endl;
							}
						}

						if ((currentItem.compare(RacePolicy::getWorker()) == 0))
						{
							produceUnit(buildingForProduction, currentItem, entityCosts.buildTime, Building::ProductionType::WorkerOrder, doubleProduction && doubleProductionRequirements);
						}
						else
						{
							produceUnit(buildingForProduction, currentItem, entityCosts.buildTime, Building::ProductionType::UnitOrder, doubleProduction && doubleProductionRequirements);
						}
						
						_buildList->setCurrentItemOk();
                        //std::cout << currentItem << " (" << time/60 << ":" << time%60 << ")" << std::endl;

						// write current item to result map
                        resultMap.insert(std::make_pair(time, currentItem));
					}
					else
					{
						PROGRESS("Simulation::run() Buildings not yet ready for unit production");
						break;
					}

					break;

				}
				else
				{
					// the item doesn't exist in the tech list
					throw std::runtime_error("Simulation: Item in build list was not found in tech tree.");
				}
			}

			// we go to the next item
			buildListState = _buildList->advance();
		}

		time++;
		timeStep();
	}
	
    resultMap.insert(std::make_pair(time, "Time"));

    return resultMap;
}

template <class RacePolicy>
Simulation<RacePolicy>::~Simulation()
{
}

