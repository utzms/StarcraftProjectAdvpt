#include <Simulation.h>
#include <TechnologyList.h>

int main()
{
	//Simulation simulation;
	TechnologyList tech;

	tech.initBuildingList("./data/Protoss/buildings.txt");
	tech.initUnitList("./data/Protoss/units.txt");
	Technology *tec = tech.findUnit("Probe1").get();
	tec = tech.findUnit("Probe").get();
	/*
	if (!(tec == NULL))
	{
		std::cout << tec->getName() << "\t";
		std::cout << tec->getMineralsCost() << "\t";
		std::cout << tec->getGasCost() << "\t";
		std::cout << tec->getSupplyCost() << "\t";
		std::cout << tec->getBuildTime() << "\t";
		std::cout << std::endl;
	}
	tec = tech.findUnit("Probe").get();
		std::cout << tec->getName() << "\t";
		std::cout << tec->getMineralsCost() << "\t";
		std::cout << tec->getGasCost() << "\t";
		std::cout << tec->getSupplyCost() << "\t";
		std::cout << tec->getBuildTime() << "\t";
		std::cout << std::endl;
	*/
}
