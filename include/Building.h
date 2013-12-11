#ifndef _BUILDING_H_
#define _BUILDING_H_

#include <queue>
#include <memory>

#include "Unit.h"
#include "Worker.h"
#include "GameState.h"
#include "TechnologyManager.h"

class Building
{
	public:
		enum class ProductionType
		{
			WorkerOrder,
			UnitOrder
		};

	private:
		std::string _name;
		int			_timer;
		std::string _productionUnitName;

		ProductionType _productionType;

		std::shared_ptr<GameState> _gameState;
		std::shared_ptr<TechnologyManager> _technologyManager;

	public:
        enum class State
        {
            UnderConstruction,
            Ready,
            Producing
        };

		State state;

		Building(std::string name, int buildTime, std::shared_ptr<GameState> gameState, std::shared_ptr<TechnologyManager> technologyManager)
			:_name(name)
			,_timer(buildTime)
			,_gameState(gameState)
			,_technologyManager(technologyManager)
			,state(State::UnderConstruction)
		{
		}

		template <class ProductionType>
		void buildUnit(std::string name, int time)
		{
//			if (state == State::Ready)
//			{
//				ProductionType type;
//				_productionType = type;
//				_productionUnitName = name;
//				_timer = time;

//				state = State::Producing;
//			}
//			else
//			{
//				std::cerr << "Building " << _name << " is already producing or being constructed." << std::endl;
//			}
		}

        void timeStep()
        {
//			if (state != State::Ready)
//			{
//				_timer--;

//				if (_timer == 0)
//				{
//					if (state == State::Producing)
//					{
//						if (_productionType == ProductionType::WorkerOrder)
//						{
//							_gameState->workerList.push_back(std::shared_ptr<Worker>(new Worker(_productionUnitName, _gameState, _technologyManager)));
//							_technologyManager->notifyCreation(_gameState->workerList.back());
//						}
//						else if (_productionType == ProductionType::UnitOrder)
//						{
//							_gameState->unitList.push_back(std::shared_ptr<Unit>(new Unit(_productionUnitName)));
//							_technologyManager->notifyCreation(_gameState->unitList.back());
//						}

//					}
//					else if (state == State::UnderConstruction)
//					{
//						// nothing, all done in worker
//					}

//					state = State::Ready;
//				}
//			}
        }

        int getTime()
        {
			return _timer;
        }

		std::string getName()
		{
			return _name;
		}
};

#endif
