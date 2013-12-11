#ifndef GAMESTATEUPDATE_H
#define GAMESTATEUPDATE_H
#include "GameState.h"
#include "TechnologyManager.h"

class GameStateUpdate
{

	private:
		std::shared_ptr<GameState> _gameState;
		std::shared_ptr<TechnologyManager> _technologyManager;

	public:


		GameStateUpdate(std::shared_ptr<GameState> gameState):_gameState(gameState)
		{
		}

        void timeStep()
		{
			std::vector< std::shared_ptr<Worker> >&      workerList   = _gameState->workerList;
			std::vector< std::shared_ptr<Building> >&    buildingList = _gameState->buildingList;

			//update buildings
			for (auto workerIterator : workerList)
			{
				if(workerIterator->timer == 0)
				{
					workerIterator->state = Worker::State::Ready;
				}
			}

			//update workers
			for (auto buildingIterator : buildingList)
			{
				if(buildingIterator->timer == 0)
				{
					if (buildingIterator->state == Building::State::Producing)
					{
						if (buildingIterator->productionType == Building::ProductionType::WorkerOrder)
						{
							_gameState->workerList.push_back(std::shared_ptr<Worker>(new Worker(buildingIterator->productionUnitName)));
							_technologyManager->notifyCreation(_gameState->workerList.back());
						}
						else if (buildingIterator->productionType == Building::ProductionType::UnitOrder)
						{
							_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit(buildingIterator->productionUnitName)));
							_technologyManager->notifyCreation(_gameState->unitList.back());
						}

					}
					else if (buildingIterator->state == Building::State::UnderConstruction)
					{
						_technologyManager->notifyCreation(buildingIterator);
					}
					buildingIterator->state == Building::State::Ready;
				}
			}

		}

		void buildBuilding(std::shared_ptr<Worker> workerForBuilding, std::string name ,int time)
		{

			std::shared_ptr<Building> buildingToBuild = std::shared_ptr<Building>(new Building(name, time));
			_gameState->buildingList.push_back(buildingToBuild);
			workerForBuilding->timer = time;
			workerForBuilding->buildingName = name;
			workerForBuilding->state = Worker::State::Constructing;

		}

		template<class ProductionType> void produceUnit(std::string name, int time)
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

			ProductionType type;
			BuildingForProduction->productionType = type;
			BuildingForProduction->productionUnitName = name;
			BuildingForProduction->timer = time;
			BuildingForProduction->state = Building::State::Producing;
		}
};

#endif // GAMESTATEUPDATE_H
