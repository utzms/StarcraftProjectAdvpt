#ifndef _WORKER_H_
#define _WORKER_H_

#include <memory>


class Worker
{

	private:
		std::string _name;

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
		int timer;
		std::string buildingName;

		Worker(std::string name)
			:_name(name)
			,state(State::Ready)
		{
		}

		void timeStep()
		{
			if (state == State::Constructing)
			{
				timer--;
			}

        }

		std::string getName()
		{
			return _name;
		}


};
#endif
