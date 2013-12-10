#include <Simulation.h>
#include <iomanip> //include this for formatted output
#include <TechnologyList.h>
#include "InitTechTree.hpp"
#include "Technology.h"

int main()
{
	//Simulation simulation;
	Simulation simulation("protoss1.txt");
	simulation.startSimulation();

	return 0;
}
