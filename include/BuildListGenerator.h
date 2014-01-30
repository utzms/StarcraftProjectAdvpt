#ifndef _BUILDLISTGENERATOR_H_
#define _BUILDLISTGENERATOR_H_

#include <random>
#include <vector>
#include <stdexcept>
#include <chrono>

#include "Debug.h"
#include "BuildList.h"
#include "TechnologyManager.h"
#include "TechnologyList.h"
#include "InitTechTree.hpp"

template <class RacePolicy>
class BuildListGenerator
{
	private:
		TechnologyList copyFrom;
		TechnologyManager<RacePolicy> *localTester;

		bool checkBuildListPossibility(std::vector<std::string> in);

	public:
		//std constructor initializes a TechList if needed
		BuildListGenerator();
		//no check, if techList is invalid/broken
		//this class demands a complete and working copy!!! Otherwise use standard Constructor
		BuildListGenerator(const TechnologyList &techList);

		//SpecialOne and weight are for weighting one Special Unit/Building with higher Priority in RandomGenerator
		//=> this Unit/Buildin is more likely to be found in a List
		void initRandomGenerator(std::string SpecialOne="", int weight=0);
                std::string getRandomTechnologyName();
		//length = wanted buildListSize
		std::shared_ptr<BuildList> buildOneRandomList(int length);
        std::vector<std::shared_ptr<BuildList>> buildManyRandomLists(int population, int length);
			
};

#endif
