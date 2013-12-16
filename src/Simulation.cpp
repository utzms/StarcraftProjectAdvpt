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

inline void temp()
{
	Simulation<Protoss> sim1("bla");
	Simulation<Zerg> sim2("bli");
	Simulation<Terran> sim3("bluuuuub");
}

	template <class RacePolicy>
void Simulation<RacePolicy>::buildBuilding(std::shared_ptr<Worker> workerForBuilding, std::string name ,int time)
{
	std::shared_ptr<Building> buildingToBuild = std::shared_ptr<Building>(new Building(name, time));
	_gameState->buildingList.push_back(buildingToBuild);
	workerForBuilding->timer = time;
	workerForBuilding->buildingName = name;
	workerForBuilding->state = Worker::State::Constructing;
}

	template <class RacePolicy>
void Simulation<RacePolicy>::produceUnit(std::shared_ptr<Building> buildingForProduction, std::string unitName, int time,Building::ProductionType type)
{
	buildingForProduction->productionType = type;
	buildingForProduction->productionUnitName = unitName;
	buildingForProduction->timer = time;
	buildingForProduction->state = Building::State::Producing;
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
void Simulation<RacePolicy>::timeStep()
{
	std::vector< std::shared_ptr<Worker> >&      workerList   = _gameState->workerList;
	std::vector< std::shared_ptr<Building> >&    buildingList = _gameState->buildingList;
	//	std::vector< std::shared_ptr<Unit> >&        unitList     = _gameState->unitList;

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
	//TODO potentielle FEHLERQUELLE wegen vespene guy
	if (ourWorker == nullptr)
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

	_buildList = new BuildList(buildListFilename);


	std::shared_ptr<GameState> gameState(new GameState());
	std::shared_ptr<TechnologyList> technologyList(new TechnologyList());
	std::shared_ptr<ResourceManager> resourceManager(new ResourceManager(gameState, 0.7f, 0.7f));
	std::shared_ptr<TechnologyManager<RacePolicy>> techManager(new TechnologyManager<RacePolicy>(gameState));
	std::shared_ptr<StartingConfiguration> startingConfiguration( new StartingConfiguration(std::string("./data/StartingConfiguration.txt")) );
	std::shared_ptr<GameStateUpdate<RacePolicy>> gameStateUpdate(new GameStateUpdate<RacePolicy>(gameState,techManager));

	if(techManager->isBuildListPossible(_buildList->getAsVector()) == false)
	{
		throw std::invalid_argument("BuildList is invalid. Other race is tested.");
	}

	_gameState = gameState;
	_resourceManager = resourceManager;
	_technologyManager = techManager;
	_startingConfiguration = startingConfiguration;
	_gameStateUpdate = gameStateUpdate;


	std::vector< std::shared_ptr<Worker> >& workerList = gameState->workerList;

	for(int currentWorker = 0; currentWorker < _startingConfiguration->getInitialWorkerCount(); ++currentWorker)
	{
		workerList.push_back(std::shared_ptr<Worker>(new Worker(RacePolicy::getWorker())));
		workerList[currentWorker]->state = Worker::State::CollectingMinerals;
		_technologyManager->notifyCreation(RacePolicy::getWorker());
	}   

	_gameState->buildingList.push_back(std::shared_ptr<Building>(new Building(RacePolicy::getMainBuilding(), 0)));
	_gameState->buildingList.back()->state = Building::State::Ready;

	int supplyToAdd = _technologyManager->getEntityCosts(RacePolicy::getMainBuilding()).supply;
	_technologyManager->notifyCreation(RacePolicy::getMainBuilding());

	_gameState->addMinerals(_startingConfiguration->getInitialMinerals());
	_gameState->addGas(_startingConfiguration->getInitialVespeneGas());
	_gameState->addSupplyMax(supplyToAdd);
	_gameState->addSupply(_startingConfiguration->getInitialWorkerCount());


}

template <class RacePolicy>
void Simulation<RacePolicy>::run()
{
	PROGRESS("Simulation::run() starts ");

	std::vector< std::shared_ptr<Worker> >& workerList = _gameState->workerList;

	BuildList::State buildListState = BuildList::State::InProgress;
	int time = 0;

	PROGRESS("Simulation::run() Starting main loop");
	while (!_buildList->allItemsOk()/*&& missing: check if no items are under construction and no building is producing anymore */)
	{
		_buildList->reset();
		buildListState = BuildList::State::InProgress;

		PROGRESS("Simulation::run() Time " << time);
		PROGRESS("Simulation::run() [Current resources] Minerals: " << _gameState->getMinerals() << " Gas: " << _gameState->getGas() << " Energy: " << _gameState->getEnergy() << " Supply: " << _gameState->getSupply());

		PROGRESS("Simulation::run() Resetting workers to collect resources");

		int neededVespeneGasWorkers = 0;
		for(auto buildingIterator : _gameState->buildingList)
		{	
			if(!(buildingIterator->getName().compare(RacePolicy::getGasHarvestBuilding())))
			{		
				neededVespeneGasWorkers += 5;
			}
		}

		//TODO diese Verteilung ist bullshit, muss geaendert werden!!!
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
			_technologyManager->checkAndGetVanishing(currentItem, techRequirements);

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
				Costs entityCosts = _technologyManager->getEntityCosts(currentItem);

				// reduce minerals and gas by costs
				_gameState->subMinerals(entityCosts.minerals);
				_gameState->subGas(entityCosts.gas);

				//TODO hier muss dann noch mit Supplys geguckt werden
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

							// we have to order the upgrade

							// then we can set the item to ok
							_buildList->setCurrentItemOk();

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
					//TODO ausser bei upgrades brauchen wir hier den Worker
					std::shared_ptr<Worker> ourWorker = getAvailableWorker();

								// we have a worker, let's build something!
					if (ourWorker)
					{
						PROGRESS("Simulation::run() Ordering building " << currentItem);
						buildBuilding(ourWorker, currentItem, entityCosts.buildTime);
						_buildList->setCurrentItemOk();
						std::cout << currentItem << " (" << time/60 << ":" << time%60 << ")" << std::endl;

						if (vanishingZergWorker)
						{
							// destroy zerg worker
							removeWorker(ourWorker, RacePolicy::getWorker());
						}
					}
					else
					{
						std::cerr << "No Worker for building found, is that even possible?" << std::endl;
						break;
					}



					// if we still haven't got a worker
					// we need to try again at a later point							
				}
				else if (_technologyManager->checkIfNameIsUnit(currentItem))
				{
					if (!vanishingRequirements.empty())
					{
						// we assume that units can have only other units as vanishing
						// requirements (not workers)

						for (auto requirementsIterator : vanishingRequirements)
						{
							for (auto unitIterator : _gameState->unitList)
							{
								if (unitIterator->getName().compare(requirementsIterator) == 0)
								{
									removeUnit(unitIterator, requirementsIterator);
									break;
								}
							}
						}
					}

					// it is a unit, so we need to decide if it is a 
					// worker or another unit
					PROGRESS("Simulation::run() Ordering unit " << currentItem);

					// get the buildings this unit can be built from
					std::vector<std::string> buildings = _technologyManager->getBuildingsForUnitProduction(currentItem);
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

					// if a building is ready, we produce a unit
					if (buildingReady)
					{
						// this way is temporary, maybe we find a nice
						// design to do this better
						if ((currentItem.compare(RacePolicy::getWorker()) == 0))
						{
							produceUnit(buildingForProduction, currentItem, entityCosts.buildTime, Building::ProductionType::WorkerOrder);
						}
						else
						{
							produceUnit(buildingForProduction, currentItem, entityCosts.buildTime, Building::ProductionType::UnitOrder);
						}

						_buildList->setCurrentItemOk();
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
			buildListState = _buildList->advance();
		}

		time++;
		timeStep();
	}


	std::cout << "Produced Minerals: " << _gameState->getMinerals() << std::endl;
}

template <class RacePolicy>
Simulation<RacePolicy>::~Simulation()
{
}

