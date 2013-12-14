#include <iomanip> 
#include <TechnologyList.h>
//#include "InitTechTree.hpp"
#include "Technology.h"
#include "TechnologyManager.h"
int test()
{
    /* 
    std::shared_ptr<TechnologyList> tech(new TechnologyList());
	InitTechTree<ProtossTechTree> initTech(tech);
	initTech.initTechTree();

	std::vector<std::string> names;
	names.push_back("Probe");
	names.push_back("Pylon");
	names.push_back("Gateway");
	names.push_back("CyberneticsCore");
	names.push_back("TwilightCouncil");
	names.push_back("Stalker");
	(*tech).printAll();
	if (!((*tech).isBuildListPossible(names)))
	{
	// *tech.printAll();
		std::cerr << "FAIL" << std::endl;
		return -1;
	}
	(*tech).printAll();
    */
	return 0;
}

void test2() {
    // bla
    std::shared_ptr<TechnologyList> tmp(new TechnologyList());
    TechnologyManager<ProtossTechTree> techManager(tmp); 
    techManager.notifyCreation("Nexus");
	std::vector<std::string> names;
	names.push_back("Probe");
	names.push_back("Pylon");
	names.push_back("Gateway");
	names.push_back("CyberneticsCore");
	names.push_back("TwilightCouncil");
	names.push_back("Stalker");

	for(auto name : names)
    {
        if(techManager.checkEntityRequirements(name)) 
        {
            std::pair<bool, std::vector<std::string>> res;
            techManager.checkAndGetVanishing(name, res);
            
            if(!res.first)
            {
                std::cerr << "checkAndGetVanishing failed" << std::endl;
            }
            for(std::string entity : res.second)
            {
                std::cout << entity << " ";
                techManager.notifyDestruction(entity);
                std::cout << "destroyed" << std::endl;
            }
            techManager.notifyCreation(name);
            std::cout << name << " created" << std::endl;
        }
        else 
        {   
            std::cerr << "checkEntityRequirements failed! " << name << " could not be built" << std::endl;
        }
    }
    std::cout << "SUCCESS!!!" << std::endl;
    /*for(size_t i = 0; i < 5; ++i)
    {
        techManager.notifyCreation("Probe", false);
    }
    if(techManager.checkEntityRequirements("Probe", false)) 
    {
        std::cout << "Requirements ckeck ok: Probe can be built" << std::endl;
    }

    techManager.notifyDestruction("Nexus", true);
    if(!techManager.checkEntityRequirements("Probe", false)) 
    {
        std::cout << "Requirements ckeck ok: Probe can not be built" << std::endl;
    }
    */

}   
