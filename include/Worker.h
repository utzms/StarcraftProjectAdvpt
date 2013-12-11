#ifndef _WORKER_H_
#define _WORKER_H_

#include <memory>

#include "Building.h"
#include "GameState.h"
#include "TechnologyManager.h"

class Worker
{
	private:
		std::string _name;
		int			_timer;

		std::shared_ptr<GameState> _gameState;
		std::shared_ptr<TechnologyManager> _technologyManager;

		std::shared_ptr<Building> _currentBuilding;

	public:
        enum class State
        {
			Ready,
            CollectingMinerals,
            CollectingVespene,
            Constructing,
            Delayed
        };

        State state;

		Worker(std::string name, std::shared_ptr<GameState> gameState, std::shared_ptr<TechnologyManager> technologyManager)
			:_name(name)
			,_gameState(gameState)
			,_technologyManager(technologyManager)
			,state(State::Ready)
		{
		}

		void buildBuilding(std::string name, int time)
		{
			if (state == State::Ready)
			{
				_currentBuilding = std::shared_ptr<Building>(new Building(name, time, _gameState, _technologyManager));

				_gameState->buildingList.push_back(_currentBuilding);
				_timer = time;

				state = State::Constructing;
			}
			else
			{
				std::cerr << "Worker " << _name << " is already busy." << std::endl;
			}
		}

        void timeStep()
        {
			if (state == State::Constructing)
			{
				_timer--;

				if(_timer == 0)
				{
					_currentBuilding->state = Building::State::Ready;
					_technologyManager->notifyCreation(_currentBuilding);

					state = State::Ready;
				}
			}

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
