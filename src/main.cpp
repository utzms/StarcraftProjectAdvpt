
#include "../include/RacePolicy.h"
#include "../include/FitnessPolicy.h"
#include "../include/BuildListOptimizer.h"
#include "../include/TemplateInstantiations.h"
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

void testTechList()
{
	TechnologyList a;
	InitTechTree<Protoss>(&a).initTechTree();
	TechnologyList b = a;

	a.printAll();
	b.printAll();

	auto t = a.findUnit("Probe");
	t->setExistence(1);
	a.printAll();
	b.printAll();
}


int main(int argc, char *argv[])
{
    //oldCall(argc, argv);
	size_t a=50;
        BuildListOptimizer<Protoss,Debug> opt(100,a);
        auto startTime = std::chrono::system_clock::now().time_since_epoch().count();
		try{
        opt.initialize("Zealot",10,1000000,100);
        //std::cout << "Size of the population: " << opt.getPopulationSize() << std::endl;
        //std::cout << opt.getFittestIndividual();
        opt.optimize("Zealot",10,1000000,1,10,10,10);
		} catch(std::exception &e)
		{
			std::cerr << "Fehler\t" <<e.what();
		}
        auto endTime =  std::chrono::system_clock::now().time_since_epoch().count();
        std::cout << "Required the following time to run: " << (endTime-startTime)*std::chrono::system_clock::period::num/std::chrono::system_clock::period::den << "s" << std::endl;
        std::cout << "Size of the population: " << opt.getPopulationSize() << std::endl;
        std::cout << opt.getFittestIndividual();
	//testTechList();

	return  0;
}

