#include "Simulation.h"

Simulation::Simulation(std::string buildListFilename)
    :_buildList(new BuildList(buildListFilename))
{

}

void Simulation::timeStep()
{
}
