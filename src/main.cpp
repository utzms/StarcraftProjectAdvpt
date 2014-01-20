

#include "RacePolicy.h"
#include "FitnessPolicy.h"
#include "BuildListOptimizer.cpp"

#include <stdexcept>

int main(int argc, char *argv[])
{
	size_t a=25;
	BuildListOptimizer<Protoss,Debug> opt(100,a);
    opt.initialize("Zealot",10,500000,1000);
        std::cout << "Size of the population: " << opt.getPopulationSize() << std::endl;
        std::cout << opt.getFittestIndividual();
        opt.optimize("Zealot",10,500000,5,5,5,5);
        std::cout << "Size of the population: " << opt.getPopulationSize() << std::endl;
        std::cout << opt.getFittestIndividual();
	return  0;
}

