#ifndef _UNIT_H_
#define _UNIT_H_

#include "Entity.h"

class Unit : public Entity
{   
    private:
        int timer;
	public:
		void timeStep();
        int getTimer();
}
#endif
