
#include "../include/RacePolicy.h"
#include "../include/FitnessPolicy.h"
#include "../include/BuildListOptimizer.h"
#include "../include/TemplateInstantiations.h"
#include <chrono>
#include <thread>

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
        const int individualSize = 30;
        const int selectionRate = 65;
        const int mutationRate = 10;
        const int reproductionRate = 20;
        const int initPopSize = 100000;
        const int generations = 50;
        const int accuracy = 100;
        std::cout << "The optimization is done with the following rates:" << std::endl;
        std::cout << "Selection Rate: " << std::to_string(selectionRate) << std::endl;
        std::cout << "Mutation Rate: " << std::to_string(mutationRate) << std::endl;
        std::cout << "Reproduction Rate: " << std::to_string(reproductionRate) << std::endl;
        BuildListOptimizer<Protoss,Debug> opt(accuracy,individualSize);
        std::cout << "The size of every individual is " << std::to_string(individualSize) << " entries." << std::endl;
        std::cout << "We start with " << std::to_string(initPopSize) << " Individuals." << std::endl;
        auto startTime = std::chrono::system_clock::now().time_since_epoch().count();
        try{
        opt.initialize("Zealot",10,1000000,initPopSize);
        //std::cout << "Size of the population: " << opt.getPopulationSize() << std::endl;
        //std::cout << opt.getFittestIndividual();
        opt.optimize("Zealot",10,1000000,generations,reproductionRate,mutationRate,selectionRate);
        } catch(std::exception &e)
        {
            std::cerr << "Fehler\t" <<e.what() << std::endl;
        }
        auto endTime =  std::chrono::system_clock::now().time_since_epoch().count();
        std::cout << "Final size of the population: " << opt.getPopulationSize() << std::endl;
        std::cout << "The top 3 are:" << std::endl;
        std::cout << opt.getFittestGroup(3) << std::endl;
        std::cout << "Required the following time to run: " << (endTime-startTime)*std::chrono::system_clock::period::num/std::chrono::system_clock::period::den << "s" << std::endl;
        std::cout << "Number of threads used: " << std::to_string(std::thread::hardware_concurrency()+1) << std::endl;
	//testTechList();

	return  0;
}

