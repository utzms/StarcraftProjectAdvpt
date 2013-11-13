#ifndef _WORKER_H_
#define _WORKER_H_

#include <Entity.h>

class Worker : public Entity
{
	private:
		int state;
		int myTimer;
	public:
		void timeStep();
		int getState(){return state;};
		int getTime();

};
#endif
