#include <Simulation.h>
#include <TechnologyList.h>
#include "InitTechTree.hpp"

int main()
{
	//Simulation simulation;
	TechnologyList tech;
	InitTechTree<ProtossTechTree> initTech(&tech);
	initTech.initTechTree();

	Technology *tec = tech.findUnit("Probe").get();
	tec = tech.findUnit("Probe").get();
	while (((tec = tech.findUnit("Archon").get()) != NULL))
	{
		std::cout << tec->getName() << "\t";
		std::cout << tec->getMineralsCost() << "\t";
		std::cout << tec->getGasCost() << "\t";
		std::cout << tec->getSupplyCost() << "\t";
		std::cout << tec->getBuildTime() << "\t";
		std::vector<std::vector<std::shared_ptr<Technology>>> req = tec->getRequirements();
		for(int i = 0; i < req.size(); ++i)
		{
			for (int j = 0; j < req[i].size(); ++j)
				std::cout << req[i][j]->getName() <<"/"; 
			std::cout << ",";
		}
		std::cout << std::endl;
	}
	while (((tec=tech.findBuilding("CyberneticsCore").get()) != NULL))
	{
		std::cout << tec->getName() << "\t";
		std::cout << tec->getMineralsCost() << "\t";
		std::cout << tec->getGasCost() << "\t";
		std::cout << tec->getSupplyCost() << "\t";
		std::cout << tec->getBuildTime() << "\t";
		std::vector<std::vector<std::shared_ptr<Technology>>> req = tec->getRequirements();
		for(int i = 0; i < req.size(); ++i)
		{
			for (int j = 0; j < req[i].size(); ++j)
				std::cout << req[i][j]->getName() <<"/";
			std::cout << ",";
		}
		std::cout << std::endl;
	}
}
