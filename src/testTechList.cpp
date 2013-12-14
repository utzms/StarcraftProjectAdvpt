#include <iomanip> 
#include <TechnologyList.h>
//#include "InitTechTree.hpp"
#include "Technology.h"
#include "TechnologyManager.h"
int test()
{
	TechnologyList tech;
	InitTechTree<ProtossTechTree> initTech(&tech);
	initTech.initTechTree();

	std::vector<std::string> names;
	names.push_back("Probe");
	names.push_back("Pylon");
	names.push_back("Gateway");
	names.push_back("CyberneticsCore");
	names.push_back("TwilightCouncil");
	names.push_back("Stalker");
	tech.printAll();
	if (!(tech.isBuildListPossible(names)))
	{
	//tech.printAll();
		std::cerr << "FAIL" << std::endl;
		return -1;
	}
	tech.printAll();
	return 0;
}

int test2() {
    std::shared_ptr<TechnologyList> techList = std::shared_ptr<TechnologyList>(TechnologyList());
    TechnologyManager<ProtossTechTree> techManager(techList);

} 
