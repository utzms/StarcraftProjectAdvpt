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
			PROGRESS("GSU Constructor");
		}

		//add another constructor for techManager
		GameStateUpdate(std::shared_ptr<GameState> gameState, std::shared_ptr<TechnologyManager> technologyManager):_gameState(gameState),_technologyManager(technologyManager)
		{
			PROGRESS("GSU Constructor");
		}

        void timeStep()
		{
			PROGRESS("GSU STEP1");
			std::vector< std::shared_ptr<Worker> >&      workerList   = _gameState->workerList;
			PROGRESS("GSU STEP2");
			std::vector< std::shared_ptr<Building> >&    buildingList = _gameState->buildingList;
			PROGRESS("GSU STEP3");

			//update buildings
			for (auto workerIterator : workerList)
			{
				if(workerIterator->timer == 0)
				{
					workerIterator->state = Worker::State::Ready;
				}
			}
			PROGRESS("GSU STEP4");

			//update workers
			for (auto buildingIterator : buildingList)
			{
			PROGRESS("GSU STEP5");
				if(buildingIterator->timer == 0)
				{
			PROGRESS("GSU STEP6");
					if (buildingIterator->state == Building::State::Producing)
					{
			PROGRESS("GSU STEP7");
                        if (buildingIterator->productionType == Building::ProductionType::WorkerOrder)
						{
			PROGRESS("GSU STEP8");
							_gameState->workerList.push_back(std::shared_ptr<Worker>(new Worker(buildingIterator->productionUnitName)));
							_technologyManager->notifyCreation(_gameState->workerList.back()->getName());
						}
                        else if (buildingIterator->productionType == Building::ProductionType::UnitOrder)
						{
			PROGRESS("GSU STEP9");
							_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit(buildingIterator->productionUnitName)));
							_technologyManager->notifyCreation(_gameState->unitList.back()->getName());
						}

					}
					else if (buildingIterator->state == Building::State::UnderConstruction)
					{
			PROGRESS("GSU STEP10");
			PROGRESS(buildingIterator->getName());
						_technologyManager->notifyCreation(buildingIterator->getName());
					}
			PROGRESS("GSU STEP11");
                    buildingIterator->state = Building::State::Ready;
					}
			}

		}


};

#endif // GAMESTATEUPDATE_H
