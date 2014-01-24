
#include "RacePolicy.h"
#include "FitnessPolicy.h"
#include "BuildListOptimizer.cpp"
#include <chrono>

#include <stdexcept>

int oldCall(int argc, char **argv)
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



int main(int argc, char *argv[])
{
    //oldCall(argc, argv);
	size_t a=25;
        BuildListOptimizer<Protoss,Debug> opt(100,a);
        auto startTime = std::chrono::system_clock::now().time_since_epoch().count();
        opt.initialize("Zealot",10,500000,1000);
        auto endTime =  std::chrono::system_clock::now().time_since_epoch().count();
        std::cout << "Size of the population: " << opt.getPopulationSize() << std::endl;
        std::cout << opt.getFittestIndividual();
        std::cout << "Initialize required the following time to run: " << endTime-startTime << std::endl;
        //opt.optimize("Zealot",10,1000000,1,5,5,5);
        //std::cout << "Size of the population: " << opt.getPopulationSize() << std::endl;
        //std::cout << opt.getFittestIndividual();
	return  0;
}

