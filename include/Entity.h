#ifndef _ENTITY_H_
#define _ENTITY_H_

/** Abstract class all specific types of entities are derived from */
class Entity
{
	public:
		virtual void timeStep()=0;
};

#endif
