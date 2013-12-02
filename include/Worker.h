#ifndef _WORKER_H_
#define _WORKER_H_

#include <Entity.h>

class Worker
{
	private:
		int timer;

	public:
        enum class State
        {
            CollectingMinerals,
            CollectingVespene,
            Constructing,
            Delayed
        };

		void timeStep(){}        
		int getTimer();

};
#endif
