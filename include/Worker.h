#ifndef _WORKER_H_
#define _WORKER_H_

#include <Entity.h>

class Worker : public Entity
{
	private:
		int timer;
	public:
		void timeStep();
		int getState()
        {
            return state;
        }
		int getTime();

};
#endif
