#ifndef GAMESTATEUPDATE_H
#define GAMESTATEUPDATE_H

#include "GameState.h"
#include "TechnologyManager.h"

template <class RacePolicy>
class GameStateUpdate
{
	private:
		std::shared_ptr<GameState> _gameState;
		std::shared_ptr<TechnologyManager<RacePolicy> > _technologyManager;

	public:
		GameStateUpdate(std::shared_ptr<GameState> gameState):_gameState(gameState)
		{
			PROGRESS("GSU Constructor");
		}

		//add another constructor for techManager
		GameStateUpdate(std::shared_ptr<GameState> gameState, std::shared_ptr<TechnologyManager<RacePolicy>> technologyManager):_gameState(gameState),_technologyManager(technologyManager)
		{
			PROGRESS("GSU Constructor");
		}

        void timeStep()
		{
			PROGRESS("GSU timeStep() start");
			std::vector< std::shared_ptr<Unit> >&      unitList   = _gameState->unitList;
			std::vector< std::shared_ptr<Worker> >&      workerList   = _gameState->workerList;
			std::vector< std::shared_ptr<Building> >&    buildingList = _gameState->buildingList;

			PROGRESS("GSU Updating Workers");
			//update units

			std::vector<std::shared_ptr<Unit>> vanishingUnits;
			std::vector<std::shared_ptr<Unit>> spawningUnits;
			std::vector<std::shared_ptr<Worker>> spawningWorkers;
            bool archonSpawned = false;

			for (auto unitIterator : unitList)
			{

				if( unitIterator->timer == 0  && unitIterator->state == Unit::State::Morphing)
				{	
                    if (unitIterator->morphTargetName.compare(RacePolicy::getWorker()) == 0)
					{
						spawningWorkers.push_back(std::shared_ptr<Worker>(new Worker(unitIterator->morphTargetName)));
					}
                    else if (unitIterator->morphTargetName.compare("Archon") == 0)
                    {
                        if(!archonSpawned)
                        {
                             spawningUnits.push_back(std::shared_ptr<Unit>(new Unit(unitIterator->morphTargetName)));
                             archonSpawned = true;
                        }
                    }
                    else
                    {
                             spawningUnits.push_back(std::shared_ptr<Unit>(new Unit(unitIterator->morphTargetName)));
                    }

					vanishingUnits.push_back(unitIterator);
					PROGRESS("GSU finished morphing " << unitIterator->getName() << " into " << unitIterator->morphTargetName);
				}
			}

			for(auto vanishingUnitsIterator : vanishingUnits)
			{
				auto unitIterator = std::find(_gameState->unitList.begin(), _gameState->unitList.end(), vanishingUnitsIterator);
				if(unitIterator != unitList.end())
				{
					PROGRESS("GSU Destroyed " << (*unitIterator)->getName());
					unitList.erase(unitIterator);
					_technologyManager->notifyDestruction((*unitIterator)->getName());					
				}
			}

			for (auto spawningIterator : spawningUnits)
			{
				unitList.push_back(spawningIterator);
				_technologyManager->notifyCreation(unitList.back()->getName());
			}


			for (auto spawningIterator : spawningWorkers)
			{
				workerList.push_back(spawningIterator);
				_technologyManager->notifyCreation(workerList.back()->getName());
			}

			//update workers
			for (auto workerIterator : workerList)
			{
				if(  workerIterator->timer == 0 && 
					((workerIterator->state != Worker::State::CollectingMinerals) &&
					 (workerIterator->state != Worker::State::CollectingVespene)))
				{
					workerIterator->state = Worker::State::Ready;
				}
			}

			PROGRESS("GSU Updating Buildings");
			//update buildings
			for (auto buildingIterator : buildingList)
			{
				if (buildingIterator->upgradeTimer == 0)
				{
					PROGRESS("GSU: Building " << buildingIterator->getName() << " timer == 0");
					if (buildingIterator->upgradeState == Building::UpgradeState::Upgrading)
					{
						Building::State tempState = buildingIterator->state;
						int tempTimer = buildingIterator->timer;
						std::string unitName = buildingIterator->productionUnitName;
						Building::ProductionType tempType = buildingIterator->productionType;

						_technologyManager->notifyCreation(buildingIterator->targetUpgradeName);
						buildingIterator = std::shared_ptr<Building>(new Building(buildingIterator->targetUpgradeName, 0));

						buildingIterator->state = tempState;
						buildingIterator->timer = tempTimer;
						buildingIterator->productionUnitName = unitName;
						buildingIterator->productionType = tempType;
					}
				}

				if(buildingIterator->timer == 0)
				{

					if (buildingIterator->state == Building::State::Producing || buildingIterator->state == Building::State::ProducingDouble)
					{						
						PROGRESS("GSU: Building " << buildingIterator->getName() << " has produced");
						bool doubleProduction = (buildingIterator->state == Building::State::ProducingDouble);

						Costs unitCosts = _technologyManager->getEntityCosts(buildingIterator->productionUnitName);

                        if (buildingIterator->productionType == Building::ProductionType::WorkerOrder)
						{
							PROGRESS("a worker");
							_gameState->workerList.push_back(std::shared_ptr<Worker>(new Worker(buildingIterator->productionUnitName)));
							_technologyManager->notifyCreation(_gameState->workerList.back()->getName());							
							_gameState->addSupply(unitCosts.supply);

							if (doubleProduction)
							{
								PROGRESS("a worker");
								_gameState->workerList.push_back(std::shared_ptr<Worker>(new Worker(buildingIterator->productionUnitName)));
								_technologyManager->notifyCreation(_gameState->workerList.back()->getName());							
								_gameState->addSupply(unitCosts.supply);
							}
						}
                        else if (buildingIterator->productionType == Building::ProductionType::UnitOrder)
						{
							PROGRESS("a unit");
							_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit(buildingIterator->productionUnitName)));
							_technologyManager->notifyCreation(_gameState->unitList.back()->getName());

							if (buildingIterator->productionUnitName.compare("Overlord") == 0)
							{
								_gameState->addSupplyMax(8);
							}
							else
							{
								_gameState->addSupply(unitCosts.supply);
							}

							if (doubleProduction)
							{
								PROGRESS("a unit");
								_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit(buildingIterator->productionUnitName)));
								_technologyManager->notifyCreation(_gameState->unitList.back()->getName());

								if (buildingIterator->productionUnitName.compare("Overlord") == 0)
								{
									_gameState->addSupplyMax(8);
								}
								else
								{
									_gameState->addSupply(unitCosts.supply);
								}
							}
						}
					}
					else if (buildingIterator->state == Building::State::UnderConstruction)
					{
						PROGRESS("GSU: Building " << buildingIterator->getName() << " has been constructed");
						_technologyManager->notifyCreation(buildingIterator->getName());

						Costs buildingCosts = _technologyManager->getEntityCosts(buildingIterator->getName());
						_gameState->addSupplyMax(buildingCosts.supply);
					}

					PROGRESS("GSU: Building " << buildingIterator->getName() << " is ready");
                    buildingIterator->state = Building::State::Ready;
				}
			}

		}


};

#endif // GAMESTATEUPDATE_H
