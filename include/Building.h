#ifndef _BUILDING_H_
#define _BUILDING_H_

#include <Entity.h>

//for using the notify-function
#include <TechnologyManager.h>

class Building : public Entity
{
	private:	
		int timer;
	public:
		void timeStep();
		int getTimer();
};

#endif
