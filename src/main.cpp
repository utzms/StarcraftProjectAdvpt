
#include "../include/RacePolicy.h"
#include "../include/FitnessPolicy.h"
#include "../include/BuildListOptimizer.h"
#include "../include/TemplateInstantiations.h"
#include "../include/TechnologyList.h"

#include <stdexcept>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>

template <typename T>
T pushOrRush(std::string in)
{
	T out;
	if (in.compare("Rush"))
	{
		out = FitnessType::Rush;
	} else if (in.compare("Push"))
	{
		out = FitnessType::Push;
	}
	return out;
}

template <typename RacePolicy, typename FitnessPolicy> void writeToFile(BuildListOptimizer<RacePolicy,FitnessPolicy>& opt)
{

    std::ofstream logFile;
    logFile.open("~/Starcraft2Optimizer.log");
    logFile << "The top 10 are:" << std::endl;
    logFile << opt.getFittestGroup(10);
    logFile.close();
}

void startBuildListOptimizer(std::string race, std::string strategy, std::string unit, const size_t timeLimit)
{
    const int individualSize = 30;
    const int selectionRate = 66;
    const int mutationRate = 15;
    const int reproductionRate = 30;
    const int initPopSize = 100000;
    const int generations = 100;
    const int accuracy = 100;
	std::cout << "The optimization is done with the following rates:" << std::endl;
	std::cout << "Selection Rate: " << std::to_string(selectionRate) << std::endl;
	std::cout << "Mutation Rate: " << std::to_string(mutationRate) << std::endl;
	std::cout << "Reproduction Rate: " << std::to_string(reproductionRate) << std::endl;
    std::cout << "The size of every individual is " << std::to_string(individualSize) << " entries." << std::endl;
	std::cout << "We start with " << std::to_string(initPopSize) << " Individuals." << std::endl;

	if (!strategy.compare("Push"))
	{
		if (!race.compare("Protoss"))
		{
			BuildListOptimizer<Protoss,Push> opt(accuracy,individualSize);
			try{
                opt.initialize(unit,10,timeLimit,initPopSize);
                opt.optimize(unit,10,timeLimit,generations,reproductionRate,mutationRate,selectionRate);
			} catch(std::exception &e)
			{
				std::cerr << "Fehler\t" <<e.what() << std::endl;
			}
			std::cout << "Final size of the population: " << opt.getPopulationSize() << std::endl;
			std::cout << "The top 3 are:" << std::endl;
            std::cout << opt.getFittestGroup(3) << std::endl;
            writeToFile(opt);
		} else if (!race.compare("Zerg"))
		{
			BuildListOptimizer<Zerg,Push> opt(accuracy,individualSize);
			try{
                opt.initialize(unit,10,timeLimit,initPopSize);
                opt.optimize(unit,10,timeLimit,generations,reproductionRate,mutationRate,selectionRate);
			} catch(std::exception &e)
			{
				std::cerr << "Fehler\t" <<e.what() << std::endl;
			}
			std::cout << "Final size of the population: " << opt.getPopulationSize() << std::endl;
			std::cout << "The top 3 are:" << std::endl;
			std::cout << opt.getFittestGroup(3) << std::endl;
            writeToFile(opt);
		} else if (!race.compare("Terran"))
		{
			BuildListOptimizer<Terran,Push> opt(accuracy,individualSize);
			try{
                opt.initialize(unit,10,timeLimit,initPopSize);
                opt.optimize(unit,10,timeLimit,generations,reproductionRate,mutationRate,selectionRate);
			} catch(std::exception &e)
			{
				std::cerr << "Fehler\t" <<e.what() << std::endl;
			}
			std::cout << "Final size of the population: " << opt.getPopulationSize() << std::endl;
			std::cout << "The top 3 are:" << std::endl;
            std::cout << opt.getFittestGroup(3) << std::endl;
            writeToFile(opt);

		} else
		{
			throw std::invalid_argument("No known race");
		}
	} else if (!strategy.compare("Rush"))
	{
		if (!race.compare("Protoss"))
		{
			BuildListOptimizer<Protoss,Rush> opt(accuracy,individualSize);
			try{
                opt.initialize(unit,10,timeLimit,initPopSize);
                opt.optimize(unit,10,timeLimit,generations,reproductionRate,mutationRate,selectionRate);
			} catch(std::exception &e)
			{
				std::cerr << "Fehler\t" <<e.what() << std::endl;
			}
			std::cout << "Final size of the population: " << opt.getPopulationSize() << std::endl;
			std::cout << "The top 3 are:" << std::endl;
            std::cout << opt.getFittestGroup(3) << std::endl;
            writeToFile(opt);
        } else if (!race.compare("Zerg"))
		{
			BuildListOptimizer<Zerg,Rush> opt(accuracy,individualSize);
			try{
                opt.initialize(unit,10,timeLimit,initPopSize);
                opt.optimize(unit,10,timeLimit,generations,reproductionRate,mutationRate,selectionRate);
			} catch(std::exception &e)
			{
				std::cerr << "Fehler\t" <<e.what() << std::endl;
			}
			std::cout << "Final size of the population: " << opt.getPopulationSize() << std::endl;
			std::cout << "The top 3 are:" << std::endl;
			std::cout << opt.getFittestGroup(3) << std::endl; 
            writeToFile(opt);
        } else if (!race.compare("Terran"))
		{
			BuildListOptimizer<Terran,Rush> opt(accuracy,individualSize);
			try{
                opt.initialize(unit,10,timeLimit,initPopSize);
                opt.optimize(unit,10,timeLimit,generations,reproductionRate,mutationRate,selectionRate);
			} catch(std::exception &e)
			{
				std::cerr << "Fehler\t" <<e.what() << std::endl;
			}
			std::cout << "Final size of the population: " << opt.getPopulationSize() << std::endl;
			std::cout << "The top 3 are:" << std::endl;
            std::cout << opt.getFittestGroup(3) << std::endl;
            writeToFile(opt);
        } else
		{
			throw std::invalid_argument("No known race");
		}
	} else
	{
		throw std::invalid_argument("No valid strategy");
	}
}

