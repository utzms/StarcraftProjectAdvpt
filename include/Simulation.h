#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include "GameState.h"
#include "ResourceManager.h"
#include "TechnologyManager.h"
#include "Entity.h"

class Simulation
{
	private:
		GameState * gameState;
		ResourceManager * resourceManager;
		TechnologyManager * technologyManager;
		void timeStep();
	public:
		void startSimulation();

};

#endif
