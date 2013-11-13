#ifndef _TECHNOLOGYMANAGER_H_
#define _TECHNOLOGYMANAGER_H_

#include "GameState.h"
#include "Entity.h"

class TechnologyManager
{
	private:
		GameState * gameState;
		//irgendwie muss er hier noch den tech-tree halten
	public:
		bool Request(Entity entity);//returns true, if requested building/unit/... can be built
		//noch eine funktion, die zurueckliefert an was die anfrage scheitert....rueckgabewert hier das problem
};

#endif
