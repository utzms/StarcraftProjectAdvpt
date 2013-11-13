#ifndef _BUILDING_H_
#define _BUILDING_H_

#include <Entity.h>

class Building : public Entity
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
