#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include "GameState.h"

class ResourceManager
{
	private:
		GameState * gameState;
		void update();
	public:
		void timeStep();

};

#endif
