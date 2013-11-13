#ifndef _UNIT_H_
#define _UNIT_H_

#include "Entity.h"

class Unit : public Entity
{
	private:
		int state;
	public:
		void timeStep();
		int getState(){return state;};

};

#endif
