

#include "RacePolicy.h"
#include "FitnessPolicy.h"
#include "BuildListOptimizer.cpp"

#include <stdexcept>

int main(int argc, char *argv[])
{
	size_t a=25;
	BuildListOptimizer<Protoss,Debug> opt(100,a);
	opt.initialize("Zealot",10,600,1000);
	opt.optimize("Zealot",10,600,5,5,5,50);
	//std::cout << opt.getFittestGroup(10) << std::endl;
	return  0;
}

