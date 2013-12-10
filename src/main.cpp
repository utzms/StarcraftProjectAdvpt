#include <iomanip> //include this for formatted output

#include "../include/Simulation.h"
#include "../include/TechnologyList.h"
#include "../include/InitTechTree.hpp"
#include "../include/Technology.h"

int main()
{
	//Simulation simulation;
	Simulation simulation("protoss1.txt");
	simulation.startSimulation();

	return 0;
}