std::string getRace(std::string unit)
{
	std::string race;
	try 
	{
		TechnologyList l;
		InitTechTree<Protoss>(&l).initTechTree();
		if (l.findTechnology(unit)!=NULL)
		{
			return "Protoss";
		}
		TechnologyList a;
		InitTechTree<Zerg>(&a).initTechTree();
		if (a.findTechnology(unit)!=NULL)
		{
			return "Zerg";
		}
		TechnologyList b;
		InitTechTree<Terran>(&b).initTechTree();
		if (b.findTechnology(unit)!=NULL)
		{
			return "Terran";
		}
		
	} catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return "";
}

int main(int argc, char *argv[])
{
	//oldCall(argc, argv);
	std::string strategy;
	std::string race;
	std::string unit;
    std::string timeLimitStr;
    if (argc < 4)
	{
        std::cerr << "Usage: ./bin/runme [Unit] [Strategy] [Timelimit]" << std::endl;
		return -1;
	} else
	{
		unit = argv[1];
		strategy = argv[2];
        timeLimitStr = argv[3];
	}
	race = getRace(unit);
    std::cout << unit << "\t" << strategy << "\t" << race << std::endl;
    std::istringstream buffer(timeLimitStr);
    int timeLimit;
    buffer >> timeLimit;

	auto startTime = std::chrono::system_clock::now().time_since_epoch().count();
    startBuildListOptimizer(race, strategy, unit, timeLimit);
	auto endTime =  std::chrono::system_clock::now().time_since_epoch().count();

/*
	std::cout << "Required the following time to run: " << (endTime-startTime)*std::chrono::system_clock::period::num/std::chrono::system_clock::period::den << "s" << std::endl;
	std::cout << "Number of threads used: " << std::to_string(std::thread::hardware_concurrency()+1) << std::endl;
	*/
	//testTechList();

	return  0;
}

