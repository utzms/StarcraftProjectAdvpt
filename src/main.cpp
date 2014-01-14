

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
#include "Simulation.cpp"
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
	TechnologyList b = a;
	for (int i = 0; i < 200000; ++i)
	{
		a = b;
		b = a;
	}
	a.initRandomGenerator(21, "Probe", 18);
	b.initRandomGenerator(21);
	for (int i = 0; i < 200; ++i)
	{
		std::cout << a.getRandomTechnology() << std::endl;
	}
	//return initialCall(argc, argv);
	return  0;
}
#endif

