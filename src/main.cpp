

#ifndef GENETIC

#include "Simulation.cpp"
#include <stdexcept>

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		std::cerr << "Too few arguments" << std::endl;
	}
	std::string argument(argv[1]);

	try
	{
		Simulation<Protoss> simulation(argument);
		simulation.run();
		return 0;
	}
	catch (std::exception &e)
	{
		std::cerr << "Protoss Error Message: " << e.what() << std::endl;
	}

	try
	{
		Simulation<Terran> simulation(argument);
		simulation.run();
		return 0;
	}
	catch (std::exception &e)
	{
		std::cerr << "Terran Error Message: " << e.what() << std::endl;
	}

	try
	{
		Simulation<Zerg> simulation(argument);
		simulation.run();
		return 0;
	}
	catch (std::exception &e)
	{
		std::cerr << "Zerg Error Message: " << e.what() << std::endl;
	}

	std::cerr << "BuildList invalid. Cannot be built by any race." << std::endl;
	return 1;
}

#else

#include "InitTechTree.hpp"
#include "BuildListGenerator.cpp"
#include "Simulation.cpp"
#include "FitnessPolicy.h"

#include <stdexcept>

int initialCall(int argc, char *argv[])
{
	if(argc < 2)
	{
		std::cerr << "Too few arguments" << std::endl;
	}
	std::string argument(argv[1]);

	try
	{
		for (int i = 0; i < 20000; ++i)
			Simulation<Protoss> simulation(argument);
		//simulation.run();
		return 0;
	}
	catch (std::exception &e)
	{   
		std::cerr << "Protoss Error Message: " << e.what() << std::endl;
	}

	try 
	{   
		Simulation<Terran> simulation(argument);
		return 0;
	}
	catch (std::exception &e)
	{   
		std::cerr << "Terran Error Message: " << e.what() << std::endl;
	}

	try 
	{   
		Simulation<Zerg> simulation(argument);
		return 0;
	}
	catch (std::exception &e)
	{   
		std::cerr << "Zerg Error Message: " << e.what() << std::endl;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	TechnologyList a;
	InitTechTree<Protoss>(&a).initTechTree();
	BuildListGenerator<Protoss> build(a);
	build.initRandomGenerator("Marine",10);
	std::vector<std::shared_ptr<BuildList>> vec;
	vec = build.buildManyRandomLists(30000,30);
	
	return  0;
}
#endif

