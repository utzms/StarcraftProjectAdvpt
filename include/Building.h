#ifndef _BUILDING_H_
#define _BUILDING_H_

#include <Entity.h>

class Building
{
	private:	
		int timer;

	public:
        enum class State
        {
            UnderConstruction,
            Finished
        };

        void timeStep()
        {

        }

        int getTime()
        {
            return timer;
        }
};

#endif
