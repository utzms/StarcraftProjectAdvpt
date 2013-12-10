#include "../include/Simulation.h"

Simulation::Simulation(std::string buildListFilename, std::shared_ptr<GameState> gameState, std::shared_ptr<ResourceManager> resourceManager)
    :_buildList(new BuildList(buildListFilename))
    ,_gameState(gameState)
    ,_resourceManager(resourceManager)
{

}

void Simulation::run()
{

}

void Simulation::startSimulation()
{
	do	
	{
        std::cout <<  _buildList->current() << std::endl;
	}
	while( _buildList->advance() != BuildList::State::Finished );

}

void Simulation::timeStep()
{

}
