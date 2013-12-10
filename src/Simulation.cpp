#include "Simulation.h"

Simulation::Simulation(std::string buildListFilename)
:_buildList(new BuildList(buildListFilename))
{



}



void Simulation::startSimulation()
{
	do	
	{
		std::cout <<  /*_buildList->current()*/"sdasd" << std::endl;
	}
	while( _buildList->advance() != BuildList::State::Finished );

}

void Simulation::timeStep()
{

}
