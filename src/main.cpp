#include <Simulation.h>
#include <TechnologyList.h>
#include "InitTechTree.hpp"

int main()
{
	//Simulation simulation;
	TechnologyList tech;
	InitTechTree<ProtossTechTree> initTech(&tech);
	initTech.initUnitList();

	//tech.initUnitList("./data/Protoss/units.txt");
	//tech.initBuildingList("./data/Protoss/buildings.txt");
	Technology *tec = tech.findUnit("Probe").get();
	tec = tech.findUnit("Probe").get();
	while (((tec = tech.findUnit("Archon").get()) != NULL))
	{
		std::cout << tec->getName() << "\t";
		std::cout << tec->getMineralsCost() << "\t";
		std::cout << tec->getGasCost() << "\t";
		std::cout << tec->getSupplyCost() << "\t";
		std::cout << tec->getBuildTime() << "\t";
		std::vector<std::shared_ptr<Technology>> req = tec->getRequirements();
		for(int i = 0; i < req.size(); ++i)
		{
			std::cout << req[i]->getName() <<","; 
		}
		std::cout << std::endl;
	}
	while (((tec=tech.findBuilding("RoboticsBay").get()) != NULL))
	{
		std::cout << tec->getName() << "\t";
		std::cout << tec->getMineralsCost() << "\t";
		std::cout << tec->getGasCost() << "\t";
		std::cout << tec->getSupplyCost() << "\t";
		std::cout << tec->getBuildTime() << "\t";
		std::vector<std::shared_ptr<Technology>> req = tec->getRequirements();
		for(int i = 0; i < req.size(); ++i)
		{
			std::cout << req[i]->getName() <<","; 
		}
		std::cout << std::endl;
	}
}
