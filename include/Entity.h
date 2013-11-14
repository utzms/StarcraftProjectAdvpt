#ifndef _ENTITY_H_
#define _ENTITY_H_

/** Abstract class all specific types of entities are derived from */
#include "TechnologyManager.h"
class Entity
{   
    private:
        TechnologyManager * technologyManager;
	public:
		virtual void timeStep()=0;
};

#endif
