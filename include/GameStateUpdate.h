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
                    buildingIterator->state = Building::State::Ready;
				}
			}

		}


};

#endif // GAMESTATEUPDATE_H
